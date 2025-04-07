#include "cwidget.h"
#include <iostream>

namespace HGS
{
    Widget::Widget(const float x, const float y) : pos{x, y} {}
    Widget::Widget(const SDL_FPoint &p) : pos{p} {}
    bool Widget::operator==(const Widget &w)
    {
        return this == &w;
    }
    bool Widget::operator!=(const Widget &w)
    {
        return !(this->operator==(w));
    }
    SDL_FPoint Widget::getPos() const
    {
        return pos;
    }
    void Widget::setPos(const float x, const float y)
    {
        pos = {x, y};
    }
    void Widget::setPos(const SDL_FPoint &p)
    {
        pos = p;
    }
    

}