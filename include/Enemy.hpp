#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <queue>
#include <vector>
#include <utility>
#include "GameObject.hpp"
#include "utils.hpp"

class GameWorld;

class Enemy : public GameObject
{
private:
    GameWorld *gameworld;
    int row;
    int col;
    int movetick;
    int speed;
    Direction direction;
    bool stunned;
    int stun_timer;
    int deathtick;
    static const int STUN_DURATION = 180; // 3 seconds at 60 FPS

    void slowlyMove();
    void findAndMoveTowardsPlayer();
    bool canMoveTo(int to_row, int to_col);
    Direction getNextDirection();
    std::string getDirTexture(Direction dir);
    std::pair<int, int> getDirOffset(Direction dir);

    // BFS pathfinding
    std::pair<int, int> bfsNextStep(int start_row, int start_col, int target_row, int target_col);

public:
    Enemy(int row, int col, GameWorld *gameworld);

    void Update() override;

    int GetRow() const { return row; }
    int GetCol() const { return col; }
    bool IsStunned() const { return stunned; }
    void Stun();
};

#endif
