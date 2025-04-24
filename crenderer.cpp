#include "crenderer.h"

namespace HGS
{
Renderer::Renderer(HGS::Window* w) : sdl_renderer{SDL_CreateRenderer(*w, nullptr)}
{
    if (!sdl_renderer) {
        SDL_Log("CANT CREATE ©Renderer : %s\n", SDL_GetError());
    }
}
Renderer::~Renderer()
{
    if (sdl_renderer) {
        SDL_DestroyRenderer(sdl_renderer);
    }
}
Renderer::operator SDL_Renderer*() const
{
    return sdl_renderer;
}
} // namespace HGS