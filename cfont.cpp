#include "cfont.h"
namespace HGS
{
    Font::Font(std::string_view file) : font{TTF_OpenFont(file.data(), 255.f)}
    {
        if (!font) {
            SDL_Log("CANT LOAD FONT\n");
        }
    }
    Font::~Font()
    {
        clear();
        SDL_Log("~Font\n");
    }
    Font::operator TTF_Font*() 
    {
        return font;
    }
    TTF_Font* Font::getFont()
    {
        return font; 
    }
    bool Font::clear()
    {
        if (font) {
            TTF_CloseFont(font);
            return true;
        }
        return false;
    }

}