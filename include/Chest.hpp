#ifndef CHEST_HPP
#define CHEST_HPP

#include "GameObject.hpp"
#include "utils.hpp"

class GameWorld;

class Chest : public GameObject
{
private:
    GameWorld *gameworld;
    bool pre_reach_chest;
    int row;
    int col;

public:
    Chest(int row, int col, GameWorld *gameworld)
        : GameObject(left_margin + col * GRID_WIDTH, up_margin + row * GRID_WIDTH, 1, GRID_WIDTH, GRID_WIDTH, "./assets/box.png"), gameworld(gameworld), pre_reach_chest(false), row(row), col(col) {}

    void Update() override;
};

#endif