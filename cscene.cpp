#include "cscene.h"
namespace HGS
{
    Scene::~Scene()
    {
        clear();
    }
    void Scene::add(Widget* w)
    {
        widgets.push_back(w);
    }
    void Scene::render(SDL_Renderer* r) const
    {
        for(const auto* w : widgets)
        {
            w->render(r);
        }
    }
    void Scene::clear()
    {
        widgets.clear();
    }
}