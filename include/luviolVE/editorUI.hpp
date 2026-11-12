#pragma once

#include <luviolVE/editorController.hpp>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <SDL3/SDL_video.h>

namespace lvlve
{

class EditorUI
{
public:
    EditorUI(SDL_Window* window, SDL_GLContext glContext, const Player& player, EditorController& controller);
    ~EditorUI();

    void update();
    void draw();
private:
    const Player& player;
    EditorController& controller;
    void mainMenuBar();
    void preview();    
};

}