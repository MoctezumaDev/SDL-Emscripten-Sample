#include <iostream>
#include <fmt/color.h>
#include <fmt/core.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <CLI/CLI.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

static SDL_Window* window;
static SDL_Renderer* renderer;
static bool running = true;

void main_loop(void) {
    SDL_Event event;

    fflush(stdout);

    // Example draw
    SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

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
}

int main(int argc, char** argv)
{
    // Set the hint BEFORE window creation or SDL_Init
    SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#canvas");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_Log("Success");

    window = SDL_CreateWindow("App", 640, 480, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, nullptr);

#ifdef __EMSCRIPTEN__
    // The browser controls the loop now
    emscripten_set_main_loop(main_loop, 0, 1);
#else
    // Native fallback (for local testing)
    while (running) {
        main_loop();
        SDL_Delay(16);
    }
#endif

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}