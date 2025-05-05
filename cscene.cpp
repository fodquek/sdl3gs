#include "cscene.h"
namespace HGS
{
    Scene::~Scene()
    {
        clear();
        SDL_Log("SCENE GONE\n");
    }
    Widget* Scene::add(Widget* w)
    {
        widgets.push_back(std::move(w));
        return *widgets.rbegin();
        // return widgets.at(widgets.size() - 1);
    }
    Widget* Scene::get(size_t i) const
    {
        if (i >= widgets.size()) {
            return nullptr;
        }
        return widgets.at(i);
    }
    void Scene::render(SDL_Renderer* r) const
    {
        for(size_t i {0}; i < widgets.size(); ++i)
        {
            widgets.at(i)->render(r);
        }
    }
    void Scene::clear()
    {
        for (auto w : widgets)
        {
            delete w;
        }
        widgets.clear();
    }
}