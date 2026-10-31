#include <luviolVE/window.hpp>
#include <stdexcept>

int main()
{

try
{
    lvlve::Window window;
    window.run();    
}
catch (const std::exception& e)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "luviolVE", e.what(), nullptr);
    return 1;
}

}