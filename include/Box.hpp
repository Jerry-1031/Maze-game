#ifndef Box_HPP
#define Box_HPP

#include "utils.hpp"
#include "GameObject.hpp"

class GameWorld;

class Box : public GameObject
{
private:
    GameWorld *gameworld;

    int row;
    int col;
    int speed;
    Direction direction;
    int movetick;
    int deathtick;
    bool is_stone;

public:
    Box(int row, int col, bool is_stone, GameWorld *gameworld);

    void Update();

    bool isPushable() const override { return true; }
    int Getrow() const { return row; }
    int Getcol() const { return col; }
    void Push(Direction new_dir, int new_speed);
    void Break();
    bool isStone() const { return is_stone; }
};

#endif