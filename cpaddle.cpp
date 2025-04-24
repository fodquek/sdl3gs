#include "cpaddle.h"
namespace HGS
{
Paddle::~Paddle()
{
    SDL_Log("~Paddle\n");
}
void Paddle::step(float dt)
{
    auto p {getPos()};
    p.y += move * dt;
    setPos(p);
}
void Paddle::setMove(float m)
{
    move = m;
}
float Paddle::getMove()
{
    return move;
}
Paddle* PaddleFactory(const SDL_FRect& g, const SDL_Color& bg)
{
    Paddle* paddle {new Paddle};
    paddle->setBG(bg);
    paddle->setPos({g.x, g.y});
    paddle->setWH({g.w, g.h});
    return paddle;
}
} // namespace HGS