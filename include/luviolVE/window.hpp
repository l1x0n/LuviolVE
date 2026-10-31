#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <luviolVE/editorUI.hpp>

namespace lvlve
{

class Window
{
public:
    Window();
    ~Window();

    void run();
private:
    SDL_Window* window = nullptr;
    SDL_GLContext glContext = nullptr;
    std::unique_ptr<EditorUI> editorUI;
};

}
