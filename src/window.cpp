#include <stdexcept>
#include <luviolVE/window.hpp>
#include <luviolVE/video.hpp>
#include <luviolVE/videoRenderer.hpp>

namespace lvlve 
{

Window::Window() 
{   
    if (!SDL_Init(SDL_INIT_VIDEO))
        throw std::runtime_error(SDL_GetError());

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    int width {1280}, height {720};

    window = SDL_CreateWindow("luviolVE", width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

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

    Video video("./video.mp4");
    VideoRenderer renderer(video.getVideoWidth(), video.getVideoHeight());
    AVRational timeBase = video.getTimeBase();
    AVFrame* currentFrame = nullptr;
    double start = static_cast<double>(SDL_GetTicksNS()) / 1e9;

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

        if (!currentFrame)
            currentFrame = video.getNextFrame();
        if (currentFrame)
        {
            double elapsed = (static_cast<double>(SDL_GetTicksNS()) / 1e9) - start;
            if(currentFrame->pts * av_q2d(timeBase) <= elapsed)
            {
                renderer.render(currentFrame);
                currentFrame = nullptr;
            }
        }
        
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        editorUI->render(renderer.getOutputTexture(), video.getVideoWidth(), video.getVideoHeight());
        SDL_GL_SwapWindow(window);
    }
}

}