#pragma once

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <SDL3/SDL.h>

namespace lvlve
{

class EditorUI
{
public:
    EditorUI(SDL_Window* window, SDL_GLContext glContext);
    ~EditorUI();

    void render();
private:
    float mainScale;
    void renderPreview();
};

}