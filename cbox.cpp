#include "cbox.h"
namespace HGS
{
    Box::Box(const SDL_Color& c) : Widget(), bg{c} {}
    Box::Box(const SDL_FRect& g) : Widget(g) {}
    Box::Box(const SDL_FRect& g, const SDL_Color& c) : Widget(g), bg{c} {}

    void Box::render(SDL_Renderer* r)
    {
        SDL_SetRenderDrawColor(r, bg.r, bg.g, bg.b, bg.a);
        const SDL_FRect g {getGeo()};
        SDL_RenderFillRect(r, &g);
    }

    SDL_Color Box::getBG() const
    {
        return bg;
    }

    void Box::setBG(const SDL_Color& c)
    {
        bg = c;
    }

}