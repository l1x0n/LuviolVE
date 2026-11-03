#pragma once

#include <luviolVE/player.hpp>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <SDL3/SDL.h>

namespace lvlve
{

class EditorUI
{
public:
    EditorUI(SDL_Window* window, SDL_GLContext glContext, const Player& player);
    ~EditorUI();

    void update();
    void draw();
private:
    float mainScale;
    const Player& player;
    void preview();
};

}