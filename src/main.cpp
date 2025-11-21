#include <iostream>
#include <stdexcept>
#include <string>
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

void main_start_loop()
{
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
}

void main_finalize()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void main_initialize()
{
    // Set the hint BEFORE window creation or SDL_Init
    SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#canvas");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error(
            std::string("SDL_Init failed: ") + SDL_GetError()
        );
    }
    SDL_Log("Success");

    window = SDL_CreateWindow("App", 640, 480, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, nullptr);
}

int main(int argc, char** argv)
{
    try
    {
        main_initialize();
        main_start_loop();
        main_finalize();
    }
    catch (const std::exception& e) {
        std::cerr <<  e.what() << "\n";
        return 1;
    }
    
    return 0;
}