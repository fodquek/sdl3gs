#include "ceng.h"
namespace HGS
{
RC ENG::init()
{
    if (!SDL_Init(0)) {
        SDL_Log("SDL INIT ERROR: %s", SDL_GetError());
        return RC::SDL_INIT_ERR;
    }
    if (!TTF_Init()) {
        SDL_Log("CANT TTF_Init() !!!\n");
        return RC::SDL_TTF_INIT_ERR;
    }
    return RC::OK;
}

RC ENG::VSYNC(SDL_Renderer* r, VSYNC_MODE v)
{
    if (!SDL_SetRenderVSync(r, static_cast<int>(v))) {
        SDL_Log("VSYNC_SET_ERROR: %s", SDL_GetError());
        return RC::SDL_VSYNC_SET_ERR;
    }
    return RC::OK;
}

void ENG::deinit()
{
    TTF_Quit();
    SDL_Quit();
}
} // namespace HGS