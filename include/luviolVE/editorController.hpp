#pragma once

#include <luviolVE/player.hpp>
#include <luviolVE/fileDialog.hpp>

namespace lvlve
{

class EditorController
{
public:
    EditorController(Player& player, FileDialog& fd);

    void update();
    void togglePlay();
    void openFile();
    
private:
    Player& player;
    FileDialog& fd;
    void pollOpenFile();    
};    

}
