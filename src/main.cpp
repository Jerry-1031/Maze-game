#include <memory>

#include "../include/GameManager.hpp"
#include "../include/utils.hpp"

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(WINDOWS_WIDTH, WINDOWS_HEIGHT, "Maze Game");
    SetTargetFPS(60);
    InitAudioDevice();
    auto &gamemanager = GameManager::GetInstance();

    while (!WindowShouldClose())
    {
        gamemanager.Update();
        BeginDrawing();
        ClearBackground(SKYBLUE);
        gamemanager.Render();
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    gamemanager.DestroyInstance();
    return 0;
}