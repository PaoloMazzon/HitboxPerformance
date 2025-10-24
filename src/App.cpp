#include <random>
#include <vector>
#include <SDL3/SDL.h>
#include "HitboxPerformance/App.hpp"
#include "HitboxPerformance/RendererState.hpp"
#include "HitboxPerformance/Constants.hpp"
#include "HitboxPerformance/Hitbox.hpp"

enum class CollisionMode {
    AABB_ONLY,
    AABB_THEN_SAT,
    SAT_ONLY
};

CollisionMode gCollisionMode = CollisionMode::AABB_THEN_SAT;

// For moving hitboxes around
struct Speed {
    float x;
    float y;
};

std::vector<Hitbox> gHitboxes;
std::vector<Speed> gHitboxesSpeeds;
#define SCALE 4
std::vector<Vec2> POLYGON_1 = {
    {2 * SCALE, 2 * SCALE},
    {10 * SCALE, 4 * SCALE},
    {11 * SCALE, 13 * SCALE},
    {3 * SCALE, 10 * SCALE}
};

std::vector<Vec2> POLYGON_2 = {
        {1 * SCALE, 1 * SCALE},
        {13 * SCALE, 2 * SCALE},
        {15 * SCALE, 13 * SCALE},
        {7 * SCALE, 14 * SCALE},
        {1 * SCALE, 9 * SCALE}
};

std::vector<Vec2> POLYGON_3 = {
        {10 * SCALE, 1 * SCALE},
        {13 * SCALE, 4 * SCALE},
        {14 * SCALE, 8 * SCALE},
        {10 * SCALE, 14 * SCALE},
        {5 * SCALE, 14 * SCALE},
        {1 * SCALE, 8 * SCALE},
        {2 * SCALE, 4 * SCALE},
        {5 * SCALE, 1 * SCALE},
};

// Called once after the window and renderer are successfully initialized
void HitboxApp::create() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> width(0, Constants::WINDOW_WIDTH - Constants::HITBOX_SIZE);
    std::uniform_int_distribution<> height(0, Constants::WINDOW_HEIGHT - Constants::HITBOX_SIZE);
    std::uniform_real_distribution<> speed(-Constants::MAX_SPEED, Constants::MAX_SPEED);

    // Randomly generate the hitboxes
    const std::vector<Vec2> polygons[] = {POLYGON_1, POLYGON_2, POLYGON_3};
    for (int i = 0; i < Constants::HITBOX_COUNT; i++) {
        gHitboxes.emplace_back(polygons[i % 3]);
        gHitboxesSpeeds.push_back((Speed){.x = static_cast<float>(speed(gen)), .y = static_cast<float>(speed(gen))});
    }
}

// Called 1x each frame, return false to quit
bool HitboxApp::loop() {
    // Move around the first hitbox so user has some level of control
    if (!gHitboxes.empty()) {
        float x, y;
        SDL_GetMouseState(&x, &y);
        const float mouse_delta_x = x - gHitboxes[0].bounding_box().x1();
        const float mouse_delta_y = y - gHitboxes[0].bounding_box().y1();
        gHitboxes[0].move(mouse_delta_x, mouse_delta_y);
    }

    // Read keyboard each frame
    const bool* keyboard = SDL_GetKeyboardState(nullptr);
    if (keyboard[SDL_SCANCODE_1]) gCollisionMode = CollisionMode::AABB_ONLY;
    if (keyboard[SDL_SCANCODE_2]) gCollisionMode = CollisionMode::AABB_THEN_SAT;
    if (keyboard[SDL_SCANCODE_3]) gCollisionMode = CollisionMode::SAT_ONLY;

    // Move all the hitboxes around
    for (int i = 1; i < gHitboxes.size(); i++) {
        Hitbox& hitbox = gHitboxes.at(i);
        Speed& speed = gHitboxesSpeeds.at(i);
        if (hitbox.bounding_box().x1() < 0 || hitbox.bounding_box().x1() > Constants::WINDOW_WIDTH - hitbox.bounding_box().w())
            speed.x = -speed.x;
        if (hitbox.bounding_box().y1() < 0 || hitbox.bounding_box().y1() > Constants::WINDOW_HEIGHT - hitbox.bounding_box().h())
            speed.y = -speed.y;
        hitbox.move(speed.x, speed.y);
    }

    // Draw all the hitboxes
    SDL_Color white = {.r = 255, .g = 255, .b = 255, .a = 255};
    SDL_Color red = {.r = 217, .g = 65, .b = 54, .a = 255};

    // Draw bounding boxes & eventually the actual hitbox
    for (auto& hitbox: gHitboxes) {
        // Check for collisions with other hitboxes
        bool colliding_bb = false;
        bool colliding = false;
        for (auto& other: gHitboxes) {
            if (&other == &hitbox) continue;

            switch (gCollisionMode) {
            case CollisionMode::AABB_ONLY:
                if (other.bounding_box_collision(hitbox)) {
                    colliding_bb = true;
                    colliding = true;
                    break;
                }
                break;

            case CollisionMode::AABB_THEN_SAT:
                if (other.bounding_box_collision(hitbox)) {
                    colliding_bb = true;
                    if (other.sat_collision(hitbox)) {
                        colliding = true;
                        break;
                    }
                }
                break;

            case CollisionMode::SAT_ONLY:
                if (other.sat_collision(hitbox)) {
                    colliding = true;
                    colliding_bb = true;
                    break;
                }
                break;
            }
        }

        if (gCollisionMode == CollisionMode::AABB_THEN_SAT || gCollisionMode == CollisionMode::AABB_ONLY)
            hitbox.draw_bounding_box(colliding_bb ? red : white);
        if (gCollisionMode == CollisionMode::AABB_THEN_SAT || gCollisionMode == CollisionMode::SAT_ONLY)
            hitbox.draw_hitbox(colliding ? red : white);
    }

    return true;
}

// Called when the program is quit, before SDL is cleaned up
void HitboxApp::cleanup() {

}