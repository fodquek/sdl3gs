#ifndef HGS_FONT
#define HGS_FONT
#include <string_view>
#include "cbox.h"
#include <SDL3_ttf/SDL_ttf.h>
namespace HGS
{
class Font : public Box
{
    public:
        Font() = default;
        ~Font();

        void render(SDL_Renderer* r) const override;
        
        bool makeFont(const std::string_view file);

        std::string_view getText() const;
        void setText(const std::string_view t);

        float getPtSize() const;
        void setPtSize(const float pt);

        SDL_Color getColor() const;
        void setColor(const SDL_Color& c);

        bool clearAllocation();

    private:
        TTF_Font* font{nullptr};
        std::string_view text{"FONT"};
        float ptsize{192.f};
        SDL_Color color{0xff, 0xff, 0x00, 0xff};
};
} // namespace HGS
#endif