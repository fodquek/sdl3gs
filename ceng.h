#ifndef HGS_ENG
#define HGS_END
#include <SDL3/SDL.h>
namespace HGS
{
    class ENG
    {
        public:
            [[nodiscard]] static bool init();
            [[nodiscard]] static bool VSYNC(SDL_Renderer* r, int v);
            static void deinit();
    };
}
#endif