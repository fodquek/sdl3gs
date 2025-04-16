#include "ccircle.h"
namespace HGS
{
Circle::~Circle()
{
    SDL_Log("~Circle\n");
}
bool Circle::isContains(const float mx, const float my) const
{
    const auto [x, y]{getPos()};
    const float diff_x{x - mx};
    const float diff_y{y - my};
    const float delta{sqrtf((diff_x * diff_x) + (diff_y * diff_y))};
    if (delta > radius) {
        return false;
    }
    return true;
}

// https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
// https://gist.github.com/Gumichan01/332c26f6197a432db91cc4327fcabb1c
void Circle::render(SDL_Renderer* r) const
{
    int offsetx{0};
    int offsety{static_cast<int>(radius)};
    int d{static_cast<int>(radius) - 1};
    int status{0};
    auto [x, y]{getPos()};

    SDL_SetRenderDrawColor(r, bg.r, bg.g, bg.b, bg.a);

    while (offsety >= offsetx) {

        status += SDL_RenderLine(r, x - offsety, y + offsetx, x + offsety, y + offsetx);
        status += SDL_RenderLine(r, x - offsetx, y + offsety, x + offsetx, y + offsety);
        status += SDL_RenderLine(r, x - offsetx, y - offsety, x + offsetx, y - offsety);
        status += SDL_RenderLine(r, x - offsety, y - offsetx, x + offsety, y - offsetx);

        if (status < 0) {
            std::cerr << "CIRCLE RENDER ERR\n";
            return;
        }

        if (d >= 2 * offsetx) {
            d -= 2 * offsetx + 1;
            offsetx += 1;
        } else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        } else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
}

float Circle::getRadius() const
{
    return radius;
}
void Circle::setRadius(const float r)
{
    radius = r;
}
SDL_Color Circle::getBG() const
{
    return bg;
}
void Circle::setBG(const SDL_Color& c)
{
    bg = c;
}

Widget* CircleFactory(Circle* circle, const SDL_FPoint& p, const float r, const SDL_Color& bg)
{
    circle->setPos(p);
    circle->setRadius(r);
    circle->setBG(bg);
    return circle;
}

} // namespace HGS