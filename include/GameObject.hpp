#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "../third_party/raylib-5.5_win64_msvc16/include/raylib.h"
#include <string>
#include "utils.hpp"

class GameObject
{
private:
    int hp;
    int pos_x;
    int pos_y;
    int size_x;
    int size_y;
    Texture2D texture;
    float rotation;
    float fade;
    bool is_tex_loaded;
    Sound sound1;
    Sound sound2;

public:
    GameObject(int x, int y, int hp, int sx, int sy, const std::string &tex_path = "", const std::string &sound_path1 = "", const std::string &sound_path2 = "", float rotation = 0.0f, float fade = 1.0f)
        : hp(hp), pos_x(x), pos_y(y), size_x(sx), size_y(sy), texture({0, 0, 0, 0, 0}), sound1(Sound{0}), sound2(Sound{0}), rotation(rotation), fade(fade), is_tex_loaded(false)
    {
        if(!tex_path.empty()){
            ChangeTexture(tex_path);
        }
        if(!sound_path1.empty()){
            sound1 = LoadSound(sound_path1.c_str());
        }
        if(!sound_path2.empty()){
            sound2 = LoadSound(sound_path2.c_str());
        }
    }

    virtual ~GameObject()
    {
        if (is_tex_loaded)
            UnloadTexture(texture);
        UnloadSound(sound1);
        UnloadSound(sound2);
    }
    GameObject(const GameObject &) = delete;
    GameObject &operator=(const GameObject &) = delete;

    virtual void Update() = 0;

    void Render() const;

    void ChangeTexture(const std::string &path);
    void ChangeImage(std::string new_path, int new_size_x, int new_size_y, float new_rotation, float new_fade);
    void SetFade(float new_fade) { fade = new_fade; }

    int GetHP() const { return hp; } // new
    void SetHP(int new_hp) { hp = new_hp; }
    int GetX() const { return pos_x; }
    int GetY() const { return pos_y; }
    int GetSizeX() const { return size_x; }
    int GetSizeY() const { return size_y; }
    void SetSize(int new_size_x, int new_size_y)
    {
        size_x = new_size_x;
        size_y = new_size_y;
    }
    void MoveTo(int new_x, int new_y)
    {
        pos_x = new_x;
        pos_y = new_y;
    }

    Sound GetSound1() const { return sound1; }
    Sound GetSound2() const { return sound2; }

    virtual bool isPushable() const { return false; } // new
    virtual bool isWater() const { return false; }
};

class GameText
{
private:
    std::string str;
    int pos_x;
    int pos_y;
    int size;
    Color color;

public:
    GameText(std::string str, int x, int y, int size, Color color) : str(str), pos_x(x), pos_y(y), size(size), color(color) {}

    void Render() const;
};

#endif