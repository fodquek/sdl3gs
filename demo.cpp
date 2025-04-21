// /* Headers */
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <sys/types.h>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>

#include "ceng.h"
#include "crenderer.h"
#include "cwindow.h"
#include "cfont.h"
#include "cbox.h"
#include "ccircle.h"
#include "clabel.h"
#include "cscene.h"

constexpr std::string_view FONT_FILE{"./assets/fonts/OpenSans-Regular.ttf"};
HGS::Font* defont{nullptr};

enum class Scenes
{
    None = 0,
    MainMenuScene,
    PlayScene
} activeScene;

struct MMSceneHandle
{
    HGS::Scene scene;
    HGS::Widget* playBtn;
    HGS::Widget* extBtn;
} mainMenuHandle;

void InitMainMenuHandle(struct MMSceneHandle& mmh)
{
    mmh.scene.clear();
    mmh.scene.add(LabelFactory({100.f, 200.f, 150.f, 75.f}, "PLAY", *defont,
                               {0x00, 0x00, 0xff, 0xff}, {0xff, 0x88, 0x00, 0xff}));
    mmh.scene.add(LabelFactory({400.f, 200.f, 150.f, 75.f}, "EXIT", *defont,
                               {0xff, 0x88, 0x00, 0xff}, {0x00, 0x00, 0xff, 0xff}));
    mmh.playBtn = mmh.scene.get(0);
    mmh.extBtn = mmh.scene.get(1);
    mmh.extBtn->setCallBack([] { activeScene = Scenes::PlayScene; });
}

struct DemoSceneHandle
{
    HGS::Scene scene;
    HGS::Widget* box;
} demoSceneHandle;

void InitDemoSceneHandle(struct DemoSceneHandle& dsh)
{
    dsh.scene.clear();
    dsh.scene.add(HGS::LabelFactory({0.f, 0.f, 640.f, 480.f}, "Demo!", *defont,
                                    {0xff, 0xff, 0xff, 0x00}, {0xff, 0x88, 0x00, 0xff}));
    dsh.scene.add(HGS::BoxFactory({10.f, 20.f, 100.f, 100.f}, {0xff, 0x00, 0xff, 0xff}));
    dsh.scene.add(HGS::BoxFactory({300.f, 100.f, 42.f, 300.f}, {0xff, 0xff, 0xff, 127}));
    dsh.scene.add(HGS::CircleFactory({150.f, 20.f}, 49.f, {0xff, 0x00, 0x00, 0xff}));
    dsh.box = dsh.scene.get(1);
    dsh.box->setCallBack([&dsh] {
        dsh.scene.add(HGS::BoxFactory({static_cast<float>(std::rand() % 600) + 20.f,
                                       static_cast<float>(std::rand() % 440) + 20.f, 20.f, 20.f},
                                      {static_cast<Uint8>(std::rand() % 0xff),
                                       static_cast<Uint8>(std::rand() % 0xff),
                                       static_cast<Uint8>(std::rand() % 0xff), 0xff}));
    });
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    std::srand(std::time(0));

    if (const auto rc{HGS::ENG::init()}; rc != HGS::RC::OK) {
        std::cerr << "SDL_CANT_INIT\n";
        return static_cast<int>(rc);
    }
    defont = new HGS::Font(FONT_FILE);
    HGS::Window w{"demo", {640, 480}};
    HGS::Renderer r{&w};
    if (const auto rc{HGS::ENG::VSYNC(r, HGS::VSYNC_MODE::ON)}; rc != HGS::RC::OK) {
        std::cerr << "SDL_VSYNC_SET_ERR\n";
        HGS::ENG::deinit();
        return static_cast<int>(rc);
    }

    bool main_loop{true};
    SDL_Event e{};
    float mx;
    float my;

    activeScene = Scenes::MainMenuScene;
    InitMainMenuHandle(mainMenuHandle);

    SDL_Log("\n\n==== MAIN LOOP ===\n\n");
    while (activeScene != Scenes::None) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                main_loop = false;
            } else if (e.type == SDL_EVENT_KEY_UP) {
                auto k{e.key.key};
                if (k == SDLK_Q || k == SDLK_ESCAPE) {
                    if (activeScene == Scenes::MainMenuScene) {
                        activeScene = Scenes::None;
                    } else {
                        activeScene = Scenes::MainMenuScene;
                    }
                }
            } else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                SDL_GetMouseState(&mx, &my);
                if (activeScene == Scenes::MainMenuScene) {
                    if (mainMenuHandle.playBtn->isContains(mx, my)) {
                        activeScene = Scenes::PlayScene;
                        InitDemoSceneHandle(demoSceneHandle);
                    } else if (mainMenuHandle.extBtn->isContains(mx, my)) {
                        activeScene = Scenes::None;
                    }
                } else {
                    if (demoSceneHandle.box->isContains(mx, my)) {
                        dynamic_cast<HGS::Box*>(demoSceneHandle.box)
                            ->setBG({0x00, 0x00, 0xff, 0xff});
                    } else {
                        demoSceneHandle.box->setPos({mx, my});
                        dynamic_cast<HGS::Box*>(demoSceneHandle.box)
                            ->setBG({0xff, 0x00, 0x00, 0xff});
                    }
                    demoSceneHandle.box->call2back();
                }
            }
            SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(r, 0x00, 0x00, 0x00, 0xff);
            SDL_RenderClear(r);
            switch (activeScene) {
            case Scenes::MainMenuScene:
                mainMenuHandle.scene.render(r);
                break;
            case Scenes::PlayScene:
                demoSceneHandle.scene.render(r);
            default:
                // todo
                break;
            }
            SDL_RenderPresent(r);
        }
    }

    demoSceneHandle.scene.clear();
    mainMenuHandle.scene.clear();
    HGS::ENG::deinit();
    return 0;
}
