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
#include <string>
#include <string_view>
#include <vector>
#include <ostream>
#include <algorithm>

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

/**
 * Function Prototypes
 */
void InitMainMenuHandle();
void InitPlaySceneHandle();
void InitMPCreateSceneHandle();
void InitMPJoinSceneHandle();
void NextRoundPlaySceneActors();
size_t kInWrite();
/**
 * Global Constants
 */
constexpr size_t WRITE_SIZE{11};
constexpr SDL_Keycode write_k[WRITE_SIZE] = {SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4,     SDLK_5,
                                             SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_PERIOD};
constexpr char write_c[WRITE_SIZE] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.'};
constexpr std::string_view FONT_FILE{"./assets/fonts/OpenSans-Regular.ttf"};
/**
 * Global Variables
 */
std::stringstream parser;
HGS::Font* defont{nullptr};
SDL_Event e;
float mx{-1.f};
float my{-1.f};
Uint64 milisecs;
Uint64 cnt{0};
bool press_up{false};
bool press_down{false};
enum class Scenes
{
    None = 0,
    MainMenuScene,
    MPCreateScene,
    MPJoinScene,
    MPPlayScene,
    PlayScene
} activeScene;
struct MMSceneHandle
{
    HGS::Scene scene;
    HGS::Label* sPlayBtn;
    HGS::Label* mpCreateBtn;
    HGS::Label* mpJoinBtn;
    HGS::Label* extBtn;
} mainMenuHandle;
struct MPCreateSceneHandle
{
    HGS::Scene scene;
    HGS::Label* rxPort;
    HGS::Label* createBtn;
} mpcreateSceneHandle;
struct MPJoinSceneHandle
{
    HGS::Scene scene;
    HGS::Label* rxPort;
    HGS::Label* hostIP;
    HGS::Label* txPort;
    HGS::Label* join;
    HGS::Label* writing;
} mpjoinSceneHandle;
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
    int player_score;
    int coa_score;
} playSceneHandle;

namespace HGS
{
/**
 * GAME CONSTANTS
 */
constexpr int SCREEN_W{800};
constexpr int SCREEN_H{600};
constexpr SDL_Point SCREEN_WH{SCREEN_W, SCREEN_H};
constexpr std::string_view SCREEN_TITLE{"LEPONG"};
/**
 * SDL_Colors
 */
constexpr SDL_Color RED{0xff, 0x00, 0x00, 0xff};
constexpr SDL_Color ORANGE{0xff, 0x7f, 0x00, 0xff};
constexpr SDL_Color YELLOW{0xff, 0xff, 0x00, 0xff};
constexpr SDL_Color GREEN{0x00, 0xff, 0x00, 0xff};
constexpr SDL_Color CYAN{0x00, 0xff, 0xff, 0xff};
constexpr SDL_Color BLUE{0x00, 0x00, 0xff, 0xff};
constexpr SDL_Color PURPLE{0xff, 0x00, 0xff, 0xff};
constexpr SDL_Color MAGENTA{0x7f, 0x00, 0xff, 0xff};
constexpr SDL_Color BLACK{0x00, 0x00, 0x00, 0xff};
constexpr SDL_Color WHITE{0xff, 0xff, 0xff, 0xff};
/**
 * BTN CONSTANTS
 */
constexpr float LABEL_W{150.f};
constexpr float LABEL_H{75.f};
constexpr SDL_Color BTN_BC{MAGENTA};
constexpr SDL_Color BTN_FC{WHITE};
/**
 * PADDLE CONSTANTS
 */
constexpr float PADDLE_W{20.f};
constexpr float PADDLE_H{150.f};
constexpr float PADDLE_W_OFFSET{20.f};
constexpr float PADDLE_L_PX{PADDLE_W_OFFSET};
constexpr float PADDLE_R_PX{SCREEN_W - PADDLE_W_OFFSET - PADDLE_W};
constexpr float PADDLE_PY{(SCREEN_H - PADDLE_H) / 2.f};
constexpr auto PADDLE_L_BC{BLUE};
constexpr auto PADDLE_R_BC{RED};
constexpr float PADDLE_SPD{3.f};
/**
 * BALL CONSTANTS
 */
constexpr SDL_FPoint BALL_P{SCREEN_W / 2.f, SCREEN_H / 2.f};
constexpr float BALL_R{20.f};
constexpr auto BALL_BC{YELLOW};
} // namespace HGS

/**
 * Functions
 */
void InitMainMenuHandle()
{
    // clear for re-init
    mainMenuHandle.scene.clear();

    // Single Play added to scene with its callback
    mainMenuHandle.sPlayBtn =
        dynamic_cast<HGS::Label*>(mainMenuHandle.scene
                                      .add(LabelFactory({50.f, 100.f, HGS::LABEL_W, HGS::LABEL_H},
                                                        "SINGLE", *defont, HGS::RED, HGS::WHITE))
                                      ->setCallBack([] {
                                          activeScene = Scenes::PlayScene;
                                          InitPlaySceneHandle();
                                      }));
    // Multi Play added to scene with its callback
    mainMenuHandle.mpCreateBtn = dynamic_cast<HGS::Label*>(
        mainMenuHandle.scene
            .add(LabelFactory({50.f, 200.f, HGS::LABEL_W, HGS::LABEL_H}, "MP CREATE", *defont,
                              HGS::GREEN, HGS::WHITE))
            ->setCallBack([] {
                activeScene = Scenes::MPCreateScene;
                InitMPCreateSceneHandle();
            }));
    // Multi Play added to scene with its callback
    mainMenuHandle.mpJoinBtn =
        dynamic_cast<HGS::Label*>(mainMenuHandle.scene
                                      .add(LabelFactory({50.f, 300.f, HGS::LABEL_W, HGS::LABEL_H},
                                                        "MP JOIN", *defont, HGS::BLUE, HGS::WHITE))
                                      ->setCallBack([] {
                                          activeScene = Scenes::MPJoinScene;
                                          InitMPJoinSceneHandle();
                                      }));
    // Exit button added to scene with its callback
    mainMenuHandle.extBtn =
        dynamic_cast<HGS::Label*>(mainMenuHandle.scene
                                      .add(LabelFactory({50.f, 400.f, HGS::LABEL_W, HGS::LABEL_H},
                                                        "EXIT", *defont, HGS::WHITE, HGS::BLACK))
                                      ->setCallBack([] { activeScene = Scenes::None; }));
}

void InitMPCreateSceneHandle()
{
    mpcreateSceneHandle.scene.clear();

    mpcreateSceneHandle.rxPort =
        dynamic_cast<HGS::Label*>(mpcreateSceneHandle.scene.add(HGS::LabelFactory(
            {50.f, 100.f, HGS::LABEL_W, HGS::LABEL_H}, "5000", *defont, HGS::BLUE, HGS::WHITE)));

    mpcreateSceneHandle.createBtn = dynamic_cast<HGS::Label*>(mpcreateSceneHandle.scene.add(
        HGS::LabelFactory({50.f, 200.f, HGS::LABEL_W, HGS::LABEL_H}, "CREATE", *defont, HGS::YELLOW,
                          HGS::WHITE)));
}

void InitMPJoinSceneHandle()
{
    mpjoinSceneHandle.scene.clear();

    mpjoinSceneHandle.rxPort = dynamic_cast<HGS::Label*>(
        mpjoinSceneHandle.scene
            .add(HGS::LabelFactory({50.f, 100.f, HGS::LABEL_W, HGS::LABEL_H}, "6000", *defont,
                                   HGS::MAGENTA, HGS::WHITE))
            ->setCallBack([] {
                mpjoinSceneHandle.writing->setBG(HGS::MAGENTA);
                mpjoinSceneHandle.writing = mpjoinSceneHandle.rxPort;
                mpjoinSceneHandle.writing->setBG(HGS::CYAN);
            }));

    mpjoinSceneHandle.hostIP = dynamic_cast<HGS::Label*>(
        mpjoinSceneHandle.scene
            .add(HGS::LabelFactory({50.f, 200.f, HGS::LABEL_W, HGS::LABEL_H}, "localhost", *defont,
                                   HGS::MAGENTA, HGS::WHITE))
            ->setCallBack([] {
                mpjoinSceneHandle.writing->setBG(HGS::MAGENTA);
                mpjoinSceneHandle.writing = mpjoinSceneHandle.hostIP;
                mpjoinSceneHandle.writing->setBG(HGS::CYAN);
            }));

    mpjoinSceneHandle.txPort = dynamic_cast<HGS::Label*>(
        mpjoinSceneHandle.scene
            .add(HGS::LabelFactory({50.f, 300.f, HGS::LABEL_W, HGS::LABEL_H}, "5000", *defont,
                                   HGS::MAGENTA, HGS::WHITE))
            ->setCallBack([] {
                mpjoinSceneHandle.writing->setBG(HGS::MAGENTA);
                mpjoinSceneHandle.writing = mpjoinSceneHandle.txPort;
                mpjoinSceneHandle.writing->setBG(HGS::CYAN);
            }));

    mpjoinSceneHandle.join =
        dynamic_cast<HGS::Label*>(mpjoinSceneHandle.scene.add(HGS::LabelFactory(
            {50.f, 400.f, HGS::LABEL_W, HGS::LABEL_H}, "JOIN", *defont, HGS::YELLOW, HGS::WHITE)));

    mpjoinSceneHandle.writing = mpjoinSceneHandle.rxPort;
    mpjoinSceneHandle.writing->setBG(HGS::CYAN);
}

void InitPlaySceneHandle()
{
    playSceneHandle.scene.clear();
    /**
     * left paddle
     */
    playSceneHandle.player =
        dynamic_cast<HGS::Paddle*>(playSceneHandle.scene.add(HGS::PaddleFactory(
            {HGS::PADDLE_L_PX, HGS::PADDLE_PY, HGS::PADDLE_W, HGS::PADDLE_H}, HGS::PADDLE_L_BC)));
    /**
     * right paddle
     */
    playSceneHandle.coa = dynamic_cast<HGS::Paddle*>(playSceneHandle.scene.add(HGS::PaddleFactory(
        {HGS::PADDLE_R_PX, HGS::PADDLE_PY, HGS::PADDLE_W, HGS::PADDLE_H}, HGS::PADDLE_R_BC)));
    /**
     * Clock
     */
    playSceneHandle.time = dynamic_cast<HGS::Label*>(playSceneHandle.scene.add(
        HGS::LabelFactory({250.f, 10.f, 120.f, 20.f}, "0 : 0", *defont, {0xff, 0xff, 0xff, 0x00},
                          {0xff, 0xff, 0xff, 0xff})));
    /**
     * Scoreboard
     */
    playSceneHandle.scores = dynamic_cast<HGS::Label*>(playSceneHandle.scene.add(
        HGS::LabelFactory({250.f, 50.f, 120.f, HGS::LABEL_H}, "0 : 0", *defont,
                          {0xff, 0xff, 0xff, 0x00}, {0xff, 0xff, 0xff, 0xff})));
    /**
     * Ball
     */
    playSceneHandle.ball = dynamic_cast<HGS::Ball*>(playSceneHandle.scene.add(
        HGS::BallFactory(HGS::BALL_P, HGS::BALL_R, HGS::BALL_BC, {5.f, 1.f})));
    playSceneHandle.min = 0;
    playSceneHandle.sec = 0;
    playSceneHandle.player_score = 0;
    playSceneHandle.coa_score = 0;
    cnt = 0;
}

void NextRoundPlaySceneActors()
{
    /**
     * left paddle
     */
    playSceneHandle.player->setPos({HGS::PADDLE_L_PX, HGS::PADDLE_PY});
    playSceneHandle.player->setMove(0.f);
    /**
     * right paddle
     */
    playSceneHandle.coa->setPos({HGS::PADDLE_R_PX, HGS::PADDLE_PY});
    playSceneHandle.coa->setMove(0.f);
    /**
     * Ball
     */
    playSceneHandle.ball->setPos(HGS::BALL_P);
    playSceneHandle.ball->setVel({5.f, 1.f});
}

size_t kInWrite(SDL_Keycode k)
{
    for (size_t i{0}; i < WRITE_SIZE; ++i) {
        if (k == write_k[i]) {
            return i;
        }
    }
    return std::string::npos;
}

/**
 * **********
 * MAIN ENTRY
 * **********
 */
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    std::srand(static_cast<unsigned int>(std::time(0)));
    if (const auto rc{HGS::ENG::init()}; rc != HGS::RC::OK) {
        std::cerr << "SDL_CANT_INIT\n";
        return static_cast<int>(rc);
    }
    defont = new HGS::Font(FONT_FILE);
    HGS::Window w{HGS::SCREEN_TITLE, HGS::SCREEN_WH};
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
            } else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                SDL_GetMouseState(&mx, &my);
                if (activeScene == Scenes::MainMenuScene) {
                    if (mainMenuHandle.sPlayBtn->isContains(mx, my)) {
                        mainMenuHandle.sPlayBtn->call2back();
                    } else if (mainMenuHandle.mpCreateBtn->isContains(mx, my)) {
                        mainMenuHandle.mpCreateBtn->call2back();
                    } else if (mainMenuHandle.mpJoinBtn->isContains(mx, my)) {
                        mainMenuHandle.mpJoinBtn->call2back();
                    } else if (mainMenuHandle.extBtn->isContains(mx, my)) {
                        mainMenuHandle.extBtn->call2back();
                    }
                } else if (activeScene == Scenes::MPCreateScene) {
                    if (mpcreateSceneHandle.createBtn->isContains(mx, my)) {
                        mpcreateSceneHandle.createBtn->call2back();
                    }
                } else if (activeScene == Scenes::MPJoinScene) {
                    if (mpjoinSceneHandle.rxPort->isContains(mx, my)) {
                        mpjoinSceneHandle.rxPort->call2back();
                    } else if (mpjoinSceneHandle.hostIP->isContains(mx, my)) {
                        mpjoinSceneHandle.hostIP->call2back();
                    } else if (mpjoinSceneHandle.txPort->isContains(mx, my)) {
                        mpjoinSceneHandle.txPort->call2back();
                    } else if (mpjoinSceneHandle.join->isContains(mx, my)) {
                        mpjoinSceneHandle.join->call2back();
                    }
                }
            } else if (e.type == SDL_EVENT_KEY_UP) {
                const auto& k{e.key.key};
                if (k == SDLK_Q || k == SDLK_ESCAPE) {
                    if (activeScene == Scenes::MainMenuScene) {
                        activeScene = Scenes::None;
                    } else {
                        activeScene = Scenes::MainMenuScene;
                    }
                } else if (k == SDLK_UP) {
                    press_up = false;
                } else if (k == SDLK_DOWN) {
                    press_down = false;
                } else if (auto i{kInWrite(k)}; i != std::string::npos) {
                    if (activeScene == Scenes::MPCreateScene) {
                        parser.str("");
                        parser.clear();
                        parser << mpcreateSceneHandle.rxPort->getText();
                        parser << write_c[i];
                        mpcreateSceneHandle.rxPort->setText(parser.str());
                    } else if (activeScene == Scenes::MPJoinScene) {
                        parser.str("");
                        parser.clear();
                        parser << mpjoinSceneHandle.writing->getText();
                        parser << write_c[i];
                        mpjoinSceneHandle.writing->setText(parser.str());
                    }
                } else if (k == SDLK_BACKSPACE) {
                    if (activeScene == Scenes::MPCreateScene) {
                        std::string s{mpcreateSceneHandle.rxPort->getText()};
                        mpcreateSceneHandle.rxPort->setText(s.substr(0, s.size() - 1));
                    } else if (activeScene == Scenes::MPJoinScene) {
                        std::string s{mpjoinSceneHandle.writing->getText()};
                        mpjoinSceneHandle.writing->setText(s.substr(0, s.size() - 1));
                    }
                }
            } else if (e.type == SDL_EVENT_KEY_DOWN) {
                const auto& k{e.key.key};
                if (activeScene == Scenes::PlayScene) {
                    if (k == SDLK_UP) {
                        press_up = true;
                    } else if (k == SDLK_DOWN) {
                        press_down = true;
                    }
                }
            }
        }

        /**
         * Render Phase
         */
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(r, 0x00, 0x00, 0x00, 0xff);
        SDL_RenderClear(r);

        if (activeScene == Scenes::MainMenuScene) {
            mainMenuHandle.scene.render(r);
        } else if (activeScene == Scenes::MPCreateScene) {
            mpcreateSceneHandle.scene.render(r);
        } else if (activeScene == Scenes::MPJoinScene) {
            mpjoinSceneHandle.scene.render(r);
        } else if (activeScene == Scenes::PlayScene) {
            /**
             * update render stuff
             */
            playSceneHandle.ball->step(1.f);
            playSceneHandle.player->step(1.f);
            playSceneHandle.coa->step(1.f);
            parser.str("");
            parser.clear();
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
            if (playSceneHandle.player_score == 3) {
                // player win
                activeScene = Scenes::MainMenuScene;
            } else if (playSceneHandle.coa_score == 3) {
                // coa win
                activeScene = Scenes::MainMenuScene;
            }

            parser.str("");
            parser.clear();
            parser << playSceneHandle.player_score;
            parser << " : ";
            parser << playSceneHandle.coa_score;
            playSceneHandle.scores->setText(parser.str());
            /**
             * set move player
             */
            if (press_up) {
                playSceneHandle.player->setMove(-HGS::PADDLE_SPD);
                if (auto p{playSceneHandle.player->getPos()}; p.y < 0.f) {
                    p.y = 0.f;
                    playSceneHandle.player->setPos(p);
                }
            } else if (press_down) {
                playSceneHandle.player->setMove(HGS::PADDLE_SPD);
                if (auto p{playSceneHandle.player->getPos()};
                    p.y > (HGS::SCREEN_H - HGS::PADDLE_H)) {
                    p.y = (HGS::SCREEN_H - HGS::PADDLE_H);
                    playSceneHandle.player->setPos(p);
                }
            } else {
                playSceneHandle.player->setMove(0.f);
            }

            /**
             * set move coa
             */
            {
                const auto ball_y{playSceneHandle.ball->getPos().y};
                const auto coa_y{playSceneHandle.coa->getCenPos().y};
                const auto dy{coa_y - ball_y};
                if (dy < 0.f) {
                    playSceneHandle.coa->setMove(HGS::PADDLE_SPD);

                } else if (dy > 0.f) {
                    playSceneHandle.coa->setMove(-HGS::PADDLE_SPD);
                } else {
                    playSceneHandle.coa->setMove(0.f);
                }
                if (auto p{playSceneHandle.coa->getPos()}; p.y < 0.f) {
                    p.y = 0.f;
                    playSceneHandle.coa->setPos(p);
                } else if (auto p{playSceneHandle.coa->getPos()};
                           p.y > (HGS::SCREEN_H - HGS::PADDLE_H)) {
                    p.y = (HGS::SCREEN_H - HGS::PADDLE_H);
                    playSceneHandle.coa->setPos(p);
                }
            }

            /**
             * set vel ball
             */
            {
                const auto [bx, by]{playSceneHandle.ball->getPos()};
                if (bx < 0.f) {
                    playSceneHandle.coa_score += 1;
                    NextRoundPlaySceneActors();
                } else if (bx > HGS::SCREEN_W) {
                    playSceneHandle.player_score += 1;
                    NextRoundPlaySceneActors();
                } else if ((by < 0.f) ||
                           (by > (HGS::SCREEN_H - playSceneHandle.ball->getRadius()))) {
                    playSceneHandle.ball->velYinverse();
                }
                if (bx < HGS::SCREEN_W / 2.f) {
                    const auto ball_r{playSceneHandle.ball->getRadius()};
                    const auto [x, y]{playSceneHandle.player->getCenPos()};
                    const auto dx{x - bx};
                    const auto dy{y - by};
                    if (dx > (-5.f - ball_r) && dx < (5.f + ball_r)) {
                        if (dy > -(HGS::PADDLE_H / 2.f + ball_r) &&
                            dy < (HGS::PADDLE_H / 2.f + ball_r)) {
                            auto vel{playSceneHandle.ball->getVel()};
                            vel.x = -1.f * (vel.x + (0.1 * vel.x));
                            if (vel.y < 0.f) {
                                vel.y = static_cast<float>(-1 * (std::rand() % 50)) / 10.f;
                            } else {
                                vel.y = static_cast<float>(std::rand() % 50) / 10.f;
                            }
                            playSceneHandle.ball->setVel(vel);
                        }
                    }
                } else {
                    const auto ball_r{playSceneHandle.ball->getRadius()};
                    const auto [x, y]{playSceneHandle.coa->getCenPos()};
                    const auto dx{x - bx};
                    const auto dy{y - by};
                    if (dx > (-5.f - ball_r) && dx < (5.f + ball_r)) {
                        if (dy > -(HGS::PADDLE_H / 2.f + ball_r) &&
                            dy < (HGS::PADDLE_H / 2.f + ball_r)) {
                            auto vel{playSceneHandle.ball->getVel()};
                            vel.x = -1.f * (vel.x + (0.1 * vel.x));
                            if (vel.y < 0.f) {
                                vel.y = static_cast<float>(-1 * (std::rand() % 50)) / 10.f;
                            } else {
                                vel.y = static_cast<float>(std::rand() % 50) / 10.f;
                            }
                            playSceneHandle.ball->setVel(vel);
                        }
                    }
                }
            }
            /**
             * render finalized screen
             */
            playSceneHandle.scene.render(r);
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
    playSceneHandle.scene.clear();
    mpcreateSceneHandle.scene.clear();
    mpjoinSceneHandle.scene.clear();
    mainMenuHandle.scene.clear();
    HGS::ENG::deinit();
    return 0;
}