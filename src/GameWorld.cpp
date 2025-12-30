#include <vector>
#include "../include/GameWorld.hpp"
#include "../include/GameManager.hpp"
#include "../include/GameObject.hpp"
#include "../include/Button.hpp"
#include "../include/Board.hpp"

GameWorld::GameWorld(GameManager *game_manager) : game_manager(game_manager), win(false), lose(false), after_game_tick(-1) {}

void GameWorld::Add(std::shared_ptr<GameObject> obj, LayerID layer)
{
    int layer_number = static_cast<int>(layer);
    if (obj && layer_number >= 0 && layer_number < LayerIDCount)
    {
        object_list[layer_number].push_back(std::move(obj));
    }
}

void GameWorld::AddText(std::shared_ptr<GameText> text)
{
    if (text)
        text_list.push_back(text);
}

void GameWorld::UpdateWorld()
{ // update all gameobjects

    // after winning, control the board and all the UI
    if ((win || lose) && after_game_tick == -1)
    { // generate the board
        auto board = std::make_shared<Board>(win, lose, game_manager);
        Add(board, LayerID::AFTERGAME);
        after_game_tick = 120;
    }
    if (after_game_tick > 0)
    { // wait for a second
        after_game_tick--;
    }
    if (after_game_tick == 0)
    { // generate all information and UI
        if (win)
        {
            if(game_manager->Get_selected_level() != 10){
                auto next_level_button = std::make_shared<NextLevelButton>(game_manager, game_manager->Get_selected_level() + 1);
                Add(next_level_button, LayerID::UI);
            }
            auto text = std::make_shared<GameText>("You Win!", 400, 400, 200, WHITE);
            AddText(text);
        }
        else
        {
            auto text = std::make_shared<GameText>("You Lose", 350, 400, 200, WHITE);
            AddText(text);
        }
        after_game_tick = -2;
    }

    // after the game (win or lose), only update the layer of UI and AFTERGAME
    int start_index = (win || lose) ? 7 : 0;
    for (int i = start_index; i < LayerIDCount; i++)
    {
        auto it = object_list[i].begin();
        while (it != object_list[i].end())
        {
            std::shared_ptr<GameObject> obj = *it;
            obj->Update();
            if (obj->GetHP() <= 0)
            {
                it = object_list[i].erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void GameWorld::RenderWorld()
{
    for (int i = 0; i < LayerIDCount; i++)
    { // render all gameobjects in the order of LayerID
        for (auto &obj : object_list[i])
        {
            obj->Render();
        }
    }
    for (auto &text : text_list)
    { // render all text controlled by gameobject
        text->Render();
    }
    switch (game_manager->Get_cur_state())
    { // render all text NOT controlled by gameobject
    case GameState::MAIN_MENU:
        DrawText("Maze Game", 550, 150, 100, WHITE);
        break;
    case GameState::LEVEL_SELECT:
        DrawText("Level Select", 520, 100, 100, WHITE);
        break;
    case GameState::GAME_PLAY:
        std::string s = "Level " + std::to_string(game_manager->Get_selected_level());
        DrawText(s.c_str(), 700, 70, 75, WHITE);
        break;
    }
}

int GameWorld::GetMazeRows() const
{
    return maze_data.size();
}

int GameWorld::GetMazeCols() const
{
    return maze_data.empty() ? 0 : maze_data[0].size();
}

int GameWorld::GetMazeData(int row, int col) const
{
    if (row < 0 || row >= GetMazeRows() || col < 0 || col >= GetMazeCols())
    {
        return -1;
    }
    return maze_data[row][col];
}

void GameWorld::SetMazeData(int row, int col, int value)
{
    if (row < 0 || row >= GetMazeRows() || col < 0 || col >= GetMazeCols())
    {
        return;
    }
    maze_data[row][col] = value;
}
