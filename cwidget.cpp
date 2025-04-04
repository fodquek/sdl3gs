#include "cwidget.h"
#include <iostream>

namespace std{
    std::ostream &operator<<(std::ostream &os, const HGS::Widget &w)
    {
        const auto& fr {w.getGeo()};
        return os << "[x: " << fr.x
                << ", y: " << fr.y
                << ", w: " << fr.w
                << ", h: " << fr.h
                << "]\n";
    }
}


namespace HGS
{
    Widget::Widget() : geometry{} 
    {
        std::cout << "0\n";
    }
    Widget::Widget(float x, float y, float w, float h) : geometry{x, y, w, h}
    {
        std::cout << "1\n";
    }

    Widget::Widget(const SDL_FRect &g) : geometry{g}
    {
        std::cout << "2\n";
    }
    Widget::Widget(const Widget &w)
    {
        std::cout << "3\n";
        if (this == &w)
        {
            return;
        }
        geometry = w.geometry;
    }
    Widget::Widget(const Widget *w)
    {
        std::cout << "4\n";
        if (this != w)
        {
            geometry = w->geometry;
        }
    }
    Widget &Widget::operator=(const Widget &w)
    {
        std::cout << "5\n";
        if (this != &w)
        {
            geometry = w.geometry;
        }
        return *this;
    }
    bool Widget::operator==(const Widget &w)
    {
        return this == &w;
    }
    bool Widget::operator!=(const Widget &w)
    {
        return !(this->operator==(w));
    }
    Widget::~Widget()
    {
        std::cout << "widget ded\n";
    }
    void Widget::setGeo(const SDL_FRect &g)
    {
        geometry = g;
    }
    SDL_FRect Widget::getGeo () const
    {
        return geometry;
    }
    bool Widget::contains(float mouse_x, float mouse_y)
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
}