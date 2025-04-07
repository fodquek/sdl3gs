#ifndef HGS_WIDGET
#define HGS_WIDGET
#include <SDL3/SDL.h>
#include <string_view>
namespace HGS
{
class Widget
{
public:
    Widget() = default;
    explicit Widget(const float x, const float y);
    explicit Widget(const SDL_FPoint& p);
    virtual ~Widget() = default;
    bool operator==(const Widget& w) const;
    bool operator!=(const Widget& w) const;

    virtual bool isContains(float mx, float my) const = 0;
    virtual void render(SDL_Renderer* r) const = 0;

    SDL_FPoint getPos() const;
    void setPos(const float x, const float y);
    void setPos(const SDL_FPoint& pos);

private:
    SDL_FPoint pos{10.f, 10.f};
};
} // namespace HGS

namespace std
{
std::ostream& operator<<(std::ostream& os, const HGS::Widget& w);
}

#endif