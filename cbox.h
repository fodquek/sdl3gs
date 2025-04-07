#ifndef HGS_BOX
#define HGS_BOX
#include "cwidget.h"
namespace HGS
{
    class Box : public Widget
    {
    public:
        Box() = default;
        Box(const SDL_Color& c);
        Box(const SDL_FRect& g);
        Box(const SDL_FRect& g, const SDL_Color& c);
        ~Box() = default;

        void render(SDL_Renderer* r) override;

        SDL_Color getBG()const;
        void setBG(const SDL_Color& c);

    private:
        SDL_Color bg{0xff, 0x00, 0xff, 0xff};
    };
}
#endif