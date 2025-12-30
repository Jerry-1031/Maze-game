#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "GameObject.hpp"
#include "utils.hpp"

class GameWorld;

class Player : public GameObject
{
private:
    GameWorld *gameworld;
    int row;
    int col;
    int movetick;
    int speed, ori_speed;
    Direction direction;
    int trembletick;
    int shoot_cooldown;
    static const int SHOOT_COOLDOWN_MAX = 30; // 0.5 seconds at 60 FPS
    int space_press_count;      // Count of space presses to unlock shooting
    bool shoot_enabled;         // Whether shooting is unlocked
    static const int UNLOCK_PRESS_COUNT = 5;  // Presses needed to unlock

    void slowlyMove();
    void detectMove();
    void detectShoot();
    bool canMove(int gridx, int gridy);
    bool isChest(int to_row, int to_col);

    std::pair<int, int> getDirOffset(Direction dir);
    std::string getDirTexture(Direction dir);
    void handleDirInput(int key, Direction dir);

public:
    Player(int row, int col, GameWorld *gameworld)
        : GameObject(left_margin + col * GRID_WIDTH, up_margin + row * GRID_WIDTH, 100, GRID_WIDTH, GRID_WIDTH, "./assets/player/down.png", "assets/sound/water.mp3"), gameworld(gameworld), col(col), row(row), movetick(0), direction(Direction::DOWN), speed(4), ori_speed(4), trembletick(0), shoot_cooldown(0), space_press_count(0), shoot_enabled(false) {}

    void Update() override;

    int GetRow() const { return row; }
    int GetCol() const { return col; }
    Direction GetDirection() const { return direction; }
};

#endif