#include <cmath>
#include "HitboxPerformance/SpatialHashmap.hpp"
#include "HitboxPerformance/RendererState.hpp"

int SpatialHashmap::grid_width() const {
    return m_grid_width;
}

int SpatialHashmap::grid_height() const {
    return m_grid_height;
}

std::vector<Hitbox*>& SpatialHashmap::get_cell(int x, int y) {
    if (x >= m_grid_width || x < 0 || y >= m_grid_height || y < 0)
        return m_grid.back();
    return m_grid.at((y * m_grid_width) + x);
}

std::vector<Hitbox> *SpatialHashmap::get_hitboxes() {
    return &m_hitboxes;
}

void SpatialHashmap::update_hitbox(int hitbox_index, bool remove_first) {
    if (hitbox_index < 0 || hitbox_index >= m_hitbox_cells.size()) return;
    Hitbox *h = &m_hitboxes[hitbox_index];
    const float midpoint_x = (h->bounding_box().x1() + h->bounding_box().x2()) / 2;
    const float midpoint_y = (h->bounding_box().y1() + h->bounding_box().y2()) / 2;
    const int target_cell_x = static_cast<int>(floorf(midpoint_x / m_cell_width));
    const int target_cell_y = static_cast<int>(floorf(midpoint_y / m_cell_height));

    // The hitbox has moved cells, we need to change its location in the grid
    // (if both the old and new position are out of bounds nothing happens)
    if ((target_cell_x != m_hitbox_cells[hitbox_index].x || target_cell_y != m_hitbox_cells[hitbox_index].y) &&
        (!is_out_of_bounds(target_cell_x, target_cell_y) || !is_out_of_bounds(m_hitbox_cells[hitbox_index].x, m_hitbox_cells[hitbox_index].y))) {
        std::vector<Hitbox*>& previous_cell = get_cell(m_hitbox_cells[hitbox_index].x, m_hitbox_cells[hitbox_index].y);
        std::vector<Hitbox*>& new_cell = get_cell(target_cell_x, target_cell_y);

        // First we find and remove it from its current spot
        if (remove_first) {
            for (int i = 0; i < previous_cell.size(); i++) {
                if (&m_hitboxes[hitbox_index] == previous_cell[i]) {
                    previous_cell[i] = previous_cell.back();
                    previous_cell.pop_back();
                    break;
                }
            }
        }

        // Now we place it where it should be
        new_cell.push_back(&m_hitboxes[hitbox_index]);

        // And update the reference in the master list
        m_hitbox_cells[hitbox_index].x = target_cell_x;
        m_hitbox_cells[hitbox_index].y = target_cell_y;
    }
}

bool SpatialHashmap::is_out_of_bounds(int x, int y) const {
    return (x >= m_grid_width || x < 0 || y >= m_grid_height || y < 0);
}

IVec2 SpatialHashmap::get_hitbox_cell(int hitbox_index) {
    return m_hitbox_cells[hitbox_index];
}

void SpatialHashmap::add_hitbox(Hitbox&& hitbox) {
    m_hitboxes.push_back(hitbox);
    m_hitbox_cells.push_back({0, 0});
    update_hitbox(static_cast<int>(m_hitboxes.size() - 1), false);
}

void SpatialHashmap::draw() {
    RendererState& state = RendererState::instance();
    SDL_SetRenderDrawColor(state.get_renderer(), 255, 255, 255, 255);
    for (int y = 0; y < m_grid_height; y++) {
        for (int x = 0; x < m_grid_width; x++) {
            SDL_FRect rect = {
                    .x = static_cast<float>(x) * m_cell_width,
                    .y = static_cast<float>(y) * m_cell_height,
                    .w = m_cell_width + 1,
                    .h = m_cell_height + 1
            };
            SDL_RenderRect(state.get_renderer(), &rect);
        }
    }
}