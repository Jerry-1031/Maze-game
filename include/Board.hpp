#ifndef BOARD_HPP
#define BOARD_HPP

#include "GameObject.hpp"
#include "utils.hpp"

class GameManager;

class Board : public GameObject
{
private:
    bool win, lose;
    int movetick;
    GameManager* game_manager;

public:
    Board(bool win, bool lose, GameManager* gamemanager);

    ~Board();

    void Update() override;
};

#endif