#include "cwidget.h"
#include <iostream>

namespace std
{
    std::ostream &operator<<(std::ostream &os, const HGS::Widget &w)
    {
        const auto &fr{w.getGeo()};
        return os << "[x: " << fr.x
                  << ", y: " << fr.y
                  << ", w: " << fr.w
                  << ", h: " << fr.h
                  << "]\n";
    }
}

namespace HGS
{
    Widget::Widget(const SDL_FRect &g) : geometry{g} {}
    bool Widget::operator==(const Widget &w)
    {
        return this == &w;
    }
    bool Widget::operator!=(const Widget &w)
    {
        return !(this->operator==(w));
    }
    bool Widget::isContains(float mouse_x, float mouse_y)
    {
        if ((mouse_x >= getGeo().x) && (mouse_x <= (getGeo().x + getGeo().w)))
        {
            if ((mouse_y >= getGeo().y) && (mouse_y <= (getGeo().y + getGeo().h)))
            {
                return true;
            }
        }

        return false;
    }
    SDL_FRect Widget::getGeo() const
    {
        return geometry;
    }
    void Widget::setGeo(const SDL_FRect &g)
    {
        geometry = g;
    }
    SDL_FPoint Widget::getPos() const
    {
        return {getGeo().x, getGeo().y};
    }
    void Widget::setPos(const SDL_FPoint &p)
    {
        auto g {getGeo()};
        g.x = p.x - (g.w / 2.f);
        g.y = p.y - (g.h / 2.f);
        setGeo(g);
    }
    

}