#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "GameObject.hpp"

class GameManager;
class GameWorld;

class Button : public GameObject
{
protected:
    GameManager *game_manager;
    GameState switch_to_state;
    int switch_to_level;
    bool pre_is_touched;
    bool cur_is_touched;

    bool is_clicked();
    bool is_touched();

public:
    Button(GameState state, int level, int x, int y, int size_x, int size_y, std::string path, GameManager *game_manager)
        : GameObject(x, y, 1, size_x, size_y, path, "./assets/sound/UI.mp3"), game_manager(game_manager), switch_to_state(state), switch_to_level(level), pre_is_touched(false), cur_is_touched(false) {}

    virtual void Update() override;
};

class GameStartButton : public Button
{
public:
    GameStartButton(GameManager *game_manager)
        : Button(GameState::LEVEL_SELECT, 0, (WINDOWS_WIDTH - 500) / 2, WINDOWS_HEIGHT / 2 - 100, 500, 300, "./assets/gamestart.png", game_manager) {}
};

class LevelSelectButton : public Button
{
private:
    int level;
    GameWorld *gameworld;
    std::shared_ptr<GameText> button_text;

public:
    LevelSelectButton(int x, int y, int level, GameManager *game_manager, GameWorld *gameworld);
};

class RestartButton : public Button
{
private:
    GameWorld *gameworld;
    int movetick;  // for move after game
    int movetick2; // for tremble when losing
public:
    RestartButton(GameManager *game_manager, int switch_to_level, GameWorld *gameworld)
        : Button(GameState::GAME_PLAY, switch_to_level, 180, 15, 150, 150, "./assets/restart.png", game_manager), gameworld(gameworld), movetick(-1), movetick2(60) {}

    void Update() override;
};

class ReturnButton : public Button
{
private:
    GameWorld *gameworld;
    int movetick;

public:
    ReturnButton(GameManager *game_manager, GameWorld *gameworld)
        : Button(GameState::LEVEL_SELECT, 0, 15, 15, 150, 150, "./assets/return.png", game_manager), gameworld(gameworld), movetick(-1) {}

    void Update() override;
};

class NextLevelButton : public Button
{
private:
    int movetick;

public:
    NextLevelButton(GameManager *game_manager, int switch_to_level)
        : Button(GameState::GAME_PLAY, switch_to_level, 925, 850, 150, 150, "./assets/nextlevel.png", game_manager), movetick(60) {}

    void Update() override;
};

class EasterEggLevelButton : public Button
{
private:
    int level;
    GameWorld *gameworld;
    std::shared_ptr<GameText> button_text;

public:
    EasterEggLevelButton(int x, int y, int level, std::string display_text, GameManager *game_manager, GameWorld *gameworld);
};

#endif