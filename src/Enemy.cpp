#include "../include/Enemy.hpp"
#include "../include/GameWorld.hpp"
#include "../include/Box.hpp"
#include "../include/Water.hpp"
#include "../include/Piston.hpp"
#include <cmath>
#include <algorithm>

Enemy::Enemy(int row, int col, GameWorld *gameworld)
    : GameObject(left_margin + col * GRID_WIDTH, up_margin + row * GRID_WIDTH, 100, GRID_WIDTH, GRID_WIDTH, "./assets/enemy/down.png", "./assets/sound/steam.mp3"),
     gameworld(gameworld), row(row), col(col), movetick(0), speed(2), direction(Direction::DOWN), deathtick(-1), stunned(false), stun_timer(0)
{
}

std::pair<int, int> Enemy::getDirOffset(Direction dir)
{
    switch (dir)
    {
    case Direction::UP:
        return {-1, 0};
    case Direction::DOWN:
        return {1, 0};
    case Direction::LEFT:
        return {0, -1};
    case Direction::RIGHT:
        return {0, 1};
    default:
        return {0, 0};
    }
}

std::string Enemy::getDirTexture(Direction dir)
{
    switch (dir)
    {
    case Direction::UP:
        return "./assets/enemy/up.png";
    case Direction::LEFT:
        return "./assets/enemy/left.png";
    case Direction::RIGHT:
        return "./assets/enemy/right.png";
    default:
        return "./assets/enemy/down.png";
    }
}

bool Enemy::canMoveTo(int to_row, int to_col)
{
    // Check maze boundaries and walls
    if (to_row < 0 || to_row >= gameworld->GetMazeRows() || to_col < 0 || to_col >= gameworld->GetMazeCols())
    {
        return false;
    }

    if (gameworld->GetMazeData(to_row, to_col) != 0)
    {
        return false;
    }

    // Check for pushable objects (Box)
    auto otherlist = gameworld->GetObjectList(LayerID::OTHER);
    for (auto &obj : otherlist)
    {
        if (obj->isPushable())
        {
            auto box = std::static_pointer_cast<Box>(obj);
            if (box->Getrow() == to_row && box->Getcol() == to_col)
            {
                return false;
            }
        }
    }

    return true;
}

std::pair<int, int> Enemy::bfsNextStep(int start_row, int start_col, int target_row, int target_col)
{
    if (start_row == target_row && start_col == target_col)
    {
        return {start_row, start_col};
    }

    int rows = gameworld->GetMazeRows();
    int cols = gameworld->GetMazeCols();

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<std::pair<int, int>>> parent(rows, std::vector<std::pair<int, int>>(cols, {-1, -1}));

    std::queue<std::pair<int, int>> q;
    q.push({start_row, start_col});
    visited[start_row][start_col] = true;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!q.empty())
    {
        auto [cur_row, cur_col] = q.front();
        q.pop();

        if (cur_row == target_row && cur_col == target_col)
        {
            // Backtrack to find first step
            int r = target_row, c = target_col;
            while (parent[r][c].first != start_row || parent[r][c].second != start_col)
            {
                auto [pr, pc] = parent[r][c];
                r = pr;
                c = pc;
            }
            return {r, c};
        }

        for (int i = 0; i < 4; i++)
        {
            int nr = cur_row + dr[i];
            int nc = cur_col + dc[i];

            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                !visited[nr][nc] && canMoveTo(nr, nc))
            {
                visited[nr][nc] = true;
                parent[nr][nc] = {cur_row, cur_col};
                q.push({nr, nc});
            }
        }
    }

    // No path found, stay in place
    return {start_row, start_col};
}

Direction Enemy::getNextDirection()
{
    // Get player position
    auto &playerList = gameworld->GetObjectList(LayerID::PLAYER);
    if (playerList.empty())
        return Direction::NONE;

    auto player = playerList.front();
    int player_row = (player->GetY() - up_margin + GRID_WIDTH / 2) / GRID_WIDTH;
    int player_col = (player->GetX() - left_margin + GRID_WIDTH / 2) / GRID_WIDTH;

    auto [next_row, next_col] = bfsNextStep(row, col, player_row, player_col);

    if (next_row < row)
        return Direction::UP;
    if (next_row > row)
        return Direction::DOWN;
    if (next_col < col)
        return Direction::LEFT;
    if (next_col > col)
        return Direction::RIGHT;

    return Direction::NONE;
}

void Enemy::slowlyMove()
{
    auto [rowOff, colOff] = getDirOffset(direction);
    MoveTo(GetX() + colOff * speed, GetY() + rowOff * speed);
}

void Enemy::findAndMoveTowardsPlayer()
{
    Direction next_dir = getNextDirection();

    if (next_dir != Direction::NONE)
    {
        auto [rowOff, colOff] = getDirOffset(next_dir);
        int targetRow = row + rowOff;
        int targetCol = col + colOff;

        if (canMoveTo(targetRow, targetCol))
        {
            direction = next_dir;
            ChangeTexture(getDirTexture(direction));
            row = targetRow;
            col = targetCol;
            movetick = GRID_WIDTH / speed;
        }
    }
}

void Enemy::Stun()
{
    stunned = true;
    stun_timer = STUN_DURATION;
    ChangeTexture("./assets/enemy/stunned.png");
}

void Enemy::Update()
{
    // Permanently stunned - don't move
    if (stunned)
    {
        return;
    }

    if (movetick > 0)
    {
        slowlyMove();
        movetick--;
    }
    else
    {
        findAndMoveTowardsPlayer();
    }

    // Check collision only when both are fully in grid positions (no movement)
    auto &playerList = gameworld->GetObjectList(LayerID::PLAYER);
    if (!playerList.empty())
    {
        auto player = playerList.front();

        if (player->GetX() - GetX() < GRID_WIDTH / 2 && player->GetX() - GetX() > GRID_WIDTH / -2 
            && player->GetY() - GetY() < GRID_WIDTH / 2 && player->GetY() - GetY() > GRID_WIDTH / -2)
        {
            player->ChangeTexture("./assets/player/lose.png");
            gameworld->Lose();
            return;
        }
    }

    // Check for lava
    if(deathtick == -1){
        auto waterlist = gameworld->GetObjectList(LayerID::DECOR);
        for (auto &obj : waterlist)
        {
            if (obj->isWater())
            {
                auto water = std::static_pointer_cast<Water>(obj);
                if (water->is_Lava() && water->GetX() - GetX() < GRID_WIDTH / 2 && water->GetX() - GetX() > GRID_WIDTH / -2
                    && water->GetY() - GetY() < GRID_WIDTH / 2 && water->GetY() - GetY() > GRID_WIDTH / -2)
                {
                    SetSoundPitch(GetSound1(), 2.0f);
                    SetSoundVolume(GetSound1(), 1.5f);
                    PlaySound(GetSound1());
                    ChangeTexture("");
                    deathtick = 30;
                    stunned = true;
                }
            }
        }
    }
    

    if(deathtick > 0) deathtick--;
    if(deathtick == 0) {
        SetHP(0) ;
    }
}
