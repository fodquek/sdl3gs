#include "cbox.h"
namespace HGS
{
    Box::Box(const float x, const float y, const float w, const float h) : Widget{x, y}, wh{w, h} {}
    Box::Box(const SDL_FRect& g) : Widget(g.x, g.y), wh {g.w, g.h} {}
    Box::Box(const SDL_FRect& g, const SDL_Color& c) : Widget(g.x, g.y), wh {g.w, g.h}, bg{c} {}

    bool Box::isContains(float mx, float my) const
    {
        const auto [x, y] {getPos()};
        const auto [w, h] {getWH()};
        if ((mx >= x) && (mx <= (x + w)))
        {
            if ((my >= y) && (my <= (y + h)))
            {
                return true;
            }
        }

        return false;
    }

    void Box::render(SDL_Renderer* r) const
    {
        const auto [x, y] {getPos()};
        const SDL_FRect g {x, y,  wh.x, wh.y};
        SDL_SetRenderDrawColor(r, bg.r, bg.g, bg.b, bg.a);
        SDL_RenderFillRect(r, &g);
    }

    SDL_FPoint Box::getWH() const
    {
        return wh;
    }

    void Box::setWH(const float w, const float h)
    {
        wh = {w, h};
    }

    void Box::setWH(const SDL_FPoint& wh)
    {
        this->wh = wh;
    }

    SDL_Color Box::getBG() const
    {
        return bg;
    }

    void Box::setBG(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a)
    {
        bg = {r, g, b, a};
    }

    void Box::setBG(const SDL_Color& c)
    {
        bg = c;
    }

}


