#ifndef HGS_WINDOW
#define HGS_WINDOW
#include <SDL3/SDL.h>
#include <string_view>
namespace HGS
{
class Window
{
public:
    Window(std::string_view title, SDL_Point wh);
    ~Window();
    Window(const Window& w) = delete;
    Window operator=(const Window& w) = delete;
    operator SDL_Window*() const;

private:
    SDL_Window* sdl_window;
};
} // namespace HGS
#endif