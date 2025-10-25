#include <random>
#include <vector>
#include <SDL3/SDL.h>
#include "HitboxPerformance/App.hpp"
#include "HitboxPerformance/RendererState.hpp"
#include "HitboxPerformance/Constants.hpp"
#include "HitboxPerformance/Hitbox.hpp"
#include "HitboxPerformance/SpatialHashmap.hpp"

enum class CollisionMode {
    AABB_ONLY,
    AABB_THEN_SAT,
    SAT_ONLY
};

CollisionMode gCollisionMode = CollisionMode::AABB_THEN_SAT;
bool gEnableSpatialCollisions = false;

// For moving hitboxes around
struct Speed {
    float x;
    float y;
};

std::vector<Speed> gHitboxesSpeeds;
SpatialHashmap gSpace;
#define SCALE 3
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
    gSpace = SpatialHashmap(16, 12, 50, 50);

    // Randomly generate the hitboxes
    const std::vector<Vec2> polygons[] = {POLYGON_1, POLYGON_2, POLYGON_3};
    for (int i = 0; i < Constants::HITBOX_COUNT; i++) {
        gSpace.add_hitbox(Hitbox(polygons[i % 3]));
        gHitboxesSpeeds.push_back((Speed){.x = static_cast<float>(speed(gen)), .y = static_cast<float>(speed(gen))});
    }
}

void collision(Hitbox *h1, Hitbox *h2, bool *bb_collision, bool *collision) {
    switch (gCollisionMode) {
        case CollisionMode::AABB_ONLY:
            if (h2->bounding_box_collision(*h1)) {
                *bb_collision = true;
                *collision = true;
                break;
            }
            break;

        case CollisionMode::AABB_THEN_SAT:
            if (h2->bounding_box_collision(*h1)) {
                *bb_collision = true;
                if (h2->sat_collision(*h1)) {
                    *collision = true;
                    break;
                }
            }
            break;

        case CollisionMode::SAT_ONLY:
            if (h2->sat_collision(*h1)) {
                *collision = true;
                *bb_collision = true;
                break;
            }
            break;
    }
}

// Called 1x each frame, return false to quit
bool HitboxApp::loop() {
    auto hitbox_list = gSpace.get_hitboxes();

    // Move around the first hitbox so user has some level of control
    if (!hitbox_list->empty()) {
        float x, y;
        SDL_GetMouseState(&x, &y);
        const float mouse_delta_x = x - hitbox_list->at(0).bounding_box().x1();
        const float mouse_delta_y = y - hitbox_list->at(0).bounding_box().y1();
        hitbox_list->at(0).move(mouse_delta_x, mouse_delta_y);
        gSpace.update_hitbox(0);
    }

    // Read keyboard each frame
    const bool* keyboard = SDL_GetKeyboardState(nullptr);
    if (keyboard[SDL_SCANCODE_1]) gCollisionMode = CollisionMode::AABB_ONLY;
    if (keyboard[SDL_SCANCODE_2]) gCollisionMode = CollisionMode::AABB_THEN_SAT;
    if (keyboard[SDL_SCANCODE_3]) gCollisionMode = CollisionMode::SAT_ONLY;

    // Move all the hitboxes around
    for (int i = 1; i < hitbox_list->size(); i++) {
        Hitbox& hitbox = hitbox_list->at(i);
        Speed& speed = gHitboxesSpeeds.at(i);
        if (hitbox.bounding_box().x1() < 0 || hitbox.bounding_box().x1() > Constants::WINDOW_WIDTH - hitbox.bounding_box().w())
            speed.x = -speed.x;
        if (hitbox.bounding_box().y1() < 0 || hitbox.bounding_box().y1() > Constants::WINDOW_HEIGHT - hitbox.bounding_box().h())
            speed.y = -speed.y;
        hitbox.move(speed.x, speed.y);
        gSpace.update_hitbox(i);
    }

    //start collision timing
    Uint64 collision_start = SDL_GetTicksNS();

    // Draw spatial hash map
    if (gEnableSpatialCollisions)
        gSpace.draw();

    // Draw all the hitboxes
    SDL_Color white = {.r = 255, .g = 255, .b = 255, .a = 255};
    SDL_Color red = {.r = 217, .g = 65, .b = 54, .a = 255};

    // Draw bounding boxes & eventually the actual hitbox
    int i = 0;
    for (auto& hitbox: *hitbox_list) {
        // Check for collisions with other hitboxes
        bool colliding_bb = false;
        bool colliding = false;

        if (!gEnableSpatialCollisions) {
            // If no spatial awareness, we just check against every possible hitbox
            for (auto &other: *hitbox_list) {
                if (&other == &hitbox) continue;
                collision(&other, &hitbox, &colliding_bb, &colliding);
            }
        } else {
            // If there is, we check against the current cell and the 8 adjacent ones
            IVec2 cell = gSpace.get_hitbox_cell(i);
            for (int y = cell.y - 1; y <= cell.y + 1; y++) {
                for (int x = cell.x - 1; x <= cell.x + 1; x++) {
                    for (auto other: gSpace.get_cell(x, y)) {
                        if (other == &hitbox) continue;
                        collision(other, &hitbox, &colliding_bb, &colliding);
                    }
                }
            }
        }

        if (gCollisionMode == CollisionMode::AABB_THEN_SAT || gCollisionMode == CollisionMode::AABB_ONLY)
            hitbox.draw_bounding_box(colliding_bb ? red : white);
        if (gCollisionMode == CollisionMode::AABB_THEN_SAT || gCollisionMode == CollisionMode::SAT_ONLY)
            hitbox.draw_hitbox(colliding ? red : white);

        i++;
    }
    Uint64 collision_elapsed = SDL_GetTicksNS() - collision_start;
    double collision_us = collision_elapsed / 1000; // nano to micro

    // update window title once per frame
    std::string mode_string;
    switch (gCollisionMode) {
    case CollisionMode::AABB_ONLY: mode_string = "AABB Only"; break;
    case CollisionMode::AABB_THEN_SAT: mode_string = "AABB then SAT"; break;
    case CollisionMode::SAT_ONLY: mode_string = "SAT Only"; break;
    }
    std::string title = "Hitbox Performance - (Use Keys 1-3 to swap mode) Mode: " + mode_string +
                        " | Collision: " + std::to_string(collision_us) + " μs";
    SDL_SetWindowTitle(RendererState::instance().get_window(), title.c_str());

    return true;
}

// Called when the program is quit, before SDL is cleaned up
void HitboxApp::cleanup() {

}