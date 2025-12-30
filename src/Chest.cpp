#include "../include/Chest.hpp"
#include "../include/GameObject.hpp"
#include "../include/GameWorld.hpp"

void Chest::Update()
{
    gameworld->SetMazeData(row, col, 1);
    if (gameworld->GetWin() && !pre_reach_chest)
    {
        ChangeTexture("./assets/box_open.png");
    }
    pre_reach_chest = gameworld->GetWin();
}
