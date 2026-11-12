#include <luviolVE/editorController.hpp>

namespace lvlve
{

EditorController::EditorController(Player& player, FileDialog& fd)
    : player(player),
      fd(fd)     
{}

void EditorController::update()
{
    pollOpenFile();
}

void EditorController::togglePlay()
{
    player.togglePlay();
}

void EditorController::openFile()
{
    fd.openFile();
}

void EditorController::pollOpenFile()
{   
    std::string sf = fd.takeSelectedFile();
    if (!sf.empty())
        player.open(sf);
}
    
}