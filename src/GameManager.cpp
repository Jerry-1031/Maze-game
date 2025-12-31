#include "../include/GameManager.hpp"

#include <sstream>
#include <string>
#include <fstream>

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
    WriteSaveFile();
    instance.reset();
}

void GameManager::SwitchGameState(GameState new_state, int new_level)
{
    if(new_state != GameState::LEVEL_SELECT){
        StopMusicStream(bgm);
        PlayMusicStream(bgm);
    }
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

void GameManager::ReadSaveFile() {
    std::ifstream saveFile("./assets/saves/saves.txt");
    if (!saveFile.is_open()) {
        std::cerr << "Error! saves.txt can't open" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(saveFile, line)) {
        if (line.empty() || line[0] != 'l') {
            continue;
        }

        int level = line[6] - '0', state;
        if(line[7] == '0'){
            level = 10;
            state = line[10] - '0';
        }
        else{
            state = line[9] - '0';
        }

        if (level >= 1 && level <= 10 && (state == 0 || state == 1 || state == 2)) {
            level_info[level - 1] = state;
        }
    }
    saveFile.close();
}

void GameManager::WriteSaveFile() {
    std::ofstream saveFile("./assets/saves/saves.txt");
    if (!saveFile.is_open()) {
        std::cerr << "Error! saves.txt can't open" << std::endl;
        return;
    }

    saveFile << "Saves of the maze game:\n";
    saveFile << "(0 for locked, 1 for unlocked but unpassed, 2 for passed)\n\n";

    for (int i = 0; i < 10; ++i) {
        saveFile << "level_" << (i + 1) << ": " << level_info[i] << "\n";
    }

    saveFile.close();
}