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
    // move all the vertices by the offset
    for (int i = 0; i < m_vertices.size(); i++) {
        m_vertices[i].x += x;
        m_vertices[i].y += y;
    }

    // move the bounding box as well
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
    RendererState& state = RendererState::instance();
    SDL_SetRenderDrawColor(state.get_renderer(), color.r, color.g, color.b, color.a);
    for (int i = 0; i < m_vertices.size(); i++) {
        const Vec2& start = m_vertices.at(i);
        const Vec2& end = m_vertices.at((i + 1) % m_vertices.size());
        SDL_RenderLine(state.get_renderer(), start.x, start.y, end.x, end.y);
    }
}

float dot(const Vec2& a, const Vec2& b) {
    return a.x * b.x + a.y * b.y;
}

void project_polygon(const std::vector<Vec2>& vertices, const Vec2& axis, float& min, float& max) {
    min = max = dot(vertices[0], axis);
    for (int i = 1; i < vertices.size(); i++) {
        float proj = dot(vertices[i], axis);
        if (proj < min) min = proj;
        if (proj > max) max = proj;
    }
}

bool overlap_on_axis(const std::vector<Vec2>& vertsA, const std::vector<Vec2>& vertsB, const Vec2& axis) {
    float minA, maxA, minB, maxB;
    project_polygon(vertsA, axis, minA, maxA);
    project_polygon(vertsB, axis, minB, maxB);
    return !(maxA < minB || maxB < minA);
}

bool Hitbox::sat_collision(Hitbox& other) {
    auto vertsA = get_vertices();
    auto verts_a_len = vertsA.size();
    auto vertsB = other.get_vertices();
    auto verts_b_len = vertsB.size();
    std::vector<Vec2> axes;

    // edges of A
    for (int i = 0; i < verts_a_len; i++) {
        Vec2 edge = { vertsA[(i + 1) % verts_a_len].x - vertsA[i].x, vertsA[(i + 1) % verts_a_len].y - vertsA[i].y };
        axes.push_back({ -edge.y, edge.x }); // perpendicular
    }

    // check all axes
    for (int i = 0; i < verts_a_len; i++) {
        if (!overlap_on_axis(vertsA, vertsB, axes[i])) return false;
    }

    // edges of B
    for (int i = 0; i < verts_b_len; i++) {
        Vec2 edge = { vertsB[(i + 1) % verts_b_len].x - vertsB[i].x, vertsB[(i + 1) % verts_b_len].y - vertsB[i].y };
        Vec2 axis = { -edge.y, edge.x }; // perpendicular
        if (!overlap_on_axis(vertsA, vertsB, axis)) return false;
    }

    return true;
}

std::vector<Vec2>& Hitbox::get_vertices() {
    return m_vertices;
}