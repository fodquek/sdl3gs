#ifndef HGS_Widget
#define HGS_Widget
#include <string_view>
#include <SDL3/SDL.h>

namespace HGS
{
    class Widget
    {
    public:
        Widget();
        Widget(float x, float y, float w, float h);
        Widget(const SDL_FRect &g);
        Widget(const Widget &w);
        Widget(const Widget *w);
        Widget &operator=(const Widget &w);
        bool operator==(const Widget &w);
        bool operator!=(const Widget &w);
        virtual ~Widget();

        SDL_FRect getGeo () const;
        void setGeo(const SDL_FRect &geo);
        virtual bool contains(float mouse_x, float mouse_y) = 0;

    private:
        SDL_FRect geometry{};
    };
}

namespace std 
{
    std::ostream &operator<<(std::ostream &os, const HGS::Widget &w);
}

#endif