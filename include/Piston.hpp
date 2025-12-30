#ifndef PISTON_HPP
#define PISTON_HPP

#include <vector>

#include "utils.hpp"
#include "GameObject.hpp"

class GameWorld;

class Piston : public GameObject
{
private:
    GameWorld *gameworld;

public:
    Piston(int row, int col, std::string color, Direction direction, GameWorld *gameworld);

    void Update() {}
};

class Pistonhead : public GameObject
{
private:
    GameWorld *gameworld;
    bool push;
    int length;
    int ori_x, ori_y;
    int to_row, to_col;
    int push_row, push_col;
    Direction direction;
    int requiredButtons; // Number of buttons required to activate
    int pressedCount;    // Current pressed button count this frame

public:
    Pistonhead(int row, int col, Direction direction, GameWorld *gameworld);

    void SetPush(bool new_push) { push = new_push; }
    void AddRequiredButton() { requiredButtons++; }                      // Call when binding a button
    void IncrementPressed() { pressedCount++; }                          // Call when a button is pressed
    void ResetPressedCount() { pressedCount = 0; }                       // Reset at frame start
    void UpdatePushState() { push = (pressedCount >= requiredButtons); } // Check if all pressed
    void Update();
};

#define pPistonhead std::shared_ptr<Pistonhead>

class PressButton : public GameObject
{
private:
    GameWorld *gameworld;
    std::vector<pPistonhead> pistonhead;

    bool bePressed(std::shared_ptr<GameObject> obj);
    bool isPressed(); // Check if this button is pressed

public:
    PressButton(int row, int col, std::string color, GameWorld *gameworld, pPistonhead head1, pPistonhead head2 = nullptr, pPistonhead head3 = nullptr, pPistonhead head4 = nullptr, pPistonhead head5 = nullptr)
        : GameObject(left_margin + col * GRID_WIDTH, up_margin + row * GRID_WIDTH, 1, GRID_WIDTH, GRID_WIDTH, "./assets/PressButton/" + color + "button.png"), gameworld(gameworld)
    {
        if (head1)
        {
            pistonhead.push_back(head1);
            head1->AddRequiredButton();
        }
        if (head2)
        {
            pistonhead.push_back(head2);
            head2->AddRequiredButton();
        }
        if (head3)
        {
            pistonhead.push_back(head3);
            head3->AddRequiredButton();
        }
        if (head4)
        {
            pistonhead.push_back(head4);
            head4->AddRequiredButton();
        }
        if (head5)
        {
            pistonhead.push_back(head5);
            head5->AddRequiredButton();
        }
    }

    void Update();
};

#endif