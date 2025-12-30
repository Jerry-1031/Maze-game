#ifndef WATER_HPP
#define WATER_HPP

#include "GameObject.hpp"
#include "utils.hpp"

class Water;
class GameWorld;

class WaterSource : public GameObject
{
private:
    GameWorld *gameworld;
    int row, col;
    int updatetick;
    std::shared_ptr<Water> water[4];
    bool is_lava;

public:
    WaterSource(int row, int col, bool is_lava, GameWorld *gameworld);
    void Update();
    int GetRow() const { return row; }
    int GetCol() const { return col; }
    bool is_Lava() const { return is_lava; }
};

class Water : public GameObject
{
private:
    GameWorld *gameworld;
    int row, col;
    int updatetick;
    int level, max_level;
    Direction main_dir;
    Direction source_dir;
    std::shared_ptr<Water> next_waters[4];
    int die_tick;  // Countdown for die (30 ticks)
    bool need_die; // Mark if need to die
    bool is_lava;

public:
    Water(int row, int col, bool is_lava, Direction main_dir, int level, Direction source_dir, GameWorld *gameworld);
    void Update();
    void CutSource();
    void MarkToDie() { need_die = true; } // Mark to die
    int GetRow() const { return row; }
    int GetCol() const { return col; }
    int GetLevel() const { return level; }
    Direction GetDirection() const { return main_dir; }

    bool isWater() const override { return true; } // water and lava

    bool is_Lava() const { return is_lava; }
};

#endif // WATER_HPP