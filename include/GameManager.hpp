#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <memory>
#include "../third_party/raylib-5.5_win64_msvc16/include/raylib.h"
#include "utils.hpp"
#include "GameWorld.hpp"

class GameManager
{
private:
    static std::unique_ptr<GameManager> instance;
    GameState cur_state;
    int selected_level;
    bool state_switch;
    std::shared_ptr<GameWorld> cur_world;
    Music bgm;

    GameManager() : cur_state(GameState::MAIN_MENU), selected_level(0), state_switch(false), cur_world(std::make_shared<GameWorld>(this))
    {
        cur_world->InitWorld();
        bgm = LoadMusicStream("../../assets/sound/bgm.mp3"); // new
        SetMusicPitch(bgm, 0.9f);
        PlayMusicStream(bgm);       // new
        SetMusicVolume(bgm, 0.25f); // new
    }

public:
    GameManager(const GameManager &) = delete;
    GameManager &operator=(const GameManager &) = delete;
    GameManager(GameManager &&) = delete;
    GameManager &operator=(GameManager &&) = delete;

    void DestroyInstance();

    static GameManager &GetInstance();
    void SwitchGameState(GameState new_state, int new_level);
    void Update();
    void Render();

    GameState Get_cur_state() const { return cur_state; }
    int Get_selected_level() const { return selected_level; }
    Music GetBgm() const { return bgm; }
};

#endif