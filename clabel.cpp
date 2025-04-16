#include "clabel.h"
namespace HGS
{
Label::~Label()
{
    clearAllocation();
    SDL_Log("~Label\n");
}

void Label::render(SDL_Renderer* r) const
{
    Box::render(r);
    SDL_Surface* surface{TTF_RenderText_Solid(font, text.data(), 0, color)};
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

bool Label::makeFont(const std::string_view file)
{
    clearAllocation();
    font = TTF_OpenFont(file.data(), ptsize);
    if (!font) {
        SDL_Log("CANT CRATE Label@Label::makeLabel()\n");
        return false;
    }
    return true;
}

std::string_view Label::getText() const
{
    return text;
}

void Label::setText(const std::string_view t)
{
    text = t;
}

float Label::getPtSize() const
{
    return ptsize;
}

void Label::setPtSize(const float pt)
{
    ptsize = pt;
}

SDL_Color Label::getColor() const
{
    return color;
}

void Label::setColor(const SDL_Color& c)
{
    color = c;
}

bool Label::clearAllocation()
{
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
        return true;
    }
    return false;
}

Widget* LabelFactory(Label* Label, const SDL_FRect& g, const std::string_view text,
                    const std::string_view file, const float ptsize, const SDL_Color& bg,
                    const SDL_Color& c)
{
    Label->setPos({g.x, g.y});
    Label->setWH({g.w, g.h});
    Label->setText(text);
    Label->setPtSize(ptsize);
    Label->setBG(bg);
    Label->setColor(c);
    Label->makeFont(file);
    return Label;
}

} // namespace HGS