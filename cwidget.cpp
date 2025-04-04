#include "cwidget.h"
namespace HGS
{
    Widget::Widget(float x, float y) : target_geometry{x, y, -1.f, -1.f}
    {
    }
    Widget::~Widget()
    {
         destroy();
    }
    Widget::Widget(Widget &w)
    {
        if (*this == w)
        {
            return;
        }
        target_geometry = w.target_geometry;
        texture = w.texture;
        w.texture = nullptr;
    }
    Widget::operator SDL_Texture *() const
    {
        return texture;
    }
    void Widget::setGeo(const SDL_FRect &geo)
    {
        target_geometry = geo;
    }
    SDL_FRect Widget::getGeo()
    {
        return target_geometry;
    }
    bool Widget::LoadFromImage(const std::string_view path, SDL_Renderer *renderer)
    {
        bool fail{false};
        SDL_Surface *loadedSurface{IMG_Load(path.data())};

        if (!loadedSurface)
        {
            SDL_Log("CANT CREATE SDL_Surface: %s\n", SDL_GetError());
            fail = true;
        }
        else
        {
            if (texture = SDL_CreateTextureFromSurface(renderer, loadedSurface); !texture)
            {
                SDL_Log("CANT CRATE SDL_Texture: %s\n", SDL_GetError());
                fail = true;
            }
            target_geometry.w = static_cast<float>(texture->w);
            target_geometry.h = static_cast<float>(texture->h);
            SDL_DestroySurface(loadedSurface);
        }
        return fail;
    }
    bool Widget::loadFromTTF(const std::string_view path,
                             std::string_view text,
                             SDL_Color text_color,
                             float ptsize,
                             SDL_Renderer *renderer)
    {
        destroy();
        bool fail{false};

        TTF_Font *font{TTF_OpenFont(path.data(), ptsize)};
        if (!font)
        {
            SDL_Log("CANT CREATE FONT %s\n", SDL_GetError());
            fail = true;
        }
        else
        {
            SDL_Surface *textSurface{TTF_RenderText_Solid(font, text.data(), 0, text_color)};
            if (!textSurface)
            {
                SDL_Log("CANT CREATE SDL_Surface* textSurface: %s\n", SDL_GetError());
                fail = true;
            }
            else
            {
                if (texture = SDL_CreateTextureFromSurface(renderer, textSurface); !texture)
                {
                    SDL_Log("CANT CRATE SDL_Texture: %s\n", SDL_GetError());
                    fail = true;
                }
                target_geometry.w = static_cast<float>(texture->w);
                target_geometry.h = static_cast<float>(texture->h);
                SDL_DestroySurface(textSurface);
            }
            TTF_CloseFont(font);
            font = nullptr;
        }
        return fail;
    }
    bool Widget::isPointIn(float mouse_x, float mouse_y)
    {
        if ((mouse_x >= getGeo().x) && (mouse_x <= (getGeo().x + getGeo().w)))
        {
            if ((mouse_y >= getGeo().y) && (mouse_y <= (getGeo().y + getGeo().h)))
            {
                return true;
            }
        }

        return false;
    }
    void Widget::destroy()
    {
        if (texture)
        {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
    }
}