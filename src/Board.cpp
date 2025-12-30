#include "../include/Board.hpp"

#include "../include/GameManager.hpp"

Board::Board(bool win, bool lose, GameManager* gamemanager) : GameObject(100, -1000, 1, 1400, 1000, "./assets/board.png", "./assets/sound/win.mp3", "./assets/sound/lose.mp3"), movetick(120), game_manager(gamemanager) {
    if(win){
        SetSoundVolume(GetSound1(), 0.3f);
        PlaySound(GetSound1()); 
    }
    else if(lose){
        StopMusicStream(game_manager->GetBgm());
        SetSoundVolume(GetSound2(), 0.3f);
        PlaySound(GetSound2());
    }
}

Board::~Board(){
    if(lose) PlayMusicStream(game_manager->GetBgm());
}

void Board::Update()
{
    movetick--;

    if (0 <= movetick && movetick <= 60)
    {
        MoveTo(GetX(), GetY() + 19);
    }
}