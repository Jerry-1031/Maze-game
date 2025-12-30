#include "../include/GameManager.hpp"

std::unique_ptr<GameManager> GameManager::instance = nullptr;

GameManager &GameManager::GetInstance()
{
    if (!instance)
    {
        instance = std::unique_ptr<GameManager>(new GameManager());
    }
    return *instance;
}

void GameManager::DestroyInstance()
{
    UnloadMusicStream(bgm); // new
    instance.reset();
}

void GameManager::SwitchGameState(GameState new_state, int new_level)
{
    cur_state = new_state;
    selected_level = new_level;
    state_switch = true;
}

void GameManager::Update()
{
    UpdateMusicStream(bgm); // new
    float current = GetMusicTimePlayed(bgm);
    if (current >= 92.0f)
    {
        StopMusicStream(bgm);
        PlayMusicStream(bgm);
    }

    if (cur_world)
        cur_world->UpdateWorld();

    if (state_switch)
    {
        if (cur_world)
            cur_world.reset();
        cur_world = std::make_shared<GameWorld>(this);
        cur_world->InitWorld();
        state_switch = false;
    }
}

void GameManager::Render()
{
    if (cur_world)
    {
        cur_world->RenderWorld();
    }
}
