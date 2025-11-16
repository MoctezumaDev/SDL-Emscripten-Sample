#include <iostream>
#include <fmt/color.h>
#include <fmt/core.h>
#include <SDL3/SDL.h>
#include <CLI/CLI.hpp>

//#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
//#endif

static SDL_Window* window;
static SDL_Renderer* renderer;
static bool running = true;

void main_loop(void) {
    SDL_Event event;
    SDL_Log("LOOP");
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                running = false;
                #ifdef __EMSCRIPTEN__
                emscripten_cancel_main_loop();  // stop when user closes
                #endif
                break;
            case SDL_EVENT_KEY_DOWN:
                SDL_Log("a key was pressed: %d", event.key.key);
                break;
            default:
                break;
        }
    }

    // Example draw
    SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

int main(int argc, char** argv)
{
    /*CLI::App app{"3dentt"};

    std::string name;

    app.add_option("-n,--name", name, "Your name");

    CLI11_PARSE(app, argc, argv);

#ifndef __EMSCRIPTEN__
    // If no name was provided, show help
    if (name.empty()) {
        std::cout << app.help() << std::endl;
        return 0;
    }

    std::cout << "Hello " << name << std::endl;
#endif

    fmt::print("Such a relief when it works!\n");

    auto msg = fmt::format(fmt::fg(fmt::color::red), "This is a red message!\n");
    std::cout << msg << std::endl;*/

     // Set the hint BEFORE window creation or SDL_Init
    SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#canvas");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "%s", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow("3Dentt", 640, 480, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, nullptr);

//#ifdef __EMSCRIPTEN__
    // The browser controls the loop now
    emscripten_set_main_loop(main_loop, 0, 1);
//#else
    // Native fallback (for local testing)
//    while (running) {
//        main_loop();
//        SDL_Delay(16);
//    }
//#endif

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}