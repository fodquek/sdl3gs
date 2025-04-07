#ifndef HGS_BOX
#define HGS_BOX
#include "cwidget.h"
namespace HGS
{
    class Box : public Widget
    {
    public:
        Box() = default;
        explicit Box(const float x, const float y, const float w, const float h);
        explicit Box(const SDL_FRect& g);
        explicit Box(const SDL_FRect& g, const SDL_Color& c);
        ~Box() = default;

        bool isContains(float mx, float my) const override;
        void render(SDL_Renderer* r) const override;

        SDL_FPoint getWH() const;
        void setWH(const float w, const float h);
        void setWH(const SDL_FPoint& wh);

        SDL_Color getBG() const;
        void setBG(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a);
        void setBG(const SDL_Color& c);

    private:
        SDL_FPoint wh{10.f, 10.f};
        SDL_Color bg{0xff, 0x00, 0xff, 0xff};
    };
}
#endif