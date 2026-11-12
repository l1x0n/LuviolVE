#pragma once

#include <string>
#include <utility>
#include <SDL3/SDL_dialog.h>

namespace lvlve
{

class FileDialog
{
public:
    FileDialog(SDL_Window* window);
    ~FileDialog();

    void openFile();
    std::string takeSelectedFile() 
    {
        std::string sf = std::move(selectedFile);
        selectedFile.clear();
        return sf;
    }
private:
    SDL_Window* window = nullptr;
    std::string selectedFile;
    static void SDLCALL callback(void* userdata, const char* const* filelist, int filter);
};

}