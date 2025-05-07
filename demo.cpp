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
#include <thread>
#include <iomanip>

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
#include "udpns.h"

/**
 * Function Prototypes
 */
void InitMainMenuHandle();
void InitMPSceneHandle();
void InitPlaySceneHandle();
void InitFinalSceneHandle(bool w);
void NextRoundPlaySceneActors();
size_t kInWrite(SDL_Keycode k);
void mpRXthread();
void mpTXthread();
void MovePlayerPaddle();
void MoveCOHPaddle();
void MoveBall();
void UpdateScores();
void UpdateClock();
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
bool KEEP_CON{true};
bool MP_FIRST_PACKAGE_CAME{false};
UDPNS::UDP udpMan;
Uint64 rx_ms{};
Uint64 tx_ms{};
int ARGC;
char** ARGV;
std::stringstream parser;
HGS::Font* defont{nullptr};
SDL_Event e;
float mx{-1.f};
float my{-1.f};
Uint64 rx_ts;
Uint64 milisecs;
Uint64 cnt{0};
bool press_up{false};
bool press_down{false};
enum class Roles
{
    Idle,
    Server,
    Client
} activeRole;
enum class Scenes
{
    None = 0,
    MainMenuScene,
    MPScene,
    PlayScene,
    FinalScene,
} activeScene;
struct MMSceneHandle
{
    HGS::Scene scene;
    HGS::Label* spBtn;
    HGS::Label* mpBtn;
    HGS::Label* extBtn;
} mainMenuHandle;
struct MPSceneHandle
{
    HGS::Scene scene;
    HGS::Label* myIP;
    HGS::Label* rxPort;
    HGS::Label* cohIP;
    HGS::Label* txPort;
    HGS::Label* writing;
    HGS::Label* create;
    HGS::Label* join;
} mpSceneHandle;
struct PlaySceneHandle
{
    HGS::Scene scene;
    HGS::Paddle* player;
    HGS::Paddle* coh;
    HGS::Ball* ball;
    HGS::Label* time;
    HGS::Label* scores;
    int min;
    int sec;
    int player_score;
    int coh_score;
} playSceneHandle;
struct FinalSceneHandle
{
    HGS::Scene scene;
    HGS::Label* result;
} finalSceneHandle;

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
    mainMenuHandle.spBtn =
        dynamic_cast<HGS::Label*>(mainMenuHandle.scene
                                      .add(LabelFactory({50.f, 100.f, HGS::LABEL_W, HGS::LABEL_H},
                                                        "SINGLE", *defont, HGS::RED, HGS::WHITE))
                                      ->setCallBack([] {
                                          activeRole = Roles::Idle;
                                          activeScene = Scenes::PlayScene;
                                          InitPlaySceneHandle();
                                      }));
    // Multi Play setup
    mainMenuHandle.mpBtn = dynamic_cast<HGS::Label*>(
        mainMenuHandle.scene
            .add(LabelFactory({50.f, 200.f, HGS::LABEL_W, HGS::LABEL_H}, "MULTIPLAYER", *defont,
                              HGS::GREEN, HGS::WHITE))
            ->setCallBack([] {
                activeScene = Scenes::MPScene;
                InitMPSceneHandle();
            }));
    // Exit button added to scene with its callback
    mainMenuHandle.extBtn =
        dynamic_cast<HGS::Label*>(mainMenuHandle.scene
                                      .add(LabelFactory({50.f, 300.f, HGS::LABEL_W, HGS::LABEL_H},
                                                        "EXIT", *defont, HGS::BLUE, HGS::BLACK))
                                      ->setCallBack([] { activeScene = Scenes::None; }));

    MP_FIRST_PACKAGE_CAME = false;
}

void InitMPSceneHandle()
{
    mpSceneHandle.scene.clear();

    mpSceneHandle.myIP = dynamic_cast<HGS::Label*>(
        mpSceneHandle.scene
            .add(HGS::LabelFactory({50.f, 100.f, HGS::LABEL_W, HGS::LABEL_H},
                                   (ARGC > 1 ? ARGV[1] : "localhost"), *defont, HGS::MAGENTA,
                                   HGS::WHITE))
            ->setCallBack([] {
                mpSceneHandle.writing->setBG(HGS::MAGENTA);
                mpSceneHandle.writing = mpSceneHandle.myIP;
                mpSceneHandle.writing->setBG(HGS::CYAN);
            }));

    mpSceneHandle.rxPort = dynamic_cast<HGS::Label*>(
        mpSceneHandle.scene
            .add(HGS::LabelFactory({250.f, 100.f, HGS::LABEL_W, HGS::LABEL_H},
                                   (ARGC > 2 ? ARGV[2] : "5000"), *defont, HGS::MAGENTA,
                                   HGS::WHITE))
            ->setCallBack([] {
                mpSceneHandle.writing->setBG(HGS::MAGENTA);
                mpSceneHandle.writing = mpSceneHandle.rxPort;
                mpSceneHandle.writing->setBG(HGS::CYAN);
            }));

    mpSceneHandle.cohIP = dynamic_cast<HGS::Label*>(
        mpSceneHandle.scene
            .add(HGS::LabelFactory({50.f, 200.f, HGS::LABEL_W, HGS::LABEL_H},
                                   (ARGC > 3 ? ARGV[3] : "localhost"), *defont, HGS::MAGENTA,
                                   HGS::WHITE))
            ->setCallBack([] {
                mpSceneHandle.writing->setBG(HGS::MAGENTA);
                mpSceneHandle.writing = mpSceneHandle.cohIP;
                mpSceneHandle.writing->setBG(HGS::CYAN);
            }));

    mpSceneHandle.txPort = dynamic_cast<HGS::Label*>(
        mpSceneHandle.scene
            .add(HGS::LabelFactory({250.f, 200.f, HGS::LABEL_W, HGS::LABEL_H},
                                   (ARGC > 4 ? ARGV[4] : "6000"), *defont, HGS::MAGENTA,
                                   HGS::WHITE))
            ->setCallBack([] {
                mpSceneHandle.writing->setBG(HGS::MAGENTA);
                mpSceneHandle.writing = mpSceneHandle.txPort;
                mpSceneHandle.writing->setBG(HGS::CYAN);
            }));

    mpSceneHandle.writing = mpSceneHandle.myIP;
    mpSceneHandle.writing->setBG(HGS::CYAN);

    mpSceneHandle.create = dynamic_cast<HGS::Label*>(
        mpSceneHandle.scene
            .add(HGS::LabelFactory({50.f, 300.f, HGS::LABEL_W, HGS::LABEL_H}, "CREATE", *defont,
                                   HGS::RED, HGS::WHITE))
            ->setCallBack([] {
                // start Server
                activeRole = Roles::Idle;
                udpMan.clearAll();
                if (!udpMan.createRX(mpSceneHandle.myIP->getText(),
                                     mpSceneHandle.rxPort->getText())) {
                    SDL_Log("SERVER CANT CREATE RX\n");
                    activeScene = Scenes::MainMenuScene;
                    return;
                }
                SDL_Log("SERVER RX UP\n");
                if (!udpMan.makeBuffers()) {
                    SDL_Log("MAKEBUFFER!!\n");
                    activeScene = Scenes::MainMenuScene;
                    return;
                }
                SDL_Log("SERVER RX BUFFER UP\n");
                if (!udpMan.createTX(mpSceneHandle.cohIP->getText(),
                                     mpSceneHandle.txPort->getText())) {
                    SDL_Log("CANT CREATE TX\n");
                    activeScene = Scenes::MainMenuScene;
                    return;
                }
                SDL_Log("SERVER TX UP\n");
                rx_ms = SDL_GetTicks();
                tx_ms = SDL_GetTicks();
                activeRole = Roles::Server;
                activeScene = Scenes::PlayScene;
                InitPlaySceneHandle();
            }));

    mpSceneHandle.join = dynamic_cast<HGS::Label*>(
        mpSceneHandle.scene
            .add(HGS::LabelFactory({250.f, 300.f, HGS::LABEL_W, HGS::LABEL_H}, "JOIN", *defont,
                                   HGS::BLUE, HGS::WHITE))
            ->setCallBack([] {
                // start client
                activeRole = Roles::Idle;
                udpMan.clearAll();
                if (!udpMan.createRX(mpSceneHandle.myIP->getText(),
                                     mpSceneHandle.rxPort->getText())) {
                    SDL_Log("CANT CREATE RX\n");
                    activeScene = Scenes::MainMenuScene;
                    return;
                }
                SDL_Log("CLIENT RX UP\n");
                if (!udpMan.makeBuffers()) {
                    SDL_Log("MAKEBUFFER!!\n");
                    activeScene = Scenes::MainMenuScene;
                    return;
                }
                SDL_Log("CLIENT RX BUFFER UP\n");
                if (!udpMan.createTX(mpSceneHandle.cohIP->getText(),
                                     mpSceneHandle.txPort->getText())) {
                    SDL_Log("CANT CREATE TX\n");
                    activeScene = Scenes::MainMenuScene;
                    return;
                }
                SDL_Log("CLIENT TX UP\n");
                rx_ms = SDL_GetTicks();
                tx_ms = SDL_GetTicks();
                activeRole = Roles::Client;
                activeScene = Scenes::PlayScene;
                InitPlaySceneHandle();
            }));

    MP_FIRST_PACKAGE_CAME = false;
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
    playSceneHandle.coh = dynamic_cast<HGS::Paddle*>(playSceneHandle.scene.add(HGS::PaddleFactory(
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
    playSceneHandle.coh_score = 0;
    cnt = 0;
}

void InitFinalSceneHandle(bool w)
{
    finalSceneHandle.scene.clear();
    finalSceneHandle.scene.add(
        HGS::LabelFactory({0.f, 0.f, HGS::SCREEN_W, HGS::SCREEN_H}, (w ? "WIN" : "LOSE"), *defont,
                          (w ? HGS::BLUE : HGS::BLACK), (w ? HGS::WHITE : HGS::RED)));
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
    playSceneHandle.coh->setPos({HGS::PADDLE_R_PX, HGS::PADDLE_PY});
    playSceneHandle.coh->setMove(0.f);
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
void mpRXthread()
{
    while (KEEP_CON) {
        if (activeScene == Scenes::PlayScene) {
            if (activeRole == Roles::Idle) {
                continue;
            }
            auto res{udpMan.peek()};
            if (res == -1) {
                SDL_Log("RX DOWN <MinusOne\n");
                activeRole = Roles::Idle;
                activeScene = Scenes::MainMenuScene;
                continue;
            }
            if (res != 0) {
                rx_ms = SDL_GetTicks();
                MP_FIRST_PACKAGE_CAME = true;
                udpMan.read();
                auto rxBufSlice{udpMan.getRXBufSlice()};
                SDL_Log("GOT %s\n", rxBufSlice.buf);
                std::string_view buf{rxBufSlice.buf};
                size_t ib{0};
                size_t ie{buf.find(";")};
                float coh_y{std::stof(std::string{buf.substr(ib, ie)})};
                playSceneHandle.coh->setPos({HGS::PADDLE_R_PX, coh_y});
                if (activeRole == Roles::Client) {
                    ib = ie + 1;
                    ie = buf.find(";", ib);
                    float ball_x{std::stof(std::string{buf.substr(ib, ie)})};
                    ball_x = HGS::SCREEN_W - ball_x; // mirroring on client side
                    SDL_Log("%d %d %f\n", ib, ie, ball_x);
                    ib = ie + 1;
                    ie = buf.find(";", ib);
                    float ball_y{std::stof(std::string{buf.substr(ib, ie)})};
                    playSceneHandle.ball->setPos({ball_x, ball_y});

                    ib = ie + 1;
                    ie = buf.find(";", ib);
                    int scores{std::stoi(std::string{buf.substr(ib, ie)})};
                    playSceneHandle.player_score = scores % 10;
                    playSceneHandle.coh_score = scores / 10;

                    ib = ie + 1;
                    ie = buf.find(";", ib);
                    int min{std::stoi(std::string{buf.substr(ib, ie)})};
                    ib = ie + 1;
                    ie = buf.find(";", ib);
                    int sec{std::stoi(std::string{buf.substr(ib, ie)})};
                    SDL_Log("%d %d\n", min, sec);
                    playSceneHandle.min = min;
                    playSceneHandle.sec = sec;
                }
                udpMan.clearRXBuf();
            }
            SDL_Log("%d\n", (SDL_GetTicks() - rx_ms));
            if (MP_FIRST_PACKAGE_CAME && ((SDL_GetTicks() - rx_ms) > 10000)) {
                SDL_Log("TIMEOUT\n");
                MP_FIRST_PACKAGE_CAME = false;
                activeRole = Roles::Idle;
                activeScene == Scenes::MainMenuScene;
            }
        }
    }
    SDL_Log("KEEP_CON DEAD, RX THREAD JOINS\n");
}
void mpTXthread()
{
    std::stringstream tx_parser;
    while (KEEP_CON) {
        if (activeScene == Scenes::PlayScene) {
            if (activeRole == Roles::Idle) {
                continue;
            }
            tx_parser.str("");
            tx_parser.clear();
            tx_parser << std::setprecision(5);
            tx_parser << playSceneHandle.player->getPos().y;
            tx_parser << ";";
            if (activeRole == Roles::Server) {
                tx_parser << playSceneHandle.ball->getPos().x;
                tx_parser << ";";
                tx_parser << playSceneHandle.ball->getPos().y;
                tx_parser << ";";
                tx_parser << playSceneHandle.player_score;
                tx_parser << playSceneHandle.coh_score;
                tx_parser << ";";
                tx_parser << playSceneHandle.min;
                tx_parser << ";";
                tx_parser << playSceneHandle.sec;
                tx_parser << ";";
            }
            udpMan.transmit(tx_parser.str());
            while ((SDL_GetTicks() - tx_ms) < (1000 / 60)) {
                ;
            }
            tx_ms = SDL_GetTicks();
        }
    }
    SDL_Log("KEEP_CON DEAD, TX THREAD JOINS\n");
}
/**
 * set move player
 */
void MovePlayerPaddle()
{
    if (press_up) {
        playSceneHandle.player->setMove(-HGS::PADDLE_SPD);
        if (auto p{playSceneHandle.player->getPos()}; p.y < 0.f) {
            p.y = 0.f;
            playSceneHandle.player->setPos(p);
        }
    } else if (press_down) {
        playSceneHandle.player->setMove(HGS::PADDLE_SPD);
        if (auto p{playSceneHandle.player->getPos()}; p.y > (HGS::SCREEN_H - HGS::PADDLE_H)) {
            p.y = (HGS::SCREEN_H - HGS::PADDLE_H);
            playSceneHandle.player->setPos(p);
        }
    } else {
        playSceneHandle.player->setMove(0.f);
    }
}
/**
 * set move coh
 */
void MoveCOHPaddle()
{
    const auto ball_y{playSceneHandle.ball->getPos().y};
    const auto coh_y{playSceneHandle.coh->getCenPos().y};
    const auto dy{coh_y - ball_y};
    if (dy < 0.f) {
        playSceneHandle.coh->setMove(HGS::PADDLE_SPD);

    } else if (dy > 0.f) {
        playSceneHandle.coh->setMove(-HGS::PADDLE_SPD);
    } else {
        playSceneHandle.coh->setMove(0.f);
    }
    if (auto p{playSceneHandle.coh->getPos()}; p.y < 0.f) {
        p.y = 0.f;
        playSceneHandle.coh->setPos(p);
    } else if (auto p{playSceneHandle.coh->getPos()}; p.y > (HGS::SCREEN_H - HGS::PADDLE_H)) {
        p.y = (HGS::SCREEN_H - HGS::PADDLE_H);
        playSceneHandle.coh->setPos(p);
    }
}
/**
 * move ball
 */
void MoveBall()
{
    const auto [bx, by]{playSceneHandle.ball->getPos()};
    if (bx < 0.f) {
        playSceneHandle.coh_score += 1;
        NextRoundPlaySceneActors();
    } else if (bx > HGS::SCREEN_W) {
        playSceneHandle.player_score += 1;
        NextRoundPlaySceneActors();
    } else if ((by < 0.f) || (by > (HGS::SCREEN_H - playSceneHandle.ball->getRadius()))) {
        playSceneHandle.ball->velYinverse();
    }
    if (bx < HGS::SCREEN_W / 2.f) {
        const auto ball_r{playSceneHandle.ball->getRadius()};
        const auto [x, y]{playSceneHandle.player->getCenPos()};
        const auto dx{x - bx};
        const auto dy{y - by};
        if (dx > (-5.f - ball_r) && dx < (5.f + ball_r)) {
            if (dy > -(HGS::PADDLE_H / 2.f + ball_r) && dy < (HGS::PADDLE_H / 2.f + ball_r)) {
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
        const auto [x, y]{playSceneHandle.coh->getCenPos()};
        const auto dx{x - bx};
        const auto dy{y - by};
        if (dx > (-5.f - ball_r) && dx < (5.f + ball_r)) {
            if (dy > -(HGS::PADDLE_H / 2.f + ball_r) && dy < (HGS::PADDLE_H / 2.f + ball_r)) {
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
 * update scores
 */
void UpdateScores()
{
    parser.str("");
    parser.clear();
    parser << playSceneHandle.player_score;
    parser << " : ";
    parser << playSceneHandle.coh_score;
    playSceneHandle.scores->setText(parser.str());
}
/**
 * update clock
 */
void UpdateClock()
{
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
}
/**
 * **********
 * MAIN ENTRY
 * **********
 */
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    parser.str("");
    parser.clear();
    ARGC = argc;
    ARGV = argv;
    activeRole = Roles::Idle;
    activeScene = Scenes::MainMenuScene;
    std::jthread rxThreadHandle(mpRXthread);
    std::jthread txThreadHandle(mpTXthread);
#ifdef UDPNS_WINDOWS
    if (!UDPNS::initWSA()) {
        std::cerr << "WSA DOES NOT INIT, terminating...\n";
        return -2;
    }
#endif

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
                    if (mainMenuHandle.spBtn->isContains(mx, my)) {
                        mainMenuHandle.spBtn->call2back();
                    } else if (mainMenuHandle.mpBtn->isContains(mx, my)) {
                        mainMenuHandle.mpBtn->call2back();
                    } else if (mainMenuHandle.extBtn->isContains(mx, my)) {
                        mainMenuHandle.extBtn->call2back();
                    }
                } else if (activeScene == Scenes::MPScene) {
                    if (mpSceneHandle.myIP->isContains(mx, my)) {
                        mpSceneHandle.myIP->call2back();
                    } else if (mpSceneHandle.rxPort->isContains(mx, my)) {
                        mpSceneHandle.rxPort->call2back();
                    } else if (mpSceneHandle.cohIP->isContains(mx, my)) {
                        mpSceneHandle.cohIP->call2back();
                    } else if (mpSceneHandle.txPort->isContains(mx, my)) {
                        mpSceneHandle.txPort->call2back();
                    } else if (mpSceneHandle.create->isContains(mx, my)) {
                        mpSceneHandle.create->call2back();
                    } else if (mpSceneHandle.join->isContains(mx, my)) {
                        mpSceneHandle.join->call2back();
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
                } else if ((k == SDLK_UP) || (k == SDLK_W)) {
                    press_up = false;
                } else if ((k == SDLK_DOWN) || (k == SDLK_S)) {
                    press_down = false;
                } else if (auto i{kInWrite(k)}; i != std::string::npos) {
                    if (activeScene == Scenes::MPScene) {
                        parser.str("");
                        parser.clear();
                        parser << mpSceneHandle.writing->getText();
                        parser << write_c[i];
                        mpSceneHandle.writing->setText(parser.str());
                    }
                } else if (k == SDLK_BACKSPACE) {
                    if (activeScene == Scenes::MPScene) {
                        std::string s{mpSceneHandle.writing->getText()};
                        mpSceneHandle.writing->setText(s.substr(0, s.size() - 1));
                    }
                }
            } else if (e.type == SDL_EVENT_KEY_DOWN) {
                const auto& k{e.key.key};
                if (activeScene == Scenes::PlayScene) {
                    if ((k == SDLK_UP) || (k == SDLK_W)) {
                        press_up = true;
                    } else if ((k == SDLK_DOWN) || (k == SDLK_S)) {
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
        } else if (activeScene == Scenes::MPScene) {
            mpSceneHandle.scene.render(r);
        } else if (activeScene == Scenes::FinalScene) {
            finalSceneHandle.scene.render(r);
        } else if (activeScene == Scenes::PlayScene) {
            /**
             * update render stuff
             */
            if (activeRole == Roles::Idle) {
                playSceneHandle.player->step(1.f);
                playSceneHandle.ball->step(1.f);
                playSceneHandle.coh->step(1.f);
            } else if (MP_FIRST_PACKAGE_CAME) {
                playSceneHandle.player->step(1.f);
                if (activeRole != Roles::Client) {
                    playSceneHandle.ball->step(1.f);
                    playSceneHandle.coh->step(1.f);
                }
            }
            
            UpdateClock();

            if (playSceneHandle.player_score == 3) {
                // player win
                activeScene = Scenes::FinalScene;
                InitFinalSceneHandle(true);
            } else if (playSceneHandle.coh_score == 3) {
                // coa win
                activeScene = Scenes::FinalScene;
                InitFinalSceneHandle(false);
            }

            UpdateScores();

            MovePlayerPaddle();
            /**
             * set move coa
             */
            if (activeRole == Roles::Idle) {
                MoveCOHPaddle();
            }

            /**
             * set vel ball
             */
            if (activeRole != Roles::Client) {
                MoveBall();
            }

            /**
             * render finalized screen
             */
            playSceneHandle.scene.render(r);
        }
        SDL_RenderPresent(r);

        while ((SDL_GetTicks() - milisecs) < (1000 / 60)) {
            ;
        }
        milisecs = SDL_GetTicks();
        if (activeRole == Roles::Idle) {
            ++cnt;
        } else if (MP_FIRST_PACKAGE_CAME) {
            ++cnt;
        }

        if (activeRole != Roles::Client) {
            playSceneHandle.min = cnt / 3600;
            playSceneHandle.sec = (cnt / 60) % 60;
        }
    }
    /**
     * add custom exit function via std
     */
    KEEP_CON = false;
    playSceneHandle.scene.clear();
    mpSceneHandle.scene.clear();
    mainMenuHandle.scene.clear();
    HGS::ENG::deinit();
#ifdef UDPNS_WINDOWS
    UDPNS::stopWSA();
#endif
    return 0;
}