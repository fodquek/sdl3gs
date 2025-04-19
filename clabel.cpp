#include "clabel.h"
namespace HGS
{
Label::~Label()
{
    SDL_Log("~Label\n");
}

void Label::render(SDL_Renderer* r) const
{
    Box::render(r);
    SDL_Surface* surface{TTF_RenderText_Solid(font, text.data(), 0, fc)};
    if (!surface) {
        SDL_Log("CANT CREATE SURFACE@Label::RENDER\n");
    }
    SDL_Texture* texture{SDL_CreateTextureFromSurface(r, surface)};
    if (!texture) {
        SDL_Log("CANT CREATE TEXTURE@Label::RENDER\n");
    }
    SDL_DestroySurface(surface);
    const auto p{getPos()};
    const auto sizes{getWH()};
    const SDL_FRect dst{p.x, p.y, sizes.x, sizes.y};
    SDL_RenderTexture(r, texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
}

std::string_view Label::getText() const
{
    return text;
}

void Label::setText(const std::string_view t)
{
    text = t;
}

bool Label::testFont() const
{
    return font;
}

void Label::setFont(TTF_Font* f) 
{
    font = f;
}

SDL_Color Label::getFC() const
{
    return fc;
}

void Label::setFC(const SDL_Color& c)
{
    fc = c;
}

Widget* LabelFactory(Label* label, const SDL_FRect& g, const std::string_view text, TTF_Font* font,
    const SDL_Color& bg, const SDL_Color& fc)
{
    label->setPos({g.x, g.y});
    label->setWH({g.w, g.h});
    label->setText(text);
    label->setFont(font);
    label->setBG(bg);
    label->setFC(fc);
    return label;
}

} // namespace HGS