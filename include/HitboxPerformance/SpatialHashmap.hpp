#include <vector>
#include "HitboxPerformance/Hitbox.hpp"
#pragma once

struct IVec2 {
    int x;
    int y;
};

class SpatialHashmap {
    std::vector<Hitbox> m_hitboxes;
    std::vector<IVec2> m_hitbox_cells; // which cell the corresponding hitbox is on
    // Actual spatial hash map grid, the last one is dedicated to stuff outside the grid
    std::vector<std::vector<Hitbox*>> m_grid;
    int m_grid_width;
    int m_grid_height;
    float m_cell_width;
    float m_cell_height;

public:
    SpatialHashmap() = default;
    SpatialHashmap(int grid_width, int grid_height, float cell_width, float cell_height) {
        m_grid_width = grid_width;
        m_grid_height = grid_height;
        m_cell_width = cell_width;
        m_cell_height = cell_height;
        m_grid.resize((grid_height * grid_width) + 1);
    }

    int grid_width() const;
    int grid_height() const;

    // Returns the contents of an individual cell
    std::vector<Hitbox*>& get_cell(int x, int y);

    // Returns a full list of all hitboxes in the spatial hashmap
    std::vector<Hitbox>& get_hitboxes();

    // Prompts the map to update the position of a hitbox in the grid
    void update_hitbox(int hitbox_index, bool remove_first = true);

    // Returns the cell a hitbox is in
    IVec2 get_hitbox_cell(int hitbox_index);

    // Returns true if a cell is out of bounds in the map (and would be placed in the outzone)
    bool is_out_of_bounds(int x, int y) const;

    // Adds a hitbox to the hashmap
    void add_hitbox(Hitbox&& hitbox);

    // Draws the grid lines for the spatial hash map
    void draw();
};
