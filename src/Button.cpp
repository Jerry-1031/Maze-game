#include "../include/Button.hpp"
#include "../include/GameManager.hpp"

bool Button::is_clicked()
{
    return (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && is_touched());
}

bool Button::is_touched()
{
    return (GetMouseX() >= GetX() && GetMouseX() <= GetX() + GetSizeX() && GetMouseY() >= GetY() && GetMouseY() <= GetY() + GetSizeY());
}

void Button::Update()
{
    cur_is_touched = is_touched();

    if (is_clicked() && (!is_locked))
    {
        game_manager->SwitchGameState(switch_to_state, switch_to_level);
    }

    if (cur_is_touched && !pre_is_touched)
    {
        SetSoundVolume(GetSound1(), 0.3f);
        PlaySound(GetSound1()); 
        SetSize(GetSizeX() - 10, GetSizeY() - 10);
        MoveTo(GetX() + 5, GetY() + 5);
    }
    if (!cur_is_touched && pre_is_touched)
    {
        SetSize(GetSizeX() + 10, GetSizeY() + 10);
        MoveTo(GetX() - 5, GetY() - 5);
    }

    pre_is_touched = cur_is_touched;
}

LevelSelectButton::LevelSelectButton(int x, int y, int level, GameManager *game_manager, GameWorld *gameworld) : Button(GameState::GAME_PLAY, level, x, y, 150, 150, "./assets/button.png", game_manager), level(level), gameworld(gameworld)
{
    int text_x = level == 1 ? (x + 60) : (level == 10 ? (x + 40) : (x + 52));
    button_text = std::make_shared<GameText>(std::to_string(level), text_x, y + 25, 70, WHITE);
    gameworld->AddText(button_text);

    if(game_manager->Get_level_info(level - 1) == 0){
        ChangeTexture("./assets/locked_button.png");
        is_locked = true;
    }
    else if(game_manager->Get_level_info(level - 1) == 1)
        ChangeTexture("./assets/button.png");
    else if(game_manager->Get_level_info(level - 1) == 2)
        ChangeTexture("./assets/pass_button.png");
}

void NextLevelButton::Update()
{
    Button::Update();

    if (movetick == 60)
        MoveTo(GetX() + 20, GetY());
    if (movetick == 30)
        MoveTo(GetX() - 20, GetY());

    movetick--;
    if (movetick == 0)
        movetick = 60;
}

void RestartButton::Update()
{
    Button::Update();

    if ((gameworld->GetWin() || gameworld->GetLose()) && movetick == -1)
    {
        movetick = 120;
        MoveTo(WINDOWS_WIDTH, WINDOWS_HEIGHT);
    }
    if (movetick > 0)
    {
        movetick--;
    }
    if (movetick == 0)
    {
        if (gameworld->GetWin())
            MoveTo(725, 850);
        else
            MoveTo(925, 850);
        movetick = -2;
    }
    if (movetick == -2 && gameworld->GetLose())
    {
        if (movetick2 == 60)
            MoveTo(GetX() + 20, GetY());
        if (movetick2 == 30)
            MoveTo(GetX() - 20, GetY());

        movetick2--;
        if (movetick2 == 0)
            movetick2 = 60;
    }
}

void ReturnButton::Update()
{
    Button::Update();

    if ((gameworld->GetWin() || gameworld->GetLose()) && movetick == -1)
    {
        movetick = 120;
        MoveTo(WINDOWS_WIDTH, WINDOWS_HEIGHT);
    }
    if (movetick > 0)
    {
        movetick--;
    }
    if (movetick == 0)
    {
        MoveTo(525, 850);
        movetick = -2;
    }
}