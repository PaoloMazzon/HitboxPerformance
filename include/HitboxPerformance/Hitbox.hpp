#include <SDL3/SDL.h>
#include <vector>
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
    std::vector<Vec2> m_vertices;
public:
    Hitbox(float x, float y, float w, float h) : m_bounding_box(x, y, w, h) {};

    // The point of this contructor is to find the min and max points for a bounding box
    Hitbox(std::vector<Vec2> vertices) : m_vertices(vertices), m_bounding_box(0, 0, 0, 0) {
        float x_max = 0;
        float x_min = 999;
        float y_max = 0;
        float y_min = 999;
        for (auto vertex: vertices) {
            if (vertex.x < x_min) x_min = vertex.x;
            if (vertex.x > x_max) x_max = vertex.x;
            if (vertex.y < y_min) y_min = vertex.y;
            if (vertex.y > y_max) y_max = vertex.y;
        }
        m_bounding_box = AABB(x_min, y_min, x_max - x_min, y_max - y_min);
    };

    AABB& bounding_box();
    bool bounding_box_collision(Hitbox& other);
    void move(float x, float y);

    // Draw wireframe bounding box with a given colour
    void draw_bounding_box(SDL_Color& color);

    // Draw wireframe of the actual hitbox
    void draw_hitbox(SDL_Color& color);

    std::vector<Vec2>& get_vertices();

    bool sat_collision(Hitbox& other);
};