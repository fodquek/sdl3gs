#ifndef HGS_BOX
#define HGS_BOX
#include "cwidget.h"
namespace HGS
{
class Box : public Widget
{
public:
    Box() = default;
    ~Box();

    bool isContains(float mx, float my) const override;
    void render(SDL_Renderer* r) const override;

    SDL_FPoint getWH() const;
    void setWH(const SDL_FPoint& sizes);

    SDL_Color getBG() const;
    void setBG(const SDL_Color& c);

private:
    SDL_FPoint wh{100.f, 1.f};
    SDL_Color bg{0x00, 0x00, 0x00, 0x00};
};
} // namespace HGS
#endif