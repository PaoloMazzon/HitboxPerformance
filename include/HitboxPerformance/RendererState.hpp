#include <SDL3/SDL.h>
#pragma once

/// \brief RendererState is the singleton for renderer information, given
///        that a single program may not have more than 1 renderer.
class RendererState {
    private:
        RendererState() {}

        SDL_Window *m_window;
        SDL_Renderer *m_renderer;

    public:
        static RendererState& instance() {
            static RendererState instance;
            return instance;
        }

        RendererState(RendererState const&) = delete;
        void operator=(RendererState const&) = delete;

        // Getters
        SDL_Renderer *get_renderer();
        SDL_Window *get_window();

        // Initial setup
        void setup(SDL_Window *window, SDL_Renderer *renderer);

        // Call before SDL_Quit
        void cleanup();
};