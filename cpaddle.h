#ifndef HGS_PADDLE
#define HGS_PADDLE
#include "cbox.h"
namespace HGS
{
class Paddle : public Box
{
public:
    Paddle() = default;
    ~Paddle();
    void step(float dt);
    void setMove(float m);
    float getMove();

private:
    float move{0.f};
};
Paddle* PaddleFactory(const SDL_FRect& g, const SDL_Color& bg);
} // namespace HGS
#endif