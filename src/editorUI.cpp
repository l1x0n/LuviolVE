#include <luviolVE/editorUI.hpp>
#include <imgui_impl_opengl3.h>

namespace lvlve
{

EditorUI::EditorUI(SDL_Window* window, SDL_GLContext glContext, const Player& player, EditorController& controller)
    : player(player),
      controller(controller)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    float mainScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    style.ScaleAllSizes(mainScale);
    style.FontScaleDpi = mainScale;

    ImGui_ImplSDL3_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 460");
}

EditorUI::~EditorUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void EditorUI::update()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    
    mainMenuBar();
    preview();
}

void EditorUI::draw()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorUI::mainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open file"))
            {
                controller.openFile();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar(); 
    }
    
}

void EditorUI::preview()
{
    auto s = player.getState();

    ImGui::Begin("Preview", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
    if (s.texture != 0 && s.videoW != 0 && s.videoH != 0)    
    {
        ImVec2 avail = ImGui::GetContentRegionAvail();
        float videoAspect = static_cast<float>(s.videoW) / s.videoH;
        float availAspect = avail.x / avail.y;

        ImVec2 imageSize;
        if (availAspect > videoAspect)
        {
            imageSize.y = avail.y;
            imageSize.x = avail.y * videoAspect;
        }
        else
        {
            imageSize.x = avail.x;
            imageSize.y = avail.x / videoAspect;
        }

        float offsetX = (avail.x - imageSize.x) * 0.5f;
        float offsetY = (avail.y - imageSize.y) * 0.5f;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offsetY);

        ImGui::Image(static_cast<ImTextureID>(s.texture), imageSize);
    }
        
    if (ImGui::Button(s.playing ? "Stop" : "Start"))
    {
        controller.togglePlay();
    }

    ImGui::End();
}

}
