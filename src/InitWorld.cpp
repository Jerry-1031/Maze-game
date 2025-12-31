#include "../include/GameWorld.hpp"
#include "../include/GameManager.hpp"
#include "../include/GameObject.hpp"
#include "../include/Background.hpp"
#include "../include/Button.hpp"
#include "../include/Player.hpp"
#include "../include/Chest.hpp"
#include "../include/Board.hpp"
#include "../include/Box.hpp"
#include "../include/Piston.hpp"
#include "../include/Water.hpp"
#include "../include/Enemy.hpp"

void GameWorld::InitWorld()
{
    //create background
    auto background = std::make_shared<Background>();
    Add(background, LayerID::BG);

    if (game_manager->Get_cur_state() == GameState::MAIN_MENU)
    {
        auto start_game_button = std::make_shared<GameStartButton>(game_manager);
        Add(start_game_button, LayerID::UI);
    }

    else if (game_manager->Get_cur_state() == GameState::LEVEL_SELECT)
    {
        for (int i = 0; i < 10; i++)
        {
            int x = (i < 5 ? (250 + i * 240) : (250 + (i - 5) * 240));
            int y = (i < 5 ? 450 : 775);
            auto level_select_button = std::make_shared<LevelSelectButton>(x, y, i + 1, game_manager, this);
            Add(level_select_button, LayerID::UI);
        }
    }

    else if (game_manager->Get_cur_state() == GameState::GAME_PLAY)
    {

        auto return_button = std::make_shared<ReturnButton>(game_manager, this);
        Add(return_button, LayerID::UI);
        auto restart_button = std::make_shared<RestartButton>(game_manager, game_manager->Get_selected_level(), this);
        Add(restart_button, LayerID::UI);

        // Get current level config
        LevelConfig level_config = GetLevelConfig(game_manager->Get_selected_level());

        // Initialize maze_data
        maze_data.resize(level_config.rows);
        for (int y = 0; y < level_config.rows; y++)
        {
            maze_data[y].resize(level_config.cols);
            for (int x = 0; x < level_config.cols; x++)
            {
                maze_data[y][x] = GetLevelTile(level_config, y, x);
            }
        }

        // Create map grids
        for (int y = 0; y < level_config.rows; y++)
        {
            for (int x = 0; x < level_config.cols; x++)
            {
                int grid_x = left_margin + x * GRID_WIDTH;
                int grid_y = up_margin + y * GRID_WIDTH;
                std::string path = (maze_data[y][x] == static_cast<int>(Tile::WALL))
                                       ? "../../assets/wall.png"
                                       : "../../assets/grass.png";
                auto grid = std::make_shared<Grid>(grid_x, grid_y, path);
                Add(grid, LayerID::MAP);
            }
        }

        switch (game_manager->Get_selected_level())
        {
        case 1:
        {
            auto player = std::make_shared<Player>(3, 3, this);
            Add(player, LayerID::PLAYER);

            chest_row = 10; // needs to be explained explicitly
            chest_col = 15;
            auto chest = std::make_shared<Chest>(chest_row, chest_col, this);
            Add(chest, LayerID::AFTERGAME); // higher layer to ensure it update after winning

            break;
        }

        case 2:
        {
            auto player = std::make_shared<Player>(3, 3, this);
            Add(player, LayerID::PLAYER);

            chest_row = 4;
            chest_col = 15;
            auto chest = std::make_shared<Chest>(chest_row, chest_col, this);
            Add(chest, LayerID::AFTERGAME);

            Add(std::make_shared<Box>(4, 12, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(7, 3, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(4, 6, true, this), LayerID::OTHER);
            Add(std::make_shared<Box>(10, 3, true, this), LayerID::OTHER);
            Add(std::make_shared<Box>(7, 4, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(10, 14, true, this), LayerID::OTHER);

            break;
        }

        case 3:
        {
            auto player = std::make_shared<Player>(3, 3, this);
            Add(player, LayerID::PLAYER);

            chest_row = 7;
            chest_col = 15;
            auto chest = std::make_shared<Chest>(chest_row, chest_col, this);
            Add(chest, LayerID::AFTERGAME);

            Add(std::make_shared<Box>(5, 4, true, this), LayerID::OTHER);
            Add(std::make_shared<Box>(7, 12, false, this), LayerID::OTHER);

            auto pistonhead1 = std::make_shared<Pistonhead>(3, 5, Direction::DOWN, this);
            auto piston1 = std::make_shared<Piston>(3, 5, "red", Direction::DOWN, this);
            auto pressbutton1 = std::make_shared<PressButton>(4, 4, "red", this, pistonhead1);
            Add(pistonhead1, LayerID::OTHER);
            Add(piston1, LayerID::OTHER);
            Add(pressbutton1, LayerID::OTHER);

            auto pistonhead2 = std::make_shared<Pistonhead>(8, 7, Direction::UP, this);
            auto piston2 = std::make_shared<Piston>(8, 7, "blue", Direction::UP, this);
            auto pressbutton2 = std::make_shared<PressButton>(10, 7, "blue", this, pistonhead2);
            Add(pistonhead2, LayerID::OTHER);
            Add(piston2, LayerID::OTHER);
            Add(pressbutton2, LayerID::OTHER);
            
            auto pistonhead3 = std::make_shared<Pistonhead>(8, 12, Direction::UP, this);
            auto piston3 = std::make_shared<Piston>(8, 12, "green", Direction::UP, this);
            auto pressbutton3 = std::make_shared<PressButton>(3, 16, "green", this, pistonhead3);
            auto pressbutton4 = std::make_shared<PressButton>(11, 16, "green", this, pistonhead3);
            Add(pistonhead3, LayerID::OTHER);
            Add(piston3, LayerID::OTHER);
            Add(pressbutton3, LayerID::OTHER);
            Add(pressbutton4, LayerID::OTHER);



            break;
        }

        case 4:
        {
            auto player = std::make_shared<Player>(1, 1, this);
            Add(player, LayerID::PLAYER);

            chest_row = 12;
            chest_col = 17;
            auto chest = std::make_shared<Chest>(chest_row, chest_col, this);
            Add(chest, LayerID::AFTERGAME);

            Add(std::make_shared<Box>(4, 3, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(7, 3, true, this), LayerID::OTHER);
            Add(std::make_shared<Box>(2, 12, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(2, 16, true, this), LayerID::OTHER);
            Add(std::make_shared<Box>(12, 13, true, this), LayerID::OTHER);
            
            auto pistonhead = std::make_shared<Pistonhead>(13, 13, Direction::UP, this);
            auto piston = std::make_shared<Piston>(13, 13, "red", Direction::UP, this);
            auto pressbutton1 = std::make_shared<PressButton>(5, 10, "red", this, pistonhead);
            auto pressbutton2 = std::make_shared<PressButton>(3, 10, "red", this, pistonhead);
            auto pressbutton3 = std::make_shared<PressButton>(7, 16, "red", this, pistonhead);
            auto pressbutton4 = std::make_shared<PressButton>(8, 16, "red", this, pistonhead);
            auto pressbutton5 = std::make_shared<PressButton>(12, 10, "red", this, pistonhead);
            Add(pistonhead, LayerID::OTHER);
            Add(piston, LayerID::OTHER);
            Add(pressbutton1, LayerID::OTHER);
            Add(pressbutton2, LayerID::OTHER);
            Add(pressbutton3, LayerID::OTHER);
            Add(pressbutton4, LayerID::OTHER);
            Add(pressbutton5, LayerID::OTHER);


            break;
        }

        case 5:
        {
            auto player = std::make_shared<Player>(11, 16, this);
            Add(player, LayerID::PLAYER);

            chest_row = 4;
            chest_col = 15;
            auto chest = std::make_shared<Chest>(chest_row, chest_col, this);
            Add(chest, LayerID::AFTERGAME);

            Add(std::make_shared<Box>(10, 13, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(9, 11, true, this), LayerID::OTHER);
            Add(std::make_shared<Box>(4, 9, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(4, 12, true, this), LayerID::OTHER);

            auto pistonhead1 = std::make_shared<Pistonhead>(9, 12, Direction::LEFT, this);
            auto piston1 = std::make_shared<Piston>(9, 12, "red", Direction::LEFT, this);
            auto pressbutton1 = std::make_shared<PressButton>(7, 12, "red", this, pistonhead1);
            Add(pistonhead1, LayerID::OTHER);
            Add(piston1, LayerID::OTHER);
            Add(pressbutton1, LayerID::OTHER);

            auto pistonhead2 = std::make_shared<Pistonhead>(4, 8, Direction::UP, this);
            auto piston2 = std::make_shared<Piston>(4, 8, "blue", Direction::UP, this);
            auto pressbutton2 = std::make_shared<PressButton>(4, 9, "blue", this, pistonhead2);
            Add(pistonhead2, LayerID::OTHER);
            Add(piston2, LayerID::OTHER);
            Add(pressbutton2, LayerID::OTHER);

            auto pistonhead3 = std::make_shared<Pistonhead>(3, 12, Direction::DOWN, this);
            auto piston3 = std::make_shared<Piston>(3, 12, "green", Direction::DOWN, this);
            auto pressbutton3 = std::make_shared<PressButton>(8, 3, "green", this, pistonhead3);
            Add(pistonhead3, LayerID::OTHER);
            Add(piston3, LayerID::OTHER);
            Add(pressbutton3, LayerID::OTHER);

            Add(std::make_shared<Enemy>(11, 3, this), LayerID::ENEMY);
            Add(std::make_shared<Enemy>(7, 3, this), LayerID::ENEMY);
            Add(std::make_shared<Enemy>(7, 15, this), LayerID::ENEMY);
            
            break;
        }
        case 6:
        {
            auto player = std::make_shared<Player>(5, 18, this);
            Add(player, LayerID::PLAYER);

            chest_row = 12;
            chest_col = 17;
            auto chest = std::make_shared<Chest>(chest_row, chest_col, this);
            Add(chest, LayerID::AFTERGAME);

            Add(std::make_shared<Box>(2, 12, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(2, 17, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(8, 12, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(8, 17, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(12, 11, true, this), LayerID::OTHER);
            Add(std::make_shared<Box>(12, 12, true, this), LayerID::OTHER);
            Add(std::make_shared<Box>(12, 13, true, this), LayerID::OTHER);
            Add(std::make_shared<Box>(12, 14, true, this), LayerID::OTHER);

            auto pistonhead1_1 = std::make_shared<Pistonhead>(2, 11, Direction::RIGHT, this);
            auto piston1_1 = std::make_shared<Piston>(2, 11, "red", Direction::RIGHT, this);
            auto pistonhead1_2 = std::make_shared<Pistonhead>(13, 11, Direction::UP, this);
            auto piston1_2 = std::make_shared<Piston>(13, 11, "red", Direction::UP, this);
            auto pressbutton1 = std::make_shared<PressButton>(3, 3, "red", this, pistonhead1_1, pistonhead1_2);
            Add(pistonhead1_1, LayerID::OTHER);
            Add(piston1_1, LayerID::OTHER);
            Add(pistonhead1_2, LayerID::OTHER);
            Add(piston1_2, LayerID::OTHER);
            Add(pressbutton1, LayerID::OTHER);

            auto pistonhead2_1 = std::make_shared<Pistonhead>(2, 16, Direction::RIGHT, this);
            auto piston2_1 = std::make_shared<Piston>(2, 16, "blue", Direction::RIGHT, this);
            auto pistonhead2_2 = std::make_shared<Pistonhead>(13, 12, Direction::UP, this);
            auto piston2_2 = std::make_shared<Piston>(13, 12, "blue", Direction::UP, this);
            auto pressbutton2 = std::make_shared<PressButton>(5, 7, "blue", this, pistonhead2_1, pistonhead2_2);
            Add(pistonhead2_1, LayerID::OTHER);
            Add(piston2_1, LayerID::OTHER);
            Add(pistonhead2_2, LayerID::OTHER);
            Add(piston2_2, LayerID::OTHER);
            Add(pressbutton2, LayerID::OTHER);

            auto pistonhead3_1 = std::make_shared<Pistonhead>(8, 11, Direction::RIGHT, this);
            auto piston3_1 = std::make_shared<Piston>(8, 11, "green", Direction::RIGHT, this);
            auto pistonhead3_2 = std::make_shared<Pistonhead>(13, 13, Direction::UP, this);
            auto piston3_2 = std::make_shared<Piston>(13, 13, "green", Direction::UP, this);
            auto pressbutton3 = std::make_shared<PressButton>(9, 3, "green", this, pistonhead3_1, pistonhead3_2);
            Add(pistonhead3_1, LayerID::OTHER);
            Add(piston3_1, LayerID::OTHER);
            Add(pistonhead3_2, LayerID::OTHER);
            Add(piston3_2, LayerID::OTHER);
            Add(pressbutton3, LayerID::OTHER);

            auto pistonhead4_1 = std::make_shared<Pistonhead>(8, 16, Direction::RIGHT, this);
            auto piston4_1 = std::make_shared<Piston>(8, 16, "yellow", Direction::RIGHT, this);
            auto pistonhead4_2 = std::make_shared<Pistonhead>(13, 14, Direction::UP, this);
            auto piston4_2 = std::make_shared<Piston>(13, 14, "yellow", Direction::UP, this);
            auto pressbutton4 = std::make_shared<PressButton>(11, 7, "yellow", this, pistonhead4_1, pistonhead4_2);
            Add(pistonhead4_1, LayerID::OTHER);
            Add(piston4_1, LayerID::OTHER);
            Add(pistonhead4_2, LayerID::OTHER);
            Add(piston4_2, LayerID::OTHER);
            Add(pressbutton4, LayerID::OTHER);

            Add(std::make_shared<Enemy>(1, 12, this), LayerID::ENEMY);
            Add(std::make_shared<Enemy>(1, 17, this), LayerID::ENEMY);
            Add(std::make_shared<Enemy>(9, 12, this), LayerID::ENEMY);
            Add(std::make_shared<Enemy>(9, 17, this), LayerID::ENEMY);
            break;
        }

        case 7:
        {
            Add(std::make_shared<Player>(3, 3, this), LayerID::PLAYER);

            chest_row = 10;
            chest_col = 15;
            Add(std::make_shared<Chest>(chest_row, chest_col, this), LayerID::AFTERGAME);

            Add(std::make_shared<Box>(3, 4, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(5, 15, true, this), LayerID::OTHER);
            Add(std::make_shared<Box>(10, 12, true, this), LayerID::OTHER);
            Add(std::make_shared<Box>(10, 7, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(6, 15, true, this), LayerID::OTHER);

            auto pistonhead1 = std::make_shared<Pistonhead>(11, 12, Direction::UP, this);
            auto piston1 = std::make_shared<Piston>(11, 12, "red", Direction::UP, this);
            auto pressbutton1 = std::make_shared<PressButton>(3, 16, "red", this, pistonhead1);
            Add(pistonhead1, LayerID::OTHER);
            Add(piston1, LayerID::OTHER);
            Add(pressbutton1, LayerID::OTHER);

            auto pistonhead2 = std::make_shared<Pistonhead>(6, 14, Direction::RIGHT, this);
            auto piston2 = std::make_shared<Piston>(6, 14, "blue", Direction::RIGHT, this);
            auto pressbutton2 = std::make_shared<PressButton>(7, 15, "blue", this, pistonhead2);
            Add(pistonhead2, LayerID::OTHER);
            Add(piston2, LayerID::OTHER);
            Add(pressbutton2, LayerID::OTHER);

            auto pistonhead3 = std::make_shared<Pistonhead>(9, 7, Direction::DOWN, this);
            auto piston3 = std::make_shared<Piston>(9, 7, "green", Direction::DOWN, this);
            auto pressbutton3 = std::make_shared<PressButton>(8, 8, "green", this, pistonhead3);
            Add(pistonhead3, LayerID::OTHER);
            Add(piston3, LayerID::OTHER);
            Add(pressbutton3, LayerID::OTHER);

            Add(std::make_shared<WaterSource>(4, 15, false, this), LayerID::DECOR);
            Add(std::make_shared<WaterSource>(10, 6, false, this), LayerID::DECOR);

            Add(std::make_shared<Enemy>(11, 4, this), LayerID::ENEMY);
            break;
        }

        case 8:
        {
            Add(std::make_shared<Player>(5, 3, this), LayerID::PLAYER);

            chest_row = 10;
            chest_col = 15;
            Add(std::make_shared<Chest>(chest_row, chest_col, this), LayerID::AFTERGAME);

            Add(std::make_shared<Box>(6, 4, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(6, 7, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(6, 10, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(9, 5, true, this), LayerID::OTHER);
            Add(std::make_shared<Box>(11, 7, false, this), LayerID::OTHER);

            auto pistonhead1 = std::make_shared<Pistonhead>(4, 15, Direction::UP, this);
            auto piston1 = std::make_shared<Piston>(4, 15, "red", Direction::UP, this);
            auto pressbutton1 = std::make_shared<PressButton>(6, 4, "red", this, pistonhead1);
            Add(pistonhead1, LayerID::OTHER);
            Add(piston1, LayerID::OTHER);
            Add(pressbutton1, LayerID::OTHER);

            auto pistonhead2_1 = std::make_shared<Pistonhead>(10, 4, Direction::UP, this);
            auto piston2_1 = std::make_shared<Piston>(10, 4, "blue", Direction::UP, this);
            auto pistonhead2_2 = std::make_shared<Pistonhead>(4, 13, Direction::UP, this);
            auto piston2_2 = std::make_shared<Piston>(4, 13, "blue", Direction::UP, this);
            auto pressbutton2 = std::make_shared<PressButton>(6, 7, "blue", this, pistonhead2_1, pistonhead2_2);
            Add(pistonhead2_1, LayerID::OTHER);
            Add(piston2_1, LayerID::OTHER);
            Add(pistonhead2_2, LayerID::OTHER);
            Add(piston2_2, LayerID::OTHER);
            Add(pressbutton2, LayerID::OTHER);

            auto pistonhead3 = std::make_shared<Pistonhead>(10, 6, Direction::UP, this);
            auto piston3 = std::make_shared<Piston>(10, 6, "green", Direction::UP, this);
            auto pressbutton3 = std::make_shared<PressButton>(6, 10, "green", this, pistonhead3);
            Add(pistonhead3, LayerID::OTHER);
            Add(piston3, LayerID::OTHER);
            Add(pressbutton3, LayerID::OTHER);

            Add(std::make_shared<WaterSource>(9, 3, false, this), LayerID::DECOR);
            Add(std::make_shared<WaterSource>(3, 16, true, this), LayerID::DECOR);
            Add(std::make_shared<WaterSource>(9, 8, true, this), LayerID::DECOR);
            Add(std::make_shared<WaterSource>(7, 16, true, this), LayerID::DECOR);
            Add(std::make_shared<WaterSource>(11, 3, true, this), LayerID::DECOR);

            Add(std::make_shared<Enemy>(3, 3, this), LayerID::ENEMY);
            Add(std::make_shared<Enemy>(3, 5, this), LayerID::ENEMY);
            Add(std::make_shared<Enemy>(3, 7, this), LayerID::ENEMY);
            Add(std::make_shared<Enemy>(3, 9, this), LayerID::ENEMY);

            break;
        }

        case 9:
        {
            Add(std::make_shared<Player>(1, 1, this), LayerID::PLAYER);

            chest_row = 12;
            chest_col = 17;
            Add(std::make_shared<Chest>(chest_row, chest_col, this), LayerID::AFTERGAME);

            Add(std::make_shared<Box>(5, 3, true, this), LayerID::OTHER);
            Add(std::make_shared<Box>(6, 3, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(11, 1, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(5, 5, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(7, 10, true, this), LayerID::OTHER);

            Add(std::make_shared<WaterSource>(4, 3, false, this), LayerID::DECOR);
            Add(std::make_shared<WaterSource>(11, 3, true, this), LayerID::DECOR);
            Add(std::make_shared<WaterSource>(1, 9, true, this), LayerID::DECOR);
            Add(std::make_shared<WaterSource>(13, 9, true, this), LayerID::DECOR);
            Add(std::make_shared<WaterSource>(6, 10, true, this), LayerID::DECOR);
            Add(std::make_shared<WaterSource>(8, 10, true, this), LayerID::DECOR);
            Add(std::make_shared<WaterSource>(12, 11, true, this), LayerID::DECOR);

            auto pistonhead1 = std::make_shared<Pistonhead>(9, 10, Direction::UP, this);
            auto piston1 = std::make_shared<Piston>(9, 10, "blue", Direction::UP, this);
            auto pressbutton1 = std::make_shared<PressButton>(13, 9, "blue", this, pistonhead1);
            Add(pistonhead1, LayerID::OTHER);
            Add(piston1, LayerID::OTHER);
            Add(pressbutton1, LayerID::OTHER);
            
            auto pistonhead2 = std::make_shared<Pistonhead>(5, 10, Direction::DOWN, this);
            auto piston2 = std::make_shared<Piston>(5, 10, "green", Direction::DOWN, this);
            auto pressbutton2 = std::make_shared<PressButton>(1, 9, "green", this, pistonhead2);
            Add(pistonhead2, LayerID::OTHER);
            Add(piston2, LayerID::OTHER);
            Add(pressbutton2, LayerID::OTHER);

            auto pistonhead3 = std::make_shared<Pistonhead>(13, 12, Direction::UP, this);
            auto piston3 = std::make_shared<Piston>(13, 12, "yellow", Direction::UP, this);
            auto pressbutton3 = std::make_shared<PressButton>(4, 15, "yellow", this, pistonhead3);
            Add(pistonhead3, LayerID::OTHER);
            Add(piston3, LayerID::OTHER);
            Add(pressbutton3, LayerID::OTHER);

            Add(std::make_shared<Enemy>(1, 11, this), LayerID::ENEMY);

            break;
        }

        case 10:
        {
            Add(std::make_shared<Player>(2, 18, this), LayerID::PLAYER);

            chest_row = 12;
            chest_col = 16;
            Add(std::make_shared<Chest>(chest_row, chest_col, this), LayerID::AFTERGAME);

            Add(std::make_shared<Box>(4, 10, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(4, 12, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(9, 10, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(9, 12, false, this), LayerID::OTHER);
            Add(std::make_shared<Box>(1, 16, true, this), LayerID::OTHER);

            //Add(std::make_shared<WaterSource>(1, 11, false, this), LayerID::DECOR);
            //Add(std::make_shared<WaterSource>(13, 11, false, this), LayerID::DECOR);
            Add(std::make_shared<WaterSource>(6, 18, true, this), LayerID::DECOR);
            Add(std::make_shared<WaterSource>(9, 18, true, this), LayerID::DECOR);

            auto pistonhead1_1 = std::make_shared<Pistonhead>(11, 2, Direction::LEFT, this);
            auto piston1_1 = std::make_shared<Piston>(11, 2, "yellow", Direction::LEFT, this);
            auto pistonhead1_2 = std::make_shared<Pistonhead>(12, 4, Direction::LEFT, this);
            auto piston1_2 = std::make_shared<Piston>(12, 4, "yellow", Direction::LEFT, this);
            auto pressbutton1 = std::make_shared<PressButton>(4, 10, "yellow", this, pistonhead1_1, pistonhead1_2);
            Add(pistonhead1_1, LayerID::OTHER);
            Add(piston1_1, LayerID::OTHER);
            Add(pistonhead1_2, LayerID::OTHER);
            Add(piston1_2, LayerID::OTHER);
            Add(pressbutton1, LayerID::OTHER);

            auto pistonhead2_1 = std::make_shared<Pistonhead>(11, 4, Direction::LEFT, this);
            auto piston2_1 = std::make_shared<Piston>(11, 4, "blue", Direction::LEFT, this);
            auto pistonhead2_2 = std::make_shared<Pistonhead>(12, 8, Direction::LEFT, this);
            auto piston2_2 = std::make_shared<Piston>(12, 8, "blue", Direction::LEFT, this);
            auto pressbutton2 = std::make_shared<PressButton>(4, 12, "blue", this, pistonhead2_1, pistonhead2_2);
            Add(pistonhead2_1, LayerID::OTHER);
            Add(piston2_1, LayerID::OTHER);
            Add(pistonhead2_2, LayerID::OTHER);
            Add(piston2_2, LayerID::OTHER);
            Add(pressbutton2, LayerID::OTHER);

            auto pistonhead3_1 = std::make_shared<Pistonhead>(11, 6, Direction::LEFT, this);
            auto piston3_1 = std::make_shared<Piston>(11, 6, "green", Direction::LEFT, this);
            auto pistonhead3_2 = std::make_shared<Pistonhead>(12, 2, Direction::LEFT, this);
            auto piston3_2 = std::make_shared<Piston>(12, 2, "green", Direction::LEFT, this);
            auto pressbutton3 = std::make_shared<PressButton>(9, 10, "green", this, pistonhead3_1, pistonhead3_2);
            Add(pistonhead3_1, LayerID::OTHER);
            Add(piston3_1, LayerID::OTHER);
            Add(pistonhead3_2, LayerID::OTHER);
            Add(piston3_2, LayerID::OTHER);
            Add(pressbutton3, LayerID::OTHER);

            auto pistonhead4_1 = std::make_shared<Pistonhead>(11, 8, Direction::LEFT, this);
            auto piston4_1 = std::make_shared<Piston>(11, 8, "skyblue", Direction::LEFT, this);
            auto pistonhead4_2 = std::make_shared<Pistonhead>(12, 6, Direction::LEFT, this);
            auto piston4_2 = std::make_shared<Piston>(12, 6, "skyblue", Direction::LEFT, this);
            auto pressbutton4 = std::make_shared<PressButton>(9, 12, "skyblue", this, pistonhead4_1, pistonhead4_2);
            Add(pistonhead4_1, LayerID::OTHER);
            Add(piston4_1, LayerID::OTHER);
            Add(pistonhead4_2, LayerID::OTHER);
            Add(piston4_2, LayerID::OTHER);
            Add(pressbutton4, LayerID::OTHER);

            auto pistonhead5_1 = std::make_shared<Pistonhead>(8, 17, Direction::DOWN, this);
            auto piston5_1 = std::make_shared<Piston>(8, 17, "red", Direction::DOWN, this);
            auto pistonhead5_2 = std::make_shared<Pistonhead>(7, 17, Direction::UP, this);
            auto piston5_2 = std::make_shared<Piston>(7, 17, "red", Direction::UP, this);
            auto pressbutton5_1 = std::make_shared<PressButton>(6, 1, "red", this, pistonhead5_1, pistonhead5_2);
            auto pressbutton5_2 = std::make_shared<PressButton>(9, 3, "red", this, pistonhead5_1, pistonhead5_2);
            auto pressbutton5_3 = std::make_shared<PressButton>(7, 5, "red", this, pistonhead5_1, pistonhead5_2);
            auto pressbutton5_4 = std::make_shared<PressButton>(8, 7, "red", this, pistonhead5_1, pistonhead5_2);
            Add(pistonhead5_1, LayerID::OTHER);
            Add(piston5_1, LayerID::OTHER);
            Add(pistonhead5_2, LayerID::OTHER);
            Add(piston5_2, LayerID::OTHER);
            Add(pressbutton5_1, LayerID::OTHER);
            Add(pressbutton5_2, LayerID::OTHER);
            Add(pressbutton5_3, LayerID::OTHER);
            Add(pressbutton5_4, LayerID::OTHER);

            Add(std::make_shared<Enemy>(1, 1, this), LayerID::ENEMY);
            Add(std::make_shared<Enemy>(1, 3, this), LayerID::ENEMY);
            Add(std::make_shared<Enemy>(1, 5, this), LayerID::ENEMY);
            Add(std::make_shared<Enemy>(1, 7, this), LayerID::ENEMY);
            Add(std::make_shared<Enemy>(13, 1, this), LayerID::ENEMY);
            Add(std::make_shared<Enemy>(13, 3, this), LayerID::ENEMY);
            Add(std::make_shared<Enemy>(4, 18, this), LayerID::ENEMY);

            break;
        }

        default:
            break;
        }
    }
}