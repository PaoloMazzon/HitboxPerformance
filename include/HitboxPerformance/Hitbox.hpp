#include <SDL3/SDL.h>
#pragma once

class AABB {
    float m_x;
    float m_y;
    float m_w;
    float m_h;
public:
    AABB(float x, float y, float w, float h) 
        : m_x(x), m_y(y), m_w(w), m_h(h) {}

    float x1();
    float x2();
    float y1();
    float y2();

    bool collision(AABB& other);
};

class Hitbox {
    AABB m_bounding_box;
public:
    Hitbox(float x, float y, float w, float h) : m_bounding_box(x, y, w, h) {};
    AABB& bounding_box();
    bool bounding_box_collision(Hitbox& other);

    // Draw wireframe bounding box with a given colour
    void draw_bounding_box(SDL_Color& color);

    // Draw wireframe of the actual hitbox
    void draw_hitbox(SDL_Color& color);
};