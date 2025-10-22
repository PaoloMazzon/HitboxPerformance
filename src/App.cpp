#include <random>
#include <vector>
#include "HitboxPerformance/App.hpp"
#include "HitboxPerformance/RendererState.hpp"
#include "HitboxPerformance/Constants.hpp"
#include "HitboxPerformance/Hitbox.hpp"

// For moving hitboxes around
struct Speed {
    float x;
    float y;
};

std::vector<Hitbox> gHitboxes;
std::vector<Speed> gHitboxesSpeeds;

// Called once after the window and renderer are successfully initialized
void HitboxApp::create() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> width(0, Constants::WINDOW_WIDTH - Constants::HITBOX_SIZE);
    std::uniform_int_distribution<> height(0, Constants::WINDOW_HEIGHT - Constants::HITBOX_SIZE);
    std::uniform_real_distribution<> speed(-Constants::MAX_SPEED, Constants::MAX_SPEED);

    // Randomly generate the hitboxes
    for (int i = 0; i < Constants::HITBOX_COUNT; i++) {
        gHitboxes.push_back(Hitbox(width(gen), height(gen), Constants::HITBOX_SIZE, Constants::HITBOX_SIZE));
        gHitboxesSpeeds.push_back((Speed){.x = static_cast<float>(speed(gen)), .y = static_cast<float>(speed(gen))});
    }
}

// Called 1x each frame, return false to quit
bool HitboxApp::loop() {
    // Move all the hitboxes around
    for (int i = 0; i < gHitboxes.size(); i++) {
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
        bool colliding = false;
        for (auto& other: gHitboxes) {
            if (&other == &hitbox) continue;
            if (other.bounding_box_collision(hitbox)) {
                colliding = true;
                break;
            }
        }

        hitbox.draw_bounding_box(colliding ? red : white);
    }

    return true;
}

// Called when the program is quit, before SDL is cleaned up
void HitboxApp::cleanup() {

}
