#include <spdlog/spdlog.h>
#include <SDL3/SDL.h>
#include "HitboxPerformance/RendererState.hpp"
#include "HitboxPerformance/Constants.hpp"
#include "HitboxPerformance/App.hpp"

int main(int argc, const char *argv[]) {
    RendererState &renderer = RendererState::instance();
    SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO);
    
    // Create and error check window
    SDL_Window *window = SDL_CreateWindow(
        Constants::WINDOW_TITLE, 
        Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT, 
        0
    );
    
    if (!window) {
        spdlog::error("Failed to create SDL window, SDL error: {}", SDL_GetError());
        return 1;
    }

    // Create and error check renderer
    SDL_Renderer *sdl_renderer = SDL_CreateRenderer(window, nullptr);

    if (!sdl_renderer) {
        spdlog::error("Failed to create SDL renderer, SDL error: {}", SDL_GetError());
        return 1;
    }

    spdlog::info("Successfully created window and renderer.");

    renderer.setup(window, sdl_renderer);

    HitboxApp::create();

    bool keep_running = true;
    while (keep_running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                spdlog::info("Quit signal received.");
                keep_running = false;
            }
        }

        SDL_RenderClear(renderer.get_renderer());

        if (!HitboxApp::loop())
            keep_running = false;

        SDL_RenderPresent(renderer.get_renderer());
    }
    
    spdlog::info("Cleaning up.");
    HitboxApp::cleanup();
    renderer.cleanup();
    SDL_Quit();
    spdlog::info("Done.");
    return 0;
}