#pragma once

#include <luviolVE/window.hpp>
#include <luviolVE/editorUI.hpp>

namespace lvlve
{

class Application
{
public:
    Application();
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void run();
private:
    Window window;
    Player player;
    FileDialog fd;
    EditorController controller;
    EditorUI ui;       
};

}