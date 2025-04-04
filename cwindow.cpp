#include "cwindow.h"

namespace HGS
{
    Window::Window(std::string_view title, SDL_Point wh) : sdl_window{SDL_CreateWindow(title.data(), wh.x, wh.y, 0)}
    {
        if (!sdl_window)
        {
            SDL_Log("CANT CREATE ©Window : %s\n", SDL_GetError());
        }
    }
    Window::~Window()
    {
        if (sdl_window)
        {
            SDL_DestroyWindow(sdl_window);
        }
    }
    Window::operator SDL_Window *()
    {
        return sdl_window;
    }
}