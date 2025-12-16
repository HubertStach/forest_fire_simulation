#include "automat.h"
#include "raylib.h"
#include <cstdlib>
#include <iostream>
#include <cmath>

block::block()
{
    this->x = 0;
    this->y = 0;
    this->state = 0;
    this->height = 0;
    this->iterations_burning = 0;
}

block::block(int x, int y, int new_state, int height){
    this->x = x;
    this->y = y;
    this->state = new_state;
    this->height = height;
    // Losowy czas spalania (10-25 iteracji dla realizmu)
    this->iterations_burning = rand() % 16 + 10;
}

Automaty::Automaty(){
    this->pole.assign(1, std::vector<block>(1));
    this->iteration_count = 0;
    this->wind_direction = 0;
    this->moisture = 0.0;
}

void Automaty::init(std::vector<std::vector<int>> &matrix_img, std::vector<std::vector<int>> &matrix_ter, int SCREEN_HEIGHT, int SCREEN_WIDTH)
{
    int rows = matrix_img.size();
    int cols = matrix_img[0].size();
    
    this->pole.assign(rows, std::vector<block>(cols));
    int temp_height = 0;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {  
            int state = matrix_img[i][j];
            int height = matrix_ter[i][j];

            if(height >= temp_height){
                temp_height = height;
            }

            pole[i][j] = block(i, j, state, height);
        }
    }

    this->max_height = temp_height;
    
    int toolbar_width = 250;
    int margin = 20;

    int x_start = toolbar_width + margin;
    int x_end = SCREEN_WIDTH - margin;
    int y_start = margin;
    int y_end = SCREEN_HEIGHT - margin;
    
    int board_width = x_end - x_start; 
    int board_height = y_end - y_start;

    if (cols == 0 || rows == 0) return;

    int cell_w = board_width / cols;
    int cell_h = board_height / rows;
    int cell_size = (cell_w < cell_h) ? cell_w : cell_h;
    if (cell_size <= 0) return;

    int used_width = cell_size * cols;
    int used_height = cell_size * rows;

    int offset_x = x_start + (board_width - used_width) / 2;
    int offset_y = y_start + (board_height - used_height) / 2;

    this->cell_h = cell_h;
    this->cell_w = cell_w;
    this->x_start = x_start;
    this->x_end = x_end;
    this->y_start = y_start;
    this->y_end = y_end;
    this->cell_size = cell_size;
    this->initialised = true;

    std::cout << "Max height: " << this->max_height << "\n";
}

void Automaty::visualise()
{
    int cols = this->pole.size();
    int rows = (int)pole[0].size();

    int board_width = x_end - x_start;
    int board_height = y_end - y_start;

    int cell_width = board_width / cols;
    int cell_height = board_height / rows;
    
    int adaptive_cell_size = std::min(cell_width, cell_height);

    int offset_x = x_start;
    int offset_y = y_start;

    Color background = {30, 30, 30, 255};
    DrawRectangle(x_start, y_start, board_width, board_height, background);

    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            int px = offset_x + i * adaptive_cell_size;
            int py = offset_y + j * adaptive_cell_size;

            Color col = BLACK;

            // Stan komórki
            if(pole[i][j].state == 0){
                col = GREEN; 
            }
            else if(pole[i][j].state == 1){
                // Animacja ognia - zmiana koloru w zależności od czasu spalania
                int burn_time = pole[i][j].iterations_burning;
                if(burn_time > 15){
                    col = ORANGE;
                } else if(burn_time > 5){
                    col = RED;
                }
            }
            else if(pole[i][j].state == 2){
                col = DARKGRAY;
            }
            else if(pole[i][j].state == 3){
                col = BLUE;
            }

            // Cieniowanie według wysokości
            float max_height_f = (float)this->max_height;
            float h_curr = (float)pole[i][j].height;
            if (h_curr > max_height_f) h_curr = max_height_f;
            float brightness = 0.4f + (h_curr / max_height_f) * 0.6f;

            col.r = (unsigned char)(col.r * brightness);
            col.g = (unsigned char)(col.g * brightness);
            col.b = (unsigned char)(col.b * brightness);
            
            DrawRectangle(px, py, adaptive_cell_size, adaptive_cell_size, col);
            Color gridCol = {100, 100, 100, 100};
            DrawRectangleLines(px, py, adaptive_cell_size, adaptive_cell_size, gridCol);
        }
    }
}

std::pair<int, int> Automaty::get_cell_at_mouse(int mouse_x, int mouse_y)
{
    int cols = this->pole.size();
    int rows = (int)pole[0].size();
    int board_width = x_end - x_start;
    int board_height = y_end - y_start;
    
    int cell_width = board_width / cols;
    int cell_height = board_height / rows;
    int adaptive_cell_size = std::min(cell_width, cell_height);
    
    int offset_x = x_start;
    int offset_y = y_start;

    if (mouse_x < offset_x || mouse_x >= offset_x + cols * adaptive_cell_size ||
        mouse_y < offset_y || mouse_y >= offset_y + rows * adaptive_cell_size) {
        return {-1, -1}; 
    }

    int col = (mouse_x - offset_x) / adaptive_cell_size;
    int row = (mouse_y - offset_y) / adaptive_cell_size;

    if (col >= 0 && col < cols && row >= 0 && row < rows) {
        return {col, row};
    }

    return {-1, -1}; 
}

double Automaty::calculate_ignition_prop(int x, int y, int cols_num, int rows_num) {
    double base_probability = 0.0;
    
    const double BASE_SPREAD_RATE = this->opcje.BASE_SPREAD_RATE;
    const double DIAGONAL_FACTOR = this->opcje.DIAGONAL_FACTOR;
    
    const double WIND_BOOST_STRONG = this->opcje.WIND_BOOST_STRONG;
    const double WIND_BOOST_SIDE = this->opcje.WIND_BOOST_SIDE;
    const double WIND_PENALTY_AGAINST = this->opcje.WIND_PENALTY_AGAINST;

    // Zwiększona wrażliwość na nachylenie dla realizmu
    const double SLOPE_SENSITIVITY = 0.15;

    int wind_dx = 0, wind_dy = 0;
    switch(this->wind_direction) {
        case 1: wind_dy = 1; break; // północ
        case 2: wind_dx = -1;  break; // wschód
        case 3: wind_dy = -1;  break; // południe
        case 4: wind_dx = 1; break; // zachód
    }
    
    double target_height = (double)this->pole[x][y].height;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            
            int nx = x + dx;
            int ny = y + dy;
            
            if (nx < 0 || nx >= cols_num || ny < 0 || ny >= rows_num) continue;
            
            // Sprawdzamy czy sąsiad płonie
            if (this->pole[nx][ny].state != 1) {
                continue;
            }
            
            double spread_chance = BASE_SPREAD_RATE;
            
            // Odległość między komórkami
            double distance_between_cells = 1.0;
            if (abs(dx) + abs(dy) == 2) {
                spread_chance *= DIAGONAL_FACTOR;
                distance_between_cells = 1.4142;
            }
            
            // Wpływ wiatru
            if (this->wind_direction != 0) {
                // Kierunek rozprzestrzeniania się (od płonącej do docelowej)
                int spread_dx = dx;  // POPRAWKA: bez negacji
                int spread_dy = dy;
                
                int wind_alignment = spread_dx * wind_dx + spread_dy * wind_dy;
                
                if (wind_alignment > 0) {
                    spread_chance *= WIND_BOOST_STRONG;
                } else if (wind_alignment < 0) {
                    spread_chance *= WIND_PENALTY_AGAINST;
                } else {
                    spread_chance *= WIND_BOOST_SIDE;
                }
            }

            double source_height = (double)this->pole[nx][ny].height;
            double elevation_diff = target_height - source_height;
            double slope = elevation_diff / distance_between_cells;
            
            double slope_factor = std::exp(SLOPE_SENSITIVITY * slope);
            
            if (slope_factor > 10.0) slope_factor = 10.0;
            if (slope_factor < 0.2) slope_factor = 0.2;
            
            spread_chance *= slope_factor;

            base_probability += spread_chance;
        }
    }
    
    // Ograniczenie prawdopodobieństwa
    if (base_probability > 1.0) {
        base_probability = 1.0;
    }

    return base_probability;
}

void Automaty::simulate_curr_state(){

    int cols_num = (int)this->pole.size();
    if (cols_num == 0) return;
    int rows_num = (int)this->pole[0].size();
    if (rows_num == 0) return;
    
    std::vector<std::vector<block>> next = this->pole;

    for (int x = 0; x < cols_num; ++x) {
        for (int y = 0; y < rows_num; ++y) {
            
            int current_state = this->pole[x][y].state;

            // Komórka płonie - spala się
            if (current_state == 1) {
                if(this->pole[x][y].iterations_burning == 0){
                    next[x][y].state = 2;  // Spalone
                }
                else{
                    next[x][y].iterations_burning--;
                }
            }
            
            // Komórka żywa - może się zapalić
            if (current_state == 0) {
                double ignition_prob = calculate_ignition_prop(x, y, cols_num, rows_num);
                
                // Wpływ wilgotności
                double moisture_factor = 1.0 - (this->moisture / 100.0);
                ignition_prob *= moisture_factor;
                
                double rand_value = (double)(rand() % 100000) / 100000.0*2.0;
                
                if (rand_value*2 < ignition_prob) {
                    next[x][y].state = 1;
                    next[x][y].iterations_burning = rand() % 16 + 10;
                }
            }
        }
    }

    this->pole.swap(next);
    this->iteration_count++;
}

void Automaty::paint_fire()
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        auto [col, row] = get_cell_at_mouse((int)mousePos.x, (int)mousePos.y);

        if(col != -1 && row != -1){
            pole[col][row].state = 1;
            pole[col][row].iterations_burning = rand() % 16 + 10;
        }
    }
}