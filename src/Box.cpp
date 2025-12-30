#include "../include/Box.hpp"
#include "../include/GameWorld.hpp"
#include "../include/Water.hpp"

Box::Box(int row, int col, bool is_stone, GameWorld *gameworld)
    : GameObject(left_margin + col * GRID_WIDTH, up_margin + row * GRID_WIDTH, 30, GRID_WIDTH, GRID_WIDTH, is_stone ? "./assets/push_stone.png" : "./assets/push_box.png", "./assets/sound/push.mp3", "./assets/sound/break.mp3"),
     gameworld(gameworld), row(row), col(col), speed(0), deathtick(-1), direction(Direction::NONE), is_stone(is_stone)
{
    gameworld->SetMazeData(row, col, 1);
}

void Box::Update()
{
    if (deathtick == -1)
        gameworld->SetMazeData(row, col, 1);
    if (movetick)
    {
        switch (direction)
        {
        case Direction::UP:
            MoveTo(GetX(), GetY() - speed);
            break;
        case Direction::DOWN:
            MoveTo(GetX(), GetY() + speed);
            break;
        case Direction::LEFT:
            MoveTo(GetX() - speed, GetY());
            break;
        case Direction::RIGHT:
            MoveTo(GetX() + speed, GetY());
            break;
        }
        movetick--;
    }

    if(deathtick > 0){
        deathtick -- ;
    }
    else if(deathtick == 0){
        SetHP(0);
    }
}

void Box::Push(Direction new_dir, int new_speed)
{
    SetSoundVolume(GetSound1(), 0.2f);
    SetSoundPitch(GetSound1(), 2.0f);
    PlaySound(GetSound1()); 
    
    gameworld->SetMazeData(row, col, 0);
    direction = new_dir;
    speed = new_speed;
    movetick = GRID_WIDTH / speed;
    switch (direction)
    {
    case Direction::UP:
        row--;
        break;
    case Direction::DOWN:
        row++;
        break;
    case Direction::LEFT:
        col--;
        break;
    case Direction::RIGHT:
        col++;
        break;
    }

    if (!is_stone)
    {
        // wooden box be destroyed by lava
        auto waterlist = gameworld->GetObjectList(LayerID::DECOR);
        for (auto &obj : waterlist)
        {
            if (obj->isWater())
            {
                auto water = std::static_pointer_cast<Water>(obj);
                if (water->GetCol() == col && water->GetRow() == row && water->is_Lava())
                {
                    SetSoundVolume(GetSound2(), 0.4f);
                    SetSoundPitch(GetSound2(), 2.0f);
                    PlaySound(GetSound2()); 
                    deathtick = 15;
                }
            }
        }
    }
}