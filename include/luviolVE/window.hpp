#pragma once

#include <SDL3/SDL_video.h>

namespace lvlve
{

class Window
{
public:
    Window();
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    SDL_Window* getWindow() const { return window; }
    SDL_GLContext getGLContext() const { return glContext; }
private:
    SDL_Window* window = nullptr;
    SDL_GLContext glContext = nullptr;
};

}
