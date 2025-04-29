// /* Headers */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
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
#include <ostream>

#include "ceng.h"
#include "crenderer.h"
#include "cwindow.h"
#include "cfont.h"
#include "cbox.h"
#include "cpaddle.h"
#include "ccircle.h"
#include "cball.h"
#include "clabel.h"
#include "cscores.h"
#include "cscene.h"

constexpr std::string_view FONT_FILE{"./assets/fonts/OpenSans-Regular.ttf"};
HGS::Font* defont{nullptr};

SDL_Event e{};
float mx;
float my;

enum class Scenes
{
    None = 0,
    MainMenuScene,
    PlayScene
} activeScene;

struct MMSceneHandle
{
    HGS::Scene scene;
    HGS::Label* playBtn;
    HGS::Label* extBtn;
} mainMenuHandle;

struct DemoSceneHandle
{
    HGS::Scene scene;
    HGS::Box* box;
} demoSceneHandle;

struct PlaySceneHandle
{
    HGS::Scene scene;
    HGS::Paddle* player;
    HGS::Paddle* coa;
    HGS::Ball* ball;
    HGS::Label* time;
    HGS::Label* scores;
    int min;
    int sec;
    int l;
    int r;
} playSceneHandle;

void InitMainMenuHandle();
void InitDemoSceneHandle();
void InitPlaySceneHandle();

Uint64 milisecs;
Uint64 cnt{0};

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    std::srand(static_cast<unsigned int>(std::time(0)));

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

    activeScene = Scenes::MainMenuScene;
    InitMainMenuHandle();
    milisecs = SDL_GetTicks();
    SDL_Log("\n\n==== MAIN LOOP ===\n\n");
    while (activeScene != Scenes::None) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                activeScene = Scenes::None;
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
                        mainMenuHandle.playBtn->call2back();
                    } else if (mainMenuHandle.extBtn->isContains(mx, my)) {
                        mainMenuHandle.extBtn->call2back();
                    }
                }
            }
            // else if (e.type == SDL_EVENT_KEY_DOWN) {
            //     const auto& k{e.key.key};
            //     if (activeScene == Scenes::PlayScene) {
            //         switch (k) {
            //         case SDLK_UP:
            //             break;
            //         }
            //     }
            // }
        }
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(r, 0x00, 0x00, 0x00, 0xff);
        SDL_RenderClear(r);
        std::stringstream parser;
        switch (activeScene) {
        case Scenes::MainMenuScene:
            mainMenuHandle.scene.render(r);
            break;
        case Scenes::PlayScene:
            playSceneHandle.ball->step(1.f);
            playSceneHandle.player->step(1.f);
            if (playSceneHandle.min < 10) {
                parser << "0";
            }
            parser << playSceneHandle.min;
            parser << ":";
            if (playSceneHandle.sec < 10) {
                parser << "0";
            }
            parser << playSceneHandle.sec;
            playSceneHandle.time->setText(parser.str());
            playSceneHandle.scene.render(r);
        default:
            // todo
            break;
        }
        SDL_RenderPresent(r);
        while ((SDL_GetTicks() - milisecs) < 1000 / 60) {
            ;
        }
        milisecs = SDL_GetTicks();
        ++cnt;
        playSceneHandle.min = cnt / 3600;
        playSceneHandle.sec = (cnt / 60) % 60;
    }

    /**
     * add custom exit function via std
     */
    demoSceneHandle.scene.clear();
    playSceneHandle.scene.clear();
    mainMenuHandle.scene.clear();
    HGS::ENG::deinit();
    return 0;
}

void InitMainMenuHandle()
{
    mainMenuHandle.scene.clear();
    mainMenuHandle.scene.add(LabelFactory({100.f, 200.f, 150.f, 75.f}, "PLAY", *defont,
                                          {0x00, 0x00, 0xff, 0xff}, {0xff, 0x88, 0x00, 0xff}));
    mainMenuHandle.scene.add(LabelFactory({400.f, 200.f, 150.f, 75.f}, "EXIT", *defont,
                                          {0xff, 0x88, 0x00, 0xff}, {0x00, 0x00, 0xff, 0xff}));
    mainMenuHandle.playBtn = dynamic_cast<HGS::Label*>(mainMenuHandle.scene.get(0));
    mainMenuHandle.playBtn->setCallBack([] {
        activeScene = Scenes::PlayScene;
        InitPlaySceneHandle();
    });
    mainMenuHandle.extBtn = dynamic_cast<HGS::Label*>(mainMenuHandle.scene.get(1));
    mainMenuHandle.extBtn->setCallBack([] { activeScene = Scenes::None; });
}

void InitDemoSceneHandle()
{
    demoSceneHandle.scene.clear();
    demoSceneHandle.scene.add(HGS::LabelFactory({0.f, 0.f, 640.f, 480.f}, "Demo!", *defont,
                                                {0xff, 0xff, 0xff, 0x00},
                                                {0xff, 0x88, 0x00, 0xff}));
    demoSceneHandle.scene.add(
        HGS::BoxFactory({10.f, 20.f, 100.f, 100.f}, {0xff, 0x00, 0xff, 0xff}));
    demoSceneHandle.scene.add(
        HGS::BoxFactory({300.f, 100.f, 42.f, 300.f}, {0xff, 0xff, 0xff, 127}));
    demoSceneHandle.scene.add(HGS::CircleFactory({150.f, 20.f}, 49.f, {0xff, 0x00, 0x00, 0xff}));
    demoSceneHandle.box = dynamic_cast<HGS::Box*>(demoSceneHandle.scene.get(1));
    demoSceneHandle.box->setCallBack([] {
        if (demoSceneHandle.box->isContains(mx, my)) {
            demoSceneHandle.box->setBG({0x00, 0x00, 0xff, 0xff});
        } else {
            demoSceneHandle.box->setPos({mx, my});
            demoSceneHandle.box->setBG({0xff, 0x00, 0x00, 0xff});
        }
        demoSceneHandle.scene.add(HGS::BoxFactory(
            {static_cast<float>(std::rand() % 600) + 20.f,
             static_cast<float>(std::rand() % 440) + 20.f, 20.f, 20.f},
            {static_cast<Uint8>(std::rand() % 0xff), static_cast<Uint8>(std::rand() % 0xff),
             static_cast<Uint8>(std::rand() % 0xff), 0xff}));
    });
}

void InitPlaySceneHandle()
{
    playSceneHandle.scene.clear();
    playSceneHandle.scene.add(
        HGS::PaddleFactory({20.f, 180.f, 20.f, 120.f}, {0x00, 0x00, 0xff, 0xff}));
    playSceneHandle.scene.add(
        HGS::PaddleFactory({600.f, 180.f, 20.f, 120.f}, {0xff, 0x00, 0x00, 0xff}));
    playSceneHandle.scene.add(HGS::LabelFactory({250.f, 10.f, 120.f, 20.f}, "0 : 0", *defont,
                                                {0xff, 0xff, 0xff, 0x00},
                                                {0xff, 0xff, 0xff, 0xff}));
    playSceneHandle.scene.add(HGS::LabelFactory({250.f, 50.f, 120.f, 20.f}, "0 : 0", *defont,
                                                {0xff, 0xff, 0xff, 0x00},
                                                {0xff, 0xff, 0xff, 0xff}));
    playSceneHandle.scene.add(
        HGS::BallFactory({310.f, 230.f}, {20.f}, {0xff, 0xff, 0x00, 0xff}, {5.f, 0.f}));
    playSceneHandle.player = dynamic_cast<HGS::Paddle*>(playSceneHandle.scene.get(0));
    playSceneHandle.coa = dynamic_cast<HGS::Paddle*>(playSceneHandle.scene.get(1));
    playSceneHandle.time = dynamic_cast<HGS::Label*>(playSceneHandle.scene.get(2));
    playSceneHandle.scores = dynamic_cast<HGS::Label*>(playSceneHandle.scene.get(3));
    playSceneHandle.ball = dynamic_cast<HGS::Ball*>(playSceneHandle.scene.get(4));
    playSceneHandle.min = 0;
    playSceneHandle.sec = 0;
    playSceneHandle.l = 0;
    playSceneHandle.r = 0;
    cnt = 0;
}