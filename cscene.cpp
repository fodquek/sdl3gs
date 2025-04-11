#include "cscene.h"
namespace HGS
{
    Scene::~Scene()
    {
        clear();
        SDL_Log("SCENE GONE\n");
    }
    void Scene::add(Widget* w)
    {
        std::unique_ptr<Widget> up_w(w);
        widgets.push_back(std::move(up_w));
    }
    void Scene::render(SDL_Renderer* r) const
    {
        for(size_t i {0}; i < widgets.size(); ++i)
        {
            widgets.at(i).get()->render(r);
        }
    }
    void Scene::clear()
    {
        widgets.clear();
    }
}