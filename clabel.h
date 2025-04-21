#ifndef HGS_LABEL
#define HGS_LABEL
#include <string_view>
#include "cbox.h"
#include <SDL3_ttf/SDL_ttf.h>
namespace HGS
{
class Label : public Box
{
public:
    Label() = default;
    ~Label();

    void render(SDL_Renderer* r) const override;

    std::string_view getText() const;
    void setText(const std::string_view t);

    bool testFont() const;
    void setFont(TTF_Font* f);

    SDL_Color getFC() const;
    void setFC(const SDL_Color& fc);

private:
    TTF_Font* font{nullptr};
    std::string_view text{"FONT"};
    SDL_Color fc{0xff, 0xff, 0x00, 0xff};
};

Widget* LabelFactory(const SDL_FRect& g, const std::string_view text, TTF_Font* font,
                     const SDL_Color& bg, const SDL_Color& fc);
} // namespace HGS
#endif