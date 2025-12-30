#include "../include/Player.hpp"
#include "../include/utils.hpp"
#include "../include/GameWorld.hpp"
#include "../include/Box.hpp"
#include "../include/Water.hpp"
#include "../include/Bullet.hpp"
#include "../include/Enemy.hpp"

// Get row/column offset based on movement direction
std::pair<int, int> Player::getDirOffset(Direction dir)
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

// Get texture path corresponding to movement direction
std::string Player::getDirTexture(Direction dir)
{
    switch (dir)
    {
    case Direction::UP:
        return "./assets/player/up.png";
    case Direction::LEFT:
        return "./assets/player/left.png";
    case Direction::RIGHT:
        return "./assets/player/right.png";
    default:
        return "./assets/player/down.png";
    }
}

// Handle input for specific direction (key press + texture + movement logic)
void Player::handleDirInput(int key, Direction dir)
{
    if (IsKeyPressed(key))
    {
        ChangeTexture(getDirTexture(dir).c_str());
        direction = dir;

        auto [rowOff, colOff] = getDirOffset(dir);
        int targetRow = row + rowOff;
        int targetCol = col + colOff;

        auto waterlist = gameworld->GetObjectList(LayerID::DECOR);
        bool change_speed = false;
        for (auto &obj : waterlist)
        {
            if (obj->isWater())
            {
                auto water = std::static_pointer_cast<Water>(obj);
                
                if (water->GetCol() == col && water->GetRow() == row)
                { // water change speed
                    if (water->GetDirection() == direction)
                    {
                        speed = ori_speed * 2;
                        change_speed = true;
                    }
                    else if (water->GetDirection() == ReverseDir(direction))
                    {
                        speed = ori_speed / 2;
                        change_speed = true;
                    }
                    SetSoundPitch(GetSound1(), 1.0f);
                    SetSoundVolume(GetSound1(), 0.4f);
                    PlaySound(GetSound1());
                    break;
                }
                
            }
        }
        if (!change_speed)
            speed = ori_speed;

        if (isChest(targetRow, targetCol))
        {
            gameworld->Win();
            ChangeTexture("./assets/player/win.png");
        }
        else if (canMove(targetRow, targetCol))
        {
            row = targetRow;
            col = targetCol;
            movetick = GRID_WIDTH / speed;
        }
        else
        { // new, detect push
            int pushRow = row + rowOff * 2;
            int pushCol = col + colOff * 2;
            auto pushObjList = gameworld->GetObjectList(LayerID::OTHER);
            for (auto &obj : pushObjList)
            {
                if (obj->isPushable())
                {
                    auto Box_obj = std::static_pointer_cast<Box>(obj);
                    if (Box_obj->Getrow() == targetRow && Box_obj->Getcol() == targetCol && canMove(pushRow, pushCol))
                    {
                        Box_obj->Push(direction, speed);
                        row = targetRow;
                        col = targetCol;
                        movetick = GRID_WIDTH / speed;
                        break;
                    }
                }
            }
        }
    }
}

// Check if target position is movable (not a wall)
bool Player::canMove(int to_row, int to_col)
{
    return gameworld->GetMazeData(to_row, to_col) == 0;
}

// Check if target position contains chest
bool Player::isChest(int to_row, int to_col)
{
    return (gameworld->Get_chest_col() == to_col) && (gameworld->Get_chest_row() == to_row);
}

// Move player slowly by grid unit (based on speed)
void Player::slowlyMove()
{
    auto [rowOff, colOff] = getDirOffset(direction);
    MoveTo(GetX() + colOff * speed, GetY() + rowOff * speed);
}

// Detect movement input (delegated to handleDirInput)
void Player::detectMove()
{
    handleDirInput(KEY_W, Direction::UP);
    handleDirInput(KEY_UP, Direction::UP);
    handleDirInput(KEY_A, Direction::LEFT);
    handleDirInput(KEY_LEFT, Direction::LEFT);
    handleDirInput(KEY_S, Direction::DOWN);
    handleDirInput(KEY_DOWN, Direction::DOWN);
    handleDirInput(KEY_D, Direction::RIGHT);
    handleDirInput(KEY_RIGHT, Direction::RIGHT);
}

// Detect shoot input (space key)
void Player::detectShoot()
{
    if (shoot_cooldown > 0) {
        shoot_cooldown--;
        return;
    }

    if (IsKeyPressed(KEY_SPACE)) {
        // Create bullet in the direction player is facing
        auto bullet = std::make_shared<Bullet>(row, col, direction, gameworld);
        gameworld->Add(bullet, LayerID::EFFECT);
        shoot_cooldown = SHOOT_COOLDOWN_MAX;
    }
}

// Update player state (movement + animation)
void Player::Update()
{
    // Check if crushed by a Box (stone)
    auto objList = gameworld->GetObjectList(LayerID::OTHER);
    for (auto &obj : objList)
    {
        if (obj->isPushable())
        {
            auto box = std::static_pointer_cast<Box>(obj);
            if (box->Getrow() == row && box->Getcol() == col && box->isStone() == true)
            {
                gameworld->Lose();
                ChangeTexture("./assets/player/lose.png");
                return;
            }
        }
    }

    //check if touch the lava
    auto waterlist = gameworld->GetObjectList(LayerID::DECOR);
    for (auto &obj : waterlist)
    {
        if (obj->isWater())
        {
            auto lava = std::static_pointer_cast<Water>(obj);
            if (lava->is_Lava() && lava->GetX() - GetX() <= GRID_WIDTH / 2 && lava->GetX() - GetX() >= GRID_WIDTH / -2
                && lava->GetY() - GetY() <= GRID_WIDTH / 2 && lava->GetY() - GetY() >= GRID_WIDTH / -2)
            {
                gameworld->Lose();
                ChangeTexture("./assets/player/lose.png");
                return;
            }
        }
    }

    // tremble
    if (trembletick == 30)
    {
        SetSize(GetSizeX(), GetSizeY() - 2);
        MoveTo(GetX(), GetY() + 2);
    }
    if (trembletick == 15)
    {
        SetSize(GetSizeX(), GetSizeY() + 2);
        MoveTo(GetX(), GetY() - 2);
    }
    trembletick = (trembletick == 0) ? 30 : trembletick - 1;

    if (movetick)
    {
        slowlyMove();
        movetick--;
    }
    else
    {
        detectMove();
    }

    // Always allow shooting regardless of movement
    // Unlock shooting after pressing space 5 times
    if (!shoot_enabled) {
        if (IsKeyPressed(KEY_SPACE)) {
            space_press_count++;
            if (space_press_count >= UNLOCK_PRESS_COUNT) {
                shoot_enabled = true;
            }
        }
    } else {
        detectShoot();
    }
}