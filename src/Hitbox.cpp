#include "HitboxPerformance/Hitbox.hpp"
#include "HitboxPerformance/RendererState.hpp"

float AABB::x1() { 
    return m_x; 
}

float AABB::x2() { 
    return m_x + m_w; 
}

float AABB::w() { 
    return m_w; 
}

float AABB::y1() { 
    return m_y; 
}

float AABB::y2() { 
    return m_y + m_h; 
}

float AABB::h() { 
    return m_h; 
}

void AABB::move(float x, float y) {
    m_x += x;
    m_y += y;
}

bool AABB::collision(AABB& other) {
    return x2() > other.x1() && 
           x1() < other.x2() && 
           y2() > other.y1() && 
           y1() < other.y2();
}

AABB& Hitbox::bounding_box() {
    return m_bounding_box;
}

bool Hitbox::bounding_box_collision(Hitbox& other) {
    return m_bounding_box.collision(other.bounding_box());
}

void Hitbox::move(float x, float y) {
    m_bounding_box.move(x, y);
}


void Hitbox::draw_bounding_box(SDL_Color& color) {
    RendererState& state = RendererState::instance();
    SDL_FRect rect = {
        .x = m_bounding_box.x1(),
        .y = m_bounding_box.y1(),
        .w = m_bounding_box.w(),
        .h = m_bounding_box.h(),
    };
    SDL_SetRenderDrawColor(state.get_renderer(), color.r, color.g, color.b, color.a);
    SDL_RenderRect(state.get_renderer(), &rect);
}

void Hitbox::draw_hitbox(SDL_Color& color) {

}