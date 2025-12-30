#ifndef BULLET_HPP
#define BULLET_HPP

#include "GameObject.hpp"
#include "utils.hpp"

class GameWorld;

class Bullet : public GameObject
{
private:
    GameWorld *gameworld;
    int row;
    int col;
    int movetick;
    int speed;
    Direction direction;
    int pixel_x, pixel_y; // Precise pixel position for smooth movement

    bool canMoveTo(int to_row, int to_col);
    std::pair<int, int> getDirOffset(Direction dir);

public:
    Bullet(int row, int col, Direction dir, GameWorld *gameworld);

    void Update() override;

    int GetRow() const { return row; }
    int GetCol() const { return col; }
};

#endif
