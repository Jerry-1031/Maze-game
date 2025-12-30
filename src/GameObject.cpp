#include "../include/GameObject.hpp"

void GameObject::Render() const
{
    if (!is_tex_loaded)
        return;

    Vector2 origin = {static_cast<float>(size_x) / 2.0f, static_cast<float>(size_y) / 2.0f};
    Rectangle dest = {
        static_cast<float>(pos_x) + origin.x,
        static_cast<float>(pos_y) + origin.y,
        static_cast<float>(size_x),
        static_cast<float>(size_y)};
    Rectangle source = {
        0.0f,
        0.0f,
        static_cast<float>(texture.width),
        static_cast<float>(texture.height)};

    DrawTexturePro(
        texture,
        source,
        dest,
        origin,
        rotation,
        Fade(WHITE, fade));
}

void GameObject::ChangeTexture(const std::string &path)
{
    if (is_tex_loaded)
        UnloadTexture(texture);
    texture = LoadTexture(path.c_str());
    is_tex_loaded = (texture.id != 0);
}

void GameObject::ChangeImage(std::string new_path, int new_size_x, int new_size_y, float new_rotation, float new_fade)
{
    size_x = new_size_x;
    size_y = new_size_y;
    rotation = new_rotation;
    fade = new_fade;
    ChangeTexture(new_path);
}

void GameText::Render() const
{
    DrawText(str.c_str(), pos_x, pos_y, size, color);
}