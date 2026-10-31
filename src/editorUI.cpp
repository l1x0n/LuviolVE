#include <imgui_impl_opengl3.h>
#include <luviolVE/editorUI.hpp>

namespace lvlve
{

EditorUI::EditorUI(SDL_Window* window, SDL_GLContext glContext)
{
    mainScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
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

void EditorUI::render(GLuint texture, int videoW, int videoH)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    
    renderPreview(texture, videoW, videoH);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorUI::renderPreview(GLuint texture, int videoW, int videoH)
{
    ImGui::Begin("Preview", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
    ImVec2 avail = ImGui::GetContentRegionAvail();
    float videoAspect = (float)videoW / (float)videoH;
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

    ImGui::Image((ImTextureID)(intptr_t)texture, imageSize);
    ImGui::End();
}

}