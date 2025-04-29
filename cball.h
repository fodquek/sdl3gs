#ifndef HGS_BALL
#define HGS_BALL
#include "ccircle.h"
namespace HGS
{
struct Velocity
{
    float x;
    float y;
};
class Ball : public Circle
{
public:
    Ball() = default;
    ~Ball();
    void setVel(const Velocity& v);
    Velocity getVel() const;
    void step(float dt);

private:
    Velocity vel{0.f, 0.f};
};
Widget* BallFactory(const SDL_FPoint& p, const float r, const SDL_Color& bg, const Velocity& v);
} // namespace HGS
#endif