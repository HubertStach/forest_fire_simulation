#pragma once

#include <vector>

struct block{
    int x;
    int y;
    int state=0;
    int iterations_burning = 10;
    int height = 0;

    block();
    block::block(int x, int y, int new_state=0, int height = 0);
};

struct Automaty{
    std::vector<std::vector<block>> pole;

    int iteration_count = 0;
    bool initialised = false;
    int px=0;
    int py=0;
    int cell_w=0;
    int cell_h=0;
    int x_start = 0;
    int x_end = 0;
    int y_start = 0;
    int y_end = 0;
    int cell_size=0;

    int wind_direction = 0;
    int moisture=40;
    int max_height=0;

    Automaty();

    void init(int x_size, int y_size, int SCREEN_HEIGHT, int SCREEN_WIDTH);
    void init(std::vector<std::vector<int>> &matrix_img, std::vector<std::vector<int>> &matrix_ter, int SCREEN_HEIGHT, int SCREEN_WIDTH);
    void visualise();
    double calculate_ignition_prop(int x, int y, int cols_num, int rows_num);
    void simulate_curr_state();
    
    std::pair<int, int> get_cell_at_mouse(int mouse_x, int mouse_y);
    void paint_fire();
};