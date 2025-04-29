#include "cball.h"
namespace HGS
{
Ball::~Ball()
{
    SDL_Log("~Ball");
}
void Ball::setVel(const Velocity& v)
{
    vel = v;
}
Velocity Ball::getVel() const
{
    return vel;
}
void Ball::step(float dt)
{
    auto p {getPos()};
    p.x += vel.x * dt;
    p.y += vel.y * dt;
    setPos(p);
}
Widget* BallFactory(const SDL_FPoint& p, const float r, const SDL_Color& bg, const Velocity& v)
{
    Ball* ball{new Ball};
    ball->setPos(p);
    ball->setRadius(r);
    ball->setBG(bg);
    ball->setVel(v);
    return ball;
}
} // namespace HGS