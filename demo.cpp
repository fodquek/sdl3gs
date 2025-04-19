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
HGS::Font* defont {nullptr};

void fillDemoScene(HGS::Scene& demoScene)
{
    demoScene.clear();
    demoScene.add(LabelFactory(new HGS::Label, {0.f, 0.f, 640.f, 480.f}, "Demo!", *defont,
                              {0xff, 0xff, 0xff, 0x00}, {0xff, 0x88, 0x00, 0xff}));
    demoScene.add(BoxFactory(new HGS::Box, {10.f, 20.f, 100.f, 100.f}, {0xff, 0x00, 0xff, 0xff}));
    demoScene.add(BoxFactory(new HGS::Box, {300.f, 100.f, 42.f, 300.f}, {0xff, 0xff, 0xff, 127}));
    demoScene.add(CircleFactory(new HGS::Circle, {150.f, 20.f}, 49.f, {0xff, 0x00, 0x00, 0xff}));

}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{

    std::srand(std::time(0));
    auto srand_lmbd{std::rand};

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
    bool mainMenuActive{true};

    HGS::Scene mainMenu;
    mainMenu.add(LabelFactory(new HGS::Label, {100.f, 200.f, 150.f, 75.f}, "PLAY", *defont,
                             {0x00, 0x00, 0xff, 0xff}, {0xff, 0x88, 0x00, 0xff}));
    mainMenu.add(LabelFactory(new HGS::Label, {400.f, 200.f, 150.f, 75.f}, "EXIT", *defont,
                             {0xff, 0x88, 0x00, 0xff}, {0x00, 0x00, 0xff, 0xff}));
    HGS::Widget* btn{mainMenu.get(0)};
    HGS::Widget* ext{mainMenu.get(1)};

    HGS::Scene demoScene;
    fillDemoScene(demoScene);
    HGS::Widget* box{demoScene.get(1)};
    box->setCallBack([&demoScene, &srand_lmbd]() {
        demoScene.add(BoxFactory(new HGS::Box,
                                 {static_cast<float>(srand_lmbd() % 600) + 20.f,
                                  static_cast<float>(srand_lmbd() % 440) + 20.f, 20.f, 20.f},
                                 {static_cast<Uint8>(srand_lmbd() % 0xff),
                                  static_cast<Uint8>(srand_lmbd() % 0xff),
                                  static_cast<Uint8>(srand_lmbd() % 0xff), 0xff}));
    });

    SDL_Log("\n\n==== MAIN LOOP ===\n\n");
    while (main_loop) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                main_loop = false;
            } else if (e.type == SDL_EVENT_KEY_UP) {
                auto k{e.key.key};
                if (k == SDLK_Q || k == SDLK_ESCAPE) {
                    if (mainMenuActive) {
                        main_loop = false;
                    } else {
                        mainMenuActive = true;
                    }
                }
            } else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                SDL_GetMouseState(&mx, &my);
                if (mainMenuActive) {
                    if (btn->isContains(mx, my)) {
                        mainMenuActive = false;
                        fillDemoScene(demoScene);
                        box = demoScene.get(1);
                        box->setCallBack([&demoScene, &srand_lmbd]() {
                            demoScene.add(BoxFactory(
                                new HGS::Box,
                                {static_cast<float>(srand_lmbd() % 600) + 20.f,
                                 static_cast<float>(srand_lmbd() % 440) + 20.f, 20.f, 20.f},
                                {static_cast<Uint8>(srand_lmbd() % 0xff),
                                 static_cast<Uint8>(srand_lmbd() % 0xff),
                                 static_cast<Uint8>(srand_lmbd() % 0xff), 0xff}));
                        });
                    } else if (ext->isContains(mx, my)) {
                        main_loop = false;
                    }
                } else {
                    if (box->isContains(mx, my)) {
                        dynamic_cast<HGS::Box*>(box)->setBG({0x00, 0x00, 0xff, 0xff});
                    } else {
                        box->setPos({mx, my});
                        dynamic_cast<HGS::Box*>(box)->setBG({0xff, 0x00, 0x00, 0xff});
                    }
                    box->call2back();
                }
            }
            SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(r, 0x00, 0x00, 0x00, 0xff);
            SDL_RenderClear(r);
            if (mainMenuActive) {
                mainMenu.render(r);
            } else {
                demoScene.render(r);
            }
            SDL_RenderPresent(r);
        }
    }
    demoScene.clear();
    mainMenu.clear();
    HGS::ENG::deinit();
    return 0;
}
