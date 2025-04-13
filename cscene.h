#ifndef HGS_SCENE
#define HGS_SCENE
#include <vector>
#include <memory>
#include "cwidget.h"
namespace HGS
{
    class Scene
    {
        public:
            Scene() = default;
            ~Scene();
            void add(Widget* w);
            Widget* get(size_t i) const;
            void render(SDL_Renderer* r) const;
            void clear();
        private:
            std::vector<Widget*> widgets {};
    };
}
#endif