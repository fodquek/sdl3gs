#ifndef HGS_FONT
#define HGS_FONT
#include <string_view>
#include <SDL3_ttf/SDL_ttf.h>
namespace HGS
{
    class Font
    {
        public:
        Font(std::string_view file);
        ~Font();
        Font(const Font& f) = delete;
        operator TTF_Font*();
        TTF_Font* getFont() const;
        bool clear();
        private:
        TTF_Font* font {nullptr};
    };
}
#endif