#ifndef HGS_ENG
#define HGS_ENG
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
namespace HGS
{

enum class RC : int
{
    OK = 0,
    SDL_INIT_ERR,
    SDL_WIN_INIT_ERR,
    SDL_REN_INIT_ERR,
    SDL_VSYNC_SET_ERR,
    SDL_TTF_INIT_ERR
};

enum class VSYNC_MODE : int
{
    OFF = 0,
    ON = 1
};

class ENG
{
public:
    [[nodiscard]] static RC init();
    [[nodiscard]] static RC VSYNC(SDL_Renderer* r, VSYNC_MODE v);
    static void deinit();
};
} // namespace HGS
#endif