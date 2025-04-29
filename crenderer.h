#ifndef HGS_RENDERER
#define HGS_RENDERER
#include "cwindow.h"
namespace HGS
{
class Renderer
{
public:
    Renderer(Window* w);
    ~Renderer();
    Renderer(const Renderer& r) = delete;
    Renderer operator=(const Renderer& r) = delete;
    operator SDL_Renderer*() const;

private:
    SDL_Renderer* sdl_renderer;
};
} // namespace HGS
#endif