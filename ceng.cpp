#include "ceng.h"
namespace HGS
{
    bool ENG::init()
    {
        if (!SDL_Init(0))
        {
            SDL_Log("SDL INIT ERROR: %s", SDL_GetError());
            return false;
        }
        return true;
    }

    bool ENG::VSYNC(SDL_Renderer* r, int v)
    {
        if (!SDL_SetRenderVSync(r, v)) {
            SDL_Log("VSYNC_SET_ERROR: %s", SDL_GetError());
            return false;
        }
        return true;
    }

    void ENG::deinit()
    {
        SDL_Quit();
    }
}