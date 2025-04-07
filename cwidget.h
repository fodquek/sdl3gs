#ifndef HGS_Widget
#define HGS_Widget
#include <string_view>
#include <SDL3/SDL.h>

namespace HGS
{
    class Widget
    {
    public:
        Widget() = default;
        Widget(const SDL_FRect &g);
        virtual ~Widget() = default;
        bool operator==(const Widget &w);
        bool operator!=(const Widget &w);
        virtual bool isContains(float mouse_x, float mouse_y);

        virtual void render(SDL_Renderer* r) = 0;

        SDL_FRect getGeo () const;
        void setGeo(const SDL_FRect &geo);

        SDL_FPoint getPos() const;
        void setPos(const SDL_FPoint &pos);

    private:
        SDL_FRect geometry{};
    };
}

namespace std 
{
    std::ostream &operator<<(std::ostream &os, const HGS::Widget &w);
}

#endif