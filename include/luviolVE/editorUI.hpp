#pragma once

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <SDL3/SDL.h>
#include <glad/gl.h>

namespace lvlve
{

class EditorUI
{
public:
    EditorUI(SDL_Window* window, SDL_GLContext glContext);
    ~EditorUI();

    void render(GLuint texture, int videoW, int videoH);
private:
    float mainScale;
    void renderPreview(GLuint texture, int videoW, int videoH);
};

}