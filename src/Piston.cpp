#include "../include/Piston.hpp"

#include "../include/GameWorld.hpp"
#include "../include/Box.hpp"

Piston::Piston(int row, int col, std::string color, Direction direction, GameWorld *gameworld)
    : GameObject(left_margin + col * GRID_WIDTH, up_margin + row * GRID_WIDTH, 1, GRID_WIDTH, GRID_WIDTH, "./assets/PressButton/" + color + "piston.png", "", "", Dir_to_Rota(direction), 1.0F), gameworld(gameworld)
{
    gameworld->SetMazeData(row, col, 1);
}

Pistonhead::Pistonhead(int row, int col, Direction direction, GameWorld *gameworld)
    : GameObject(left_margin + col * GRID_WIDTH, up_margin + row * GRID_WIDTH, 1, GRID_WIDTH, GRID_WIDTH, "./assets/PressButton/pistonhead.png", "./assets/sound/pistonout.ogg", "./assets/sound/pistonin.ogg", Dir_to_Rota(direction), 1.0F)
    , gameworld(gameworld), push(false), length(0), to_row(row), to_col(col), push_row(row), push_col(col), direction(direction), ori_x(GetX()), ori_y(GetY()), requiredButtons(0), pressedCount(0)
{
    switch (direction)
    {
    case Direction::UP:
        to_row--;
        push_row -= 2;
        break;
    case Direction::DOWN:
        to_row++;
        push_row += 2;
        break;
    case Direction::RIGHT:
        to_col++;
        push_col += 2;
        break;
    case Direction::LEFT:
        to_col--;
        push_col -= 2;
        break;
    }
}

void Pistonhead::Update()
{
    // Reset pressed count and update push state based on button presses
    UpdatePushState();
    ResetPressedCount();

    if ((!push) && length > 0)
    {
        if(length == 40){
            SetSoundVolume(GetSound2(), 0.3f);
            PlaySound(GetSound2()); 
        }
        length -= 4;
        if (length == 0)
        {
            gameworld->SetMazeData(to_row, to_col, 0);
        }
    }

    if (push && length < 40 && ((!gameworld->GetMazeData(to_row, to_col)) || length > 0))
    {
        if(length == 0){
            SetSoundVolume(GetSound1(), 0.3f);
            PlaySound(GetSound1()); 
        }
        length += 4;
    }

    if (push && length == 0 &&
        (!gameworld->GetMazeData(push_row, push_col)))
    {
        auto pushObjList = gameworld->GetObjectList(LayerID::OTHER);
        for (auto &obj : pushObjList)
        {
            if (obj->isPushable())
            {
                auto Box_obj = std::static_pointer_cast<Box>(obj);
                if (Box_obj->Getrow() == to_row && Box_obj->Getcol() == to_col)
                {
                    Box_obj->Push(direction, 4);
                    length += 4;
                    break;
                }
            }
        }
    }

    if (length)
        gameworld->SetMazeData(to_row, to_col, 1);

    switch (direction)
    {
    case Direction::UP:
        MoveTo(ori_x, ori_y - length);
        break;
    case Direction::DOWN:
        MoveTo(ori_x, ori_y + length);
        break;
    case Direction::RIGHT:
        MoveTo(ori_x + length, ori_y);
        break;
    case Direction::LEFT:
        MoveTo(ori_x - length, ori_y);
        break;
    }
}

bool PressButton::bePressed(std::shared_ptr<GameObject> obj)
{
    return (obj.get() != this &&
            obj->GetX() - GetX() < GRID_WIDTH / 2 &&
            obj->GetY() - GetY() < GRID_WIDTH / 2 &&
            obj->GetX() - GetX() > -1 * GRID_WIDTH / 2 &&
            obj->GetY() - GetY() > -1 * GRID_WIDTH / 2);
}

bool PressButton::isPressed()
{
    auto checkLayer = [&](LayerID layer) -> bool
    {
        auto objList = gameworld->GetObjectList(layer);
        for (auto &obj : objList)
        {
            if (bePressed(obj))
                return true;
        }
        return false;
    };
    return checkLayer(LayerID::PLAYER) || checkLayer(LayerID::ENEMY) || checkLayer(LayerID::OTHER);
}

void PressButton::Update()
{
    if (isPressed())
    {
        // the button is pressed, increment count for all connected pistonheads
        SetFade(0.5f);
        for (auto &head : pistonhead)
        {
            if (head)
                head->IncrementPressed();
        }
    }
    else{
        SetFade(1.0f);
    }
}