#include "../include/Water.hpp"
#include "../include/GameWorld.hpp"
#include "../include/Box.hpp"

WaterSource::WaterSource(int row, int col, bool is_lava, GameWorld *gameworld)
    : GameObject(left_margin + col * GRID_WIDTH, up_margin + row * GRID_WIDTH, 1, GRID_WIDTH, GRID_WIDTH, is_lava ? "./assets/lavasource.png" : "./assets/watersource.png"), gameworld(gameworld), row(row), col(col), updatetick(is_lava ? LAVA_UPDATE_TICK : WATER_UPDATE_TICK), is_lava(is_lava)
{
    for (int i = 0; i < 4; ++i)
    {
        water[i] = nullptr;
    }
}

bool HasWaterAt(GameWorld *gameworld, int target_row, int target_col)
{
    auto &decor_list = gameworld->GetObjectList(LayerID::DECOR);
    for (const auto &obj : decor_list)
    {
        Water *water_obj = dynamic_cast<Water *>(obj.get());
        if (water_obj && water_obj->GetRow() == target_row && water_obj->GetCol() == target_col)
        {
            return true;
        }
    }
    return false;
}

bool HasDifferWaterAt(GameWorld *gameworld, int target_row, int target_col, bool is_lava)
{
    auto &decor_list = gameworld->GetObjectList(LayerID::DECOR);
    for (const auto &obj : decor_list)
    {
        Water *water_obj = dynamic_cast<Water *>(obj.get());
        if (water_obj && water_obj->GetRow() == target_row && water_obj->GetCol() == target_col && water_obj->is_Lava() != is_lava)
        {
            return true;
        }
    }
    return false;
}

void WaterSource::Update()
{

    updatetick = updatetick ? (updatetick - 1) : (is_lava ? LAVA_UPDATE_TICK : WATER_UPDATE_TICK);
    if (updatetick == 0)
    {
        Direction dirs[] = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};
        for (int i = 0; i < 4; ++i)
        {
            Direction curr_dir = dirs[i];
            int target_row = row, target_col = col;
            switch (curr_dir)
            {
            case Direction::UP:
                target_row--;
                break;
            case Direction::DOWN:
                target_row++;
                break;
            case Direction::LEFT:
                target_col--;
                break;
            case Direction::RIGHT:
                target_col++;
                break;
            }

            if (water[i] && water[i]->GetHP() <= 0)
            {
                water[i] = nullptr;
            }

            if (gameworld->GetMazeData(target_row, target_col) == 0 && !water[i] && !HasWaterAt(gameworld, target_row, target_col))
            {
                water[i] = std::make_shared<Water>(
                    target_row, target_col, is_lava, curr_dir, 1, ReverseDir(curr_dir), gameworld);
                gameworld->Add(water[i], LayerID::DECOR);
            }
        }
    }

    if (gameworld->GetMazeData(row, col) == 1)
    {
        SetHP(0);
        for (int i = 0; i < 4; ++i)
        {
            if (water[i])
            {
                water[i]->CutSource();
                water[i]->SetHP(0);
                water[i] = nullptr;
            }
        }
    }
}
Water::Water(int row, int col, bool is_lava, Direction main_dir, int level, Direction source_dir, GameWorld *gameworld)
    : GameObject(left_margin + col * GRID_WIDTH, up_margin + row * GRID_WIDTH, 1, GRID_WIDTH, GRID_WIDTH, is_lava ? "./assets/lava.png" : "./assets/water.png", "./assets/sound/steam.mp3", "", Dir_to_Rota(main_dir), 1.0f - level * 0.1f), gameworld(gameworld), row(row), col(col), updatetick(is_lava ? LAVA_UPDATE_TICK : WATER_UPDATE_TICK), level(level), main_dir(main_dir), source_dir(source_dir), is_lava(is_lava), max_level(is_lava ? 3 : 5)
{
    for (int i = 0; i < 4; ++i)
    {
        next_waters[i] = nullptr;
    }
}

void Water::CutSource()
{
    for (auto &next_water : next_waters)
    {
        if (next_water)
        {
            next_water->CutSource();
            next_water->SetHP(0);
            next_water = nullptr;
        }
    }
}

void Water::Update()
{
    updatetick = updatetick ? (updatetick - 1) : (is_lava ? LAVA_UPDATE_TICK : WATER_UPDATE_TICK);
    if (updatetick == 0)
    {
        if (level >= max_level)
        {
            return;
        }
        Direction dirs[] = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};
        for (int i = 0; i < 4; ++i)
        {
            Direction curr_dir = dirs[i];
            if (curr_dir == source_dir)
            {
                continue;
            }
            int target_row = row, target_col = col;
            switch (curr_dir)
            {
            case Direction::UP:
                target_row--;
                break;
            case Direction::DOWN:
                target_row++;
                break;
            case Direction::LEFT:
                target_col--;
                break;
            case Direction::RIGHT:
                target_col++;
                break;
            }
            if (next_waters[i] && next_waters[i]->GetHP() <= 0)
            {
                next_waters[i] = nullptr;
            }
            if (gameworld->GetMazeData(target_row, target_col) == 0 && !next_waters[i] && !HasWaterAt(gameworld, target_row, target_col) && (level + 1) <= max_level)
            {
                next_waters[i] = std::make_shared<Water>(target_row, target_col, is_lava, curr_dir, level + 1, ReverseDir(curr_dir), gameworld);
                gameworld->Add(next_waters[i], LayerID::DECOR);
            }

            if (HasDifferWaterAt(gameworld, target_row, target_col, is_lava))
            { // water + lava = stone
                SetSoundPitch(GetSound1(), 2.0f);
                PlaySound(GetSound1()); 
                auto stone = std::make_shared<Box>(target_row, target_col, true, gameworld);
                gameworld->Add(stone, LayerID::OTHER);
                gameworld->SetMazeData(target_row, target_col, 1);
            }
        }
    }
    if (gameworld->GetMazeData(row, col))
    {
        SetHP(0);
        CutSource();
    }
}