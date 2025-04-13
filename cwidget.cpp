#include "cwidget.h"
#include <iostream>

namespace HGS
{
Widget::~Widget()
{
    SDL_Log("~WIDGET\n\n");
}
bool Widget::operator==(const Widget& w) const
{
    return this == &w;
}
bool Widget::operator!=(const Widget& w) const
{
    return !(this->operator==(w));
}
void Widget::setCallBack(std::function<void()> cb)
{
    callback = cb;
}
void Widget::call2back()
{
    callback();
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