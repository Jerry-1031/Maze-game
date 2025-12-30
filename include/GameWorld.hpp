#ifndef GAMEWORLD_HPP
#define GAMEWORLD_HPP

#include <list>
#include <memory>
#include <vector>
#include "utils.hpp"

class GameManager;
class GameObject;
class GameText;

class GameWorld
{
private:
    GameManager *game_manager;
    std::list<std::shared_ptr<GameObject>> object_list[LayerIDCount]; // object list for different LayerID
    std::list<std::shared_ptr<GameText>> text_list;                   // text that controlled by game object
    std::vector<std::vector<bool>> maze_data;

    int chest_row, chest_col;
    bool win, lose; // judge the winning
    int after_game_tick;

public:
    GameWorld(GameManager *game_manager);
    void Add(std::shared_ptr<GameObject> obj, LayerID layer);
    void AddText(std::shared_ptr<GameText> text);
    void InitWorld();
    void UpdateWorld();
    void RenderWorld();
    ~GameWorld() = default;

    int Get_chest_row() const { return chest_row; }
    int Get_chest_col() const { return chest_col; }
    void Win() { win = true; }
    bool GetWin() const { return win; }
    void Lose() { lose = true; }
    bool GetLose() const { return lose; }

    int GetMazeRows() const;
    int GetMazeCols() const;
    int GetMazeData(int row, int col) const;
    void SetMazeData(int row, int col, int value);

    std::list<std::shared_ptr<GameObject>> &GetObjectList(LayerID layerID)
    {
        // new
        int index = static_cast<int>(layerID);
        return object_list[index];
    }
};

#endif // GAMEWORLD_HPP