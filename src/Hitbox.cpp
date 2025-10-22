#include "HitboxPerformance/Hitbox.hpp"

float AABB::x1() { 
    return m_x; 
}

float AABB::x2() { 
    return m_x + m_w; 
}

float AABB::y1() { 
    return m_y; 
}

float AABB::y2() { 
    return m_y + m_h; 
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


void Hitbox::draw_bounding_box(SDL_Color& color) {

}

void Hitbox::draw_hitbox(SDL_Color& color) {

}