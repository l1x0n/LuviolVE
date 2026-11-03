#include <luviolVE/application.hpp>
#include <stdexcept>

int main()
{

try
{
    lvlve::Application app;
    app.run();    
}
catch (const std::exception& e)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "luviolVE", e.what(), nullptr);
    return 1;
}

}