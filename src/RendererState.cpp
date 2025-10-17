#include "HitboxPerformance/RendererState.hpp"

SDL_Renderer *RendererState::get_renderer() {
    return m_renderer;
}

SDL_Window *RendererState::get_window() {
    return m_window;
}

void RendererState::setup(SDL_Window *window, SDL_Renderer *renderer) {
    m_renderer = renderer;
    m_window = window;
}

void RendererState::cleanup() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
}
