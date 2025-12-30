#include "../include/Bullet.hpp"
#include "../include/GameWorld.hpp"
#include "../include/Enemy.hpp"
#include "../include/Box.hpp"
#include "../include/Water.hpp"

Bullet::Bullet(int row, int col, Direction dir, GameWorld *gameworld)
    : GameObject(left_margin + col * GRID_WIDTH, up_margin + row * GRID_WIDTH, 1, GRID_WIDTH / 2, GRID_WIDTH / 2, "./assets/bullet.png"), gameworld(gameworld), row(row), col(col), movetick(0), speed(16), direction(dir), pixel_x(left_margin + col * GRID_WIDTH + GRID_WIDTH / 4), pixel_y(up_margin + row * GRID_WIDTH + GRID_WIDTH / 4)
{
    // Center the bullet in the grid
    MoveTo(pixel_x, pixel_y);
}

std::pair<int, int> Bullet::getDirOffset(Direction dir)
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

bool Bullet::canMoveTo(int to_row, int to_col)
{
    // Check maze boundaries
    if (to_row < 0 || to_row >= gameworld->GetMazeRows() ||
        to_col < 0 || to_col >= gameworld->GetMazeCols())
    {
        return false;
    }

    // Check walls and obstacles
    if (gameworld->GetMazeData(to_row, to_col) != 0)
    {
        return false;
    }

    return true;
}

void Bullet::Update()
{
    auto [rowOff, colOff] = getDirOffset(direction);

    // Move bullet
    pixel_x += colOff * speed;
    pixel_y += rowOff * speed;
    MoveTo(pixel_x, pixel_y);

    // Update grid position
    int new_row = (pixel_y - up_margin + GRID_WIDTH / 2) / GRID_WIDTH;
    int new_col = (pixel_x - left_margin + GRID_WIDTH / 2) / GRID_WIDTH;

    // Check if bullet hit a wall or obstacle
    if (!canMoveTo(new_row, new_col))
    {
        SetHP(0); // Destroy bullet
        return;
    }

    row = new_row;
    col = new_col;

    // Check collision with enemies
    auto &enemyList = gameworld->GetObjectList(LayerID::ENEMY);
    for (auto &obj : enemyList)
    {
        auto enemy = std::dynamic_pointer_cast<Enemy>(obj);
        if (enemy && enemy->GetRow() == row && enemy->GetCol() == col)
        {
            if (!enemy->IsStunned())
            {
                enemy->Stun();
            }
            SetHP(0); // Destroy bullet after hitting enemy
            return;
        }
    }

    // Check if out of bounds (safety check)
    if (pixel_x < 0 || pixel_x > WINDOWS_WIDTH || pixel_y < 0 || pixel_y > WINDOWS_HEIGHT)
    {
        SetHP(0);
    }
}
