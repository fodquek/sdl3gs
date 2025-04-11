#include "cfont.h"
namespace HGS
{
Font::~Font()
{
    clearAllocation();
    SDL_Log("~Font\n");
}

void Font::render(SDL_Renderer* r) const
{
    Box::render(r);
    SDL_Surface* surface{TTF_RenderText_Solid(font, text.data(), 0, color)};
    if (!surface) {
        SDL_Log("CANT CREATE SURFACE@FONT::RENDER\n");
    }
    SDL_Texture* texture{SDL_CreateTextureFromSurface(r, surface)};
    if (!texture) {
        SDL_Log("CANT CREATE TEXTURE@FONT::RENDER\n");
    }
    SDL_DestroySurface(surface);
    const auto p {getPos()};
    const auto sizes {getWH()};
    const SDL_FRect dst{p.x, p.y, sizes.x, sizes.y};
    SDL_RenderTexture(r, texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
}

bool Font::makeFont(const std::string_view file) 
{
    clearAllocation();
    font = TTF_OpenFont(file.data(), ptsize);
    if (!font) {
        SDL_Log("CANT CRATE FONT@Font::makeFont()\n");
        return false;
    }
    return true;
}

std::string_view Font::getText() const
{
    return text;
}

void Font::setText(const std::string_view t)
{
    text = t;
}

float Font::getPtSize() const
{
    return ptsize;
}

void Font::setPtSize(const float pt)
{
    ptsize = pt;
}

SDL_Color Font::getColor() const
{
    return color;
}

void Font::setColor(const SDL_Color& c)
{
    color = c;
}

bool Font::clearAllocation()
{
    if (font)
    {
        TTF_CloseFont(font);
        font = nullptr;
        return true;
    }
    return false;
}

} // namespace HGS