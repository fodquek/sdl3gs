#include "cbox.h"
namespace HGS
{
Box::~Box()
{
    SDL_Log("~BOX\n");
}
bool Box::isContains(float mx, float my) const
{
    const auto [x, y]{getPos()};
    const auto [w, h]{getWH()};
    if ((mx >= x) && (mx <= (x + w))) {
        if ((my >= y) && (my <= (y + h))) {
            return true;
        }
    }

    return false;
}

void Box::render(SDL_Renderer* r) const
{
    const auto [x, y]{getPos()};
    const auto [w, h]{getWH()};
    const SDL_FRect g{x, y, w, h};
    SDL_SetRenderDrawColor(r, bg.r, bg.g, bg.b, bg.a);
    SDL_RenderFillRect(r, &g);
}

SDL_FPoint Box::getWH() const
{
    return wh;
}

void Box::setWH(const SDL_FPoint& sizes)
{
    wh.x = sizes.x;
    wh.y = sizes.y;
}

SDL_Color Box::getBG() const
{
    return bg;
}

void Box::setBG(const SDL_Color& c)
{
    bg = c;
}

Widget* BoxFactory(const SDL_FRect& g, const SDL_Color& bg)
{
    Box* box {new Box};
    box->setBG(bg);
    box->setPos({g.x, g.y});
    box->setWH({g.w, g.h});
    return box;
}

} // namespace HGS
