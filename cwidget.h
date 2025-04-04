#ifndef HGS_Widget
#define HGS_Widget
#include <string_view>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
namespace HGS
{
    class Widget
    {
    public:
        Widget(float x, float y);
        ~Widget();
        Widget(Widget &w);
        Widget operator=(const Widget &w) = delete;
        operator SDL_Texture *() const;
        void setGeo(const SDL_FRect &geo);
        SDL_FRect getGeo();
        bool LoadFromImage(const std::string_view path, SDL_Renderer *renderer);

        bool loadFromTTF(const std::string_view path,
                         std::string_view text,
                         SDL_Color text_color,
                         float ptsize,
                         SDL_Renderer *renderer);

        bool isPointIn(float mouse_x, float mouse_y);
        void destroy();

    private:
        SDL_FRect target_geometry{};
        SDL_Texture *texture{nullptr};
    };
}
#endif