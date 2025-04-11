#ifndef HGS_CIRCLE
#define HGS_CIRCLE
#include "cwidget.h"
#include <iostream>

namespace HGS
{
class Circle : public Widget
{
public:
    Circle() = default;
    ~Circle() = default;

    bool isContains(float mx, float my) const override;
    void render(SDL_Renderer* r) const override;

    float getRadius() const;
    void setRadius(const float r);

    SDL_Color getBG() const;
    void setBG(const SDL_Color& c);

private:
    float radius{10.f};
    SDL_Color bg{0xff, 0xff, 0x00, 0xff};
};
} // namespace HGS
#endif