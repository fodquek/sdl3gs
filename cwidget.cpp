#include "cwidget.h"
#include <iostream>

namespace HGS
{
bool Widget::operator==(const Widget& w) const
{
    return this == &w;
}
bool Widget::operator!=(const Widget& w) const
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
void Widget::setPos(const SDL_FPoint& p)
{
    pos = p;
}

} // namespace HGS