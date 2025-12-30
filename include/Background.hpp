#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "GameObject.hpp"

class Background : public GameObject
{
public:
    Background() : GameObject(0, 0, 1, WINDOWS_WIDTH, WINDOWS_HEIGHT, "./assets/background1.png") {}

    void Update() override {}
};

class Grid : public GameObject
{
public:
    Grid(int x, int y, std::string path) : GameObject(x, y, 1, GRID_WIDTH, GRID_WIDTH, path) {}

    void Update() override {}
};

#endif