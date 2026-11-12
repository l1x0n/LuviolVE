#include <luviolVE/fileDialog.hpp>

namespace lvlve
{

FileDialog::FileDialog(SDL_Window* window)
    : window(window)
{}

FileDialog::~FileDialog()
{
    window = nullptr;
}

void FileDialog::openFile()
{   
    const SDL_DialogFileFilter filters[] 
    {
        {"All media files", "mp4;mov;mkv;m4v;webm;avi;divx;xvid;ogg;mp3;wav;aac;m4a;flac;opus"},
        {"Video files", "mp4;mov;mkv;m4v;webm;avi;divx;xvid;ogg"},
        {"Audio files", "mp3;wav;aac;m4a;flac;ogg;opus"},
        {"All files", "*"}
    };

    SDL_ShowOpenFileDialog(&FileDialog::callback, this, window, filters, SDL_arraysize(filters), nullptr, false);
}

void SDLCALL FileDialog::callback(void* userdata, const char* const* filelist, [[maybe_unused]] int filter)
{
    if (!filelist)
        return;
    if (!*filelist)
        return;
        
    auto* self = static_cast<FileDialog*>(userdata);
    self->selectedFile = filelist[0];
}
    
}