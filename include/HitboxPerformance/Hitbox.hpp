#include <SDL3/SDL.h>
#pragma once
#include <array>

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
    float w();
    float h();

    void move(float x, float y);

    bool collision(AABB& other);
};

struct Vec2 {
    float x, y;
};

class Hitbox {
    AABB m_bounding_box;
public:
    Hitbox(float x, float y, float w, float h) : m_bounding_box(x, y, w, h) {};
    AABB& bounding_box();
    bool bounding_box_collision(Hitbox& other);
    void move(float x, float y);

    // Draw wireframe bounding box with a given colour
    void draw_bounding_box(SDL_Color& color);

    // Draw wireframe of the actual hitbox
    void draw_hitbox(SDL_Color& color);

    std::array<Vec2, 4> get_vertices() { //fried my brain thinking about polygons, so I simplified it to rectangles like the AABB can change later
        float x = m_bounding_box.x1();
        float y = m_bounding_box.y1();
        float w = m_bounding_box.w();
        float h = m_bounding_box.h();

        return {{
            {x, y},         // top-left
            {x + w, y},     // top-right
            {x + w, y + h}, // bottom-right
            {x, y + h}      // bottom-left - maybe double check these, should be in clockwise order
        }};
    }

    bool sat_collision(Hitbox& other); //impl in Hitbox.cpp
};