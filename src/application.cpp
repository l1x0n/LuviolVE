#include <luviolVE/application.hpp>

namespace lvlve
{

Application::Application() 
    : window(), player(),  
      ui(window.getWindow(), window.getGLContext(), player)
      
{}

void Application::run()
{
    bool running = true;
    SDL_Event event;

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
                SDL_GetWindowSizeInPixels(window.getWindow(), &width, &height);
                glViewport(0, 0, width, height);
            }
        }

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        player.update();
        ui.update();
        ui.draw();
        SDL_GL_SwapWindow(window.getWindow());
    }
}

}
