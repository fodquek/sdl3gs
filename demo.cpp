// /* Headers */
#include <SDL3/SDL.h>
// #include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string_view>
#include <sys/types.h>
#include <vector>

#include "ceng.h"
#include "cwindow.h"
#include "crenderer.h"
#include "cbox.h"

/**
 * All errors
 */
namespace HGS
{
    enum class RC : int
    {
        OK = 0,
        SDL_ERR_INIT,
        SDL_ERR_WIN_INIT,
        SDL_ERR_REN_INIT,
        SDL_ERR_VSYNC_SET
    };

    const int VSYNC_ON {1};
    const int VSYNC_OFF {0};
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    if (!HGS::ENG::init())
    {
        std::cerr << "SDL_CANT_INIT\n";
        return static_cast<int>(HGS::RC::SDL_ERR_INIT);
    }

    HGS::Window w {"demo", {600, 480}};
    HGS::Renderer r {&w};
    // renderer clear color?
    HGS::ENG::VSYNC(r, HGS::VSYNC_ON);

    bool main_loop {true};
    SDL_Event e{};
    float mx;
    float my;
    const SDL_FRect g {50.f, 100.f, 100.f, 200.f};
    HGS::Widget* box  {new HGS::Box(static_cast<SDL_FRect>(g))};
    HGS::Widget* box2 {new HGS::Box(static_cast<SDL_FRect>(g))};
    while (main_loop)
    {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
            {
                main_loop = false;
            }
            else if (e.type == SDL_EVENT_KEY_UP)
            {
                auto k {e.key.key};
                if (k == SDLK_Q || k == SDLK_ESCAPE)
                {
                    main_loop = false;
                }
            }
            else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                SDL_GetMouseState(&mx, &my);
                if (box->isContains(mx, my))
                {
                    dynamic_cast<HGS::Box*>(box)->setBG(
                        {0x00, 0x00, 0xff, 0xff}
                    );
                }
                else
                {
                    box->setPos({mx, my});
                    dynamic_cast<HGS::Box*>(box)->setBG(
                        {0xff, 0x00, 0x00, 0xff}
                    );
                }
            }
            
            SDL_SetRenderDrawColor(r, 0x00, 0x00, 0x00, 0xff);
            SDL_RenderClear(r);
            if (dynamic_cast<HGS::Box*>(box)->getBG().b == 0xff)
            {
                box2->render(r);
                box->render(r);
            }
            else
            {
                box->render(r);
                box2->render(r);
            }
            SDL_RenderPresent(r);
        }
    }

    delete box;
    delete box2;
    HGS::ENG::deinit();
    return 0;
}

// class MainWindow {
//     using Window = HGS::Window;
//     using Renderer = HGS::Renderer;
//     using Widget = HGS::Widget;
// public:
//     explicit MainWindow(const std::string_view title, const SDL_Rect& geo, const SDL_Color& color)
//     : width{ geo.w }, height{ geo.h }, color{ color } {
//         if (width < 320) {
//             SDL_Log("MW WIDTH %d!\n", width);
//         }
//         if (height < 240) {
//             SDL_Log("MW HEIGHT %d!\n", height);
//         }
//         if (!SDL_Init(0)) {
//             SDL_Log("SDL INIT OLMADIKE: %s", SDL_GetError());
//         }
//         if (window = new Window(title, { width, height }); !window) {
//             SDL_Log("CANT CREATE ©Window IN ©MW : %s\n", SDL_GetError());
//         }
//         if (renderer = new Renderer(window); !renderer) {
//             SDL_Log("CANT CREATE ©Renderer IN ©MW : %s\n", SDL_GetError());
//         }
//         SetRenderDrawColor(color);
//         if (!SDL_SetRenderVSync(*renderer, 1)) {
//             SDL_Log("CANT ENABLE VSYNC: %s", SDL_GetError());
//             std::abort();
//         }
//         if (!TTF_Init()) {
//             SDL_Log("CANT TTF_Init() !!!\n");
//         }
//     }

//     ~MainWindow() {
//         delete renderer;
//         renderer = nullptr; // ?mandatory?
//         delete window;
//         window = nullptr; // ?mandatory?
//         TTF_Quit();
//         SDL_Quit();
//     }
//     MainWindow(const MainWindow& mw)           = delete;
//     MainWindow operator=(const MainWindow& mw) = delete;
//     operator SDL_Renderer*() {
//         return *renderer;
//     }

//     void SetRenderDrawColor(const SDL_Color color) {
//         SDL_SetRenderDrawColor(*renderer, color.r, color.g, color.b, color.a); // magenta ff00ff
//         this->color = color;
//     }

//     void SetRenderDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
//         SDL_SetRenderDrawColor(*renderer, r, g, b, a); // magenta ff00ff
//         color.r = r;
//         color.g = g;
//         color.b = b;
//         color.a = a;
//     }

//     void RenderClear() {
//         SDL_RenderClear(*renderer);
//     }

//     void RenderWidget(Widget* widget) {
//         auto dstsrc{ widget->getGeo() };
//         SDL_RenderTexture(*renderer, *widget, nullptr, &dstsrc);
//     }

//     void RenderPresent() {
//         SDL_RenderPresent(*renderer);
//     }

// private:
//     int width{ 320 };
//     int height{ 240 };
//     Window* window{ nullptr };
//     Renderer* renderer{ nullptr };
//     SDL_Color color{};
// };

// class Paddle : public HGS::Widget {
// public:
//     void setVel(int v) {
//         vel = v;
//     }
//     int getVel() {
//         return vel;
//     }

// private:
//     int vel{ 0 };
// };

// /*
//  * Scenes
//  */
// enum SCENE_NAMES {
//     SCENE_MAIN = 0, // 0
//     SCENE_ONLINE,   // 1
//     SCENE_OFFLINE,  // 2
//     SCENE_CONFIG,   // 3
//     SCENE_EXIT      // 4
// };

// enum MAIN_MENU_WIDGETS { MAIN_ONLINE, MAIN_OFFLINE, MAIN_CONFIG, MAIN_EXIT };

// enum OFFLINE_WIDGETS {
//     OFFLINE_SCORE,
//     OFFLINE_PLAYER,
//     OFFLINE_AI,
//     OFFLINE_BALL
// };

// enum EXIT_MENU_WIDGETS { EXIT_LOGO, EXIT_YES, EXIT_NO };

// /*
//  * Main entrance
//  */
// int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
//     using Widget = HGS::Widget;
//     MainWindow mw{ "AdanaMerkez", { 0, 0, 1280, 720 }, { 0X44, 0xAA, 0X44, 0XFF } };

//     size_t active_scene{ 0 };
//     bool offline_setup_game{ false };
//     bool offline_game_on{ false };
//     int player_score{ 0 };
//     int other_score{ 0 };

//     std::vector<std::vector<Widget*>> scenes{
//         { new Widget(50.f, 325.f), new Widget(250.f, 325.f),
//           new Widget(50.f, 400.f), new Widget(250.f, 400.f) },
//         {},
//         { new Widget(600.f, 10.f), new Widget(20.f, 300.f),
//           new Widget(1240.f, 300.f), new Widget(640.f, 360.f) },
//         {},
//         { new Widget(150.f, 200.f), new Widget(50.f, 400.f), new Widget(250.f, 400.f) },
//     };
//     scenes.at(SCENE_MAIN).at(MAIN_ONLINE)->LoadFromImage("./assets/images/create_btn.png", mw);
//     scenes.at(SCENE_MAIN).at(MAIN_OFFLINE)->LoadFromImage("./assets/images/create_btn.png", mw);
//     scenes.at(SCENE_MAIN).at(MAIN_CONFIG)->LoadFromImage("./assets/images/join_btn.png", mw);
//     scenes.at(SCENE_MAIN).at(MAIN_EXIT)->LoadFromImage("./assets/images/quit_btn.png", mw);

//     scenes.at(SCENE_OFFLINE).at(OFFLINE_PLAYER)->LoadFromImage("./assets/images/player_paddle.png", mw);
//     scenes.at(SCENE_OFFLINE).at(OFFLINE_AI)->LoadFromImage("./assets/images/ai_paddle.png", mw);
//     scenes.at(SCENE_OFFLINE).at(OFFLINE_BALL)->LoadFromImage("./assets/images/ball.png", mw);

//     scenes.at(SCENE_EXIT).at(EXIT_LOGO)->LoadFromImage("./assets/images/quit_btn.png", mw);
//     scenes.at(SCENE_EXIT).at(EXIT_YES)->LoadFromImage("./assets/images/yes_btn.png", mw);
//     scenes.at(SCENE_EXIT).at(EXIT_NO)->LoadFromImage("./assets/images/no_btn.png", mw);

//     SDL_Event event{};
//     bool finito{ false };
//     float mouse_x{};
//     float mouse_y{};
//     constexpr float PLAYER_VEL = 10.f;
//     while (!finito) {
//         while (SDL_PollEvent(&event)) {
//             switch (event.type) {

//             case SDL_EVENT_QUIT: finito = true; break;

//             case SDL_EVENT_KEY_UP:
//                 switch (event.key.key) {
//                 case SDLK_Q:
//                     std::cerr << "Q\n";
//                     finito = true;
//                     break;
//                 }
//                 break;

//             case SDL_EVENT_KEY_DOWN:
//                 switch (event.key.key) {
//                 case SDLK_ESCAPE:
//                     offline_game_on = false;
//                     active_scene    = SCENE_MAIN;
//                     break;

//                 case SDLK_UP: {
//                     SDL_FRect next_pos{
//                         scenes.at(SCENE_OFFLINE).at(OFFLINE_PLAYER)->getGeo()
//                     };
//                     next_pos.y -= PLAYER_VEL;
//                     if (next_pos.y < 0.f) {
//                         next_pos.y = 0.f;
//                     }
//                     scenes.at(SCENE_OFFLINE).at(OFFLINE_PLAYER)->setGeo(next_pos);
//                 } break;

//                 case SDLK_DOWN: {
//                     Widget* w{ scenes.at(SCENE_OFFLINE).at(OFFLINE_PLAYER) };
//                     SDL_FRect next_pos{ w->getGeo() };
//                     next_pos.y += PLAYER_VEL;
//                     if (next_pos.y > 720.f -
//                         static_cast<float>(static_cast<SDL_Texture*>(*w)->h)) {
//                         next_pos.y = 720.f -
//                         static_cast<float>(static_cast<SDL_Texture*>(*w)->h);
//                     }
//                     scenes.at(SCENE_OFFLINE).at(OFFLINE_PLAYER)->setGeo(next_pos);
//                 } break;
//                 }
//                 break;

//             case SDL_EVENT_MOUSE_BUTTON_DOWN:
//                 SDL_GetMouseState(&mouse_x, &mouse_y);
//                 std::cout << "( " << mouse_x << " , " << mouse_y << " )\n";
//                 switch (active_scene) {
//                 case SCENE_MAIN:
//                     if (scenes.at(SCENE_MAIN).at(MAIN_EXIT)->isPointIn(mouse_x, mouse_y)) {
//                         active_scene = SCENE_EXIT;
//                         break;
//                     }
//                     if (scenes.at(SCENE_MAIN).at(MAIN_OFFLINE)->isPointIn(mouse_x, mouse_y)) {
//                         active_scene       = SCENE_OFFLINE;
//                         offline_setup_game = true;
//                         break;
//                     }
//                     break;

//                 case SCENE_OFFLINE:
//                     break;
//                     // esc basarsak ana menu atyo

//                 case SCENE_EXIT:
//                     if (scenes.at(SCENE_EXIT).at(EXIT_NO)->isPointIn(mouse_x, mouse_y)) {
//                         active_scene = SCENE_MAIN;
//                         break;
//                     }
//                     if (scenes.at(SCENE_EXIT).at(EXIT_YES)->isPointIn(mouse_x, mouse_y)) {
//                         finito = true;
//                         break;
//                     }
//                     break;
//                 }
//             }
//         }

//         mw.RenderClear();
//         if (offline_setup_game) {
//             player_score       = 0;
//             other_score        = 0;
//             offline_setup_game = false;
//             offline_game_on    = true;
//         } else if (offline_game_on) {
//             std::stringstream ss;
//             ss << player_score;
//             ss << " - ";
//             ss << other_score;
//             scenes.at(SCENE_OFFLINE)
//             .at(OFFLINE_SCORE)
//             ->loadFromTTF("./assets/fonts/JosyWine-G33rg.ttf", ss.str(),
//                           { 0x00, 0x00, 0x00, 0xFF }, 36, mw);
//         }
//         for (auto w : scenes.at(active_scene)) {
//             mw.RenderWidget(w);
//         }
//         mw.RenderPresent();
//     }
//     for (auto s : scenes) {
//         for (auto w : s) {
//             delete w;
//         }
//     }
//     return 0;
// }

/*
memcpy, memset olsa güzel olur? 20:42_07.02.2025
*/

/*
==19710== HEAP SUMMARY:
==19710==     in use at exit: 296,106 bytes in 3,345 blocks
==19710==   total heap usage: 99,169 allocs, 95,824 frees, 28,909,046 bytes allocated
==19710==
==19710== LEAK SUMMARY:
==19710==    definitely lost: 1,717 bytes in 29 blocks
==19710==    indirectly lost: 2,157 bytes in 12 blocks
==19710==      possibly lost: 0 bytes in 0 blocks
==19710==    still reachable: 292,232 bytes in 3,304 blocks
==19710==         suppressed: 0 bytes in 0 blocks
==19710== Rerun with --leak-check=full to see details of leaked memory
==19710==
==19710== For lists of detected and suppressed errors, rerun with: -s
==19710== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
*/

/*

herşeyi widget üzerinden yapıyoz ama saçma. nihahi hedef buton label paddle vb. olmalı
game state i düşnmek lazım, artık diğer pong oyunlarının kodlarını okumak lazım
widget ın şuanki hali berbat, font we texture lar kendileri bağımsız varlık olmalı VE gerekli widgetlar ile eşleşitiirlmeli
event handling sahne ve widget bazlı olmalı

*/
