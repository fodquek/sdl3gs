#include "ccircle.h"
namespace HGS
{
    Circle::Circle(const float x, const float y, const float r) : Widget(x, y), radius(r) {}
    Circle::Circle(const float x, const float y, const float r, const SDL_Color& c) : Widget(x, y), radius(r), bg(c) {}
    
    bool Circle::isContains(const float mx, const float my) const
    {
        const auto [x, y] {getPos()};
        const float diff_x {x - mx};
        const float diff_y {y - my};
        const float delta {sqrtf((diff_x * diff_x) + (diff_y * diff_y))};
        if (delta > radius)
        {
            return false;
        } 
        return true;
    }

    // https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
    void Circle::render(SDL_Renderer* r) const
    {
        // TODO
    }

    float Circle::getRadius() const
    {
        return radius;
    }
    void Circle::setRadius(const float r)
    {
        radius = r;
    }
}