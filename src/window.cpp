#include <iostream>
#include <stdexcept>
#include <luviolVE/window.hpp>

namespace lvlve 
{

Window::Window() 
{   
    if (!SDL_Init(SDL_INIT_VIDEO))
        throw std::runtime_error(SDL_GetError());

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("luviolVE", 720, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        SDL_Quit();
        throw std::runtime_error(SDL_GetError());
    }

    glContext = SDL_GL_CreateContext(window);

    if (!glContext)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error(SDL_GetError());
    }

    SDL_GL_MakeCurrent(window, glContext);

    if(!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress))
    {
        SDL_GL_DestroyContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("Falied to init glad\n");
    }

    editorUI = std::make_unique<EditorUI>(window, glContext);

    SDL_GL_SetSwapInterval(1);
}

Window::~Window()
{
    if (editorUI)
        editorUI.reset();
    if (glContext)
        SDL_GL_DestroyContext(glContext);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::run() 
{
    bool running = true;
    SDL_Event event;
    glClearColor(0.05, 0.05, 0.05, 1.0);

    while (running)
    {   
        while (SDL_PollEvent(&event)) 
        {
            ImGui_ImplSDL3_ProcessEvent(&event);

            if (event.type == SDL_EVENT_QUIT)
                running = false;

            if (event.type == SDL_EVENT_WINDOW_RESIZED)
            {
                int width, height;
                SDL_GetWindowSizeInPixels(window, &width, &height);
                glViewport(0, 0, width, height);
            }
        }
        
        glClear(GL_COLOR_BUFFER_BIT);
        editorUI->render();
        SDL_GL_SwapWindow(window);
    }
}

}