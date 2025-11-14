#include "automat.h"
#include "raylib.h"
#include <cstdlib>
#include <iostream>

#include <cstdlib>

block::block()
{
    this->x = 0;
    this->y = 0;
    this->state = 0;
}

block::block(int x, int y, int new_state){
    this->x = x;
    this->y = y;
    this->state = new_state;

    //this->iterations_burning = rand() % 16+10;
}

Automaty::Automaty(){
    this->pole.assign(1, std::vector<block>(1));
}

void Automaty::init(int x_size, int y_size, int SCREEN_HEIGHT, int SCREEN_WIDTH)
{
    this->pole.assign(x_size, std::vector<block>(y_size));

    for (int i = 0; i < x_size; ++i) {
        for (int j = 0; j < y_size; ++j) {
            int state = 0;
            if(rand()%100001 == 1){
                state = 1;
            }

            pole[i][j] = block(i, j, state);
        }
    }
    

    int toolbar_width = 250; //stała w programie
    int margin = 20; //żeby jakoś lepiej wyglądało

    int x_start, x_end, y_start, y_end;

    x_start = toolbar_width + margin;
    x_end = SCREEN_WIDTH - margin;
    y_start = margin;
    y_end = SCREEN_HEIGHT - margin;
    
    //cale pole do rysowania
    int board_width = x_end - x_start; 
    int board_height = y_end - y_start;

    int cols = x_size;
    int rows = y_size;

    if (cols == 0 || rows == 0) return; //plansza pusta

    int cell_w = board_width / cols;
    int cell_h = board_height / rows;
    int cell_size = (cell_w < cell_h) ? cell_w : cell_h;
    if (cell_size <= 0) return;

    //liczymy środek planszy
    int used_width = cell_size * cols;
    int used_height = cell_size * rows;

    int offset_x = x_start + (board_width - used_width) / 2;
    int offset_y = y_start + (board_height - used_height) / 2;

    this->px = px;
    this->py = py;
    this->cell_h = cell_h;
    this->cell_w = cell_w;
    this->x_start = x_start;
    this->x_end = x_end;
    this->y_start = y_start;
    this->y_end = y_end;
    this->cell_size = cell_size;

    this->initialised = true;

    std::cout<<"used_width = "<<cell_size * cols<<"\n";
    std::cout<<"used_height = "<<cell_size * rows<<"\n";

}

void Automaty::init(std::vector<std::vector<int>> &matrix, int SCREEN_HEIGHT, int SCREEN_WIDTH)
{
    int rows = matrix.size();
    int cols = matrix[0].size();
    
    this->pole.assign(rows, std::vector<block>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {  
            int state = matrix[i][j];

            pole[i][j] = block(i, j, state);
        }
    }
    
    int toolbar_width = 250; //stała w programie
    int margin = 20; //żeby jakoś lepiej wyglądało

    int x_start, x_end, y_start, y_end;

    x_start = toolbar_width + margin;
    x_end = SCREEN_WIDTH - margin;
    y_start = margin;
    y_end = SCREEN_HEIGHT - margin;
    
    //cale pole do rysowania
    int board_width = x_end - x_start; 
    int board_height = y_end - y_start;

    if (cols == 0 || rows == 0) return; //plansza pusta

    int cell_w = board_width / cols;
    int cell_h = board_height / rows;
    int cell_size = (cell_w < cell_h) ? cell_w : cell_h;
    if (cell_size <= 0) return;

    //liczymy środek planszy
    int used_width = cell_size * cols;
    int used_height = cell_size * rows;

    int offset_x = x_start + (board_width - used_width) / 2;
    int offset_y = y_start + (board_height - used_height) / 2;

    this->px = px;
    this->py = py;
    this->cell_h = cell_h;
    this->cell_w = cell_w;
    this->x_start = x_start;
    this->x_end = x_end;
    this->y_start = y_start;
    this->y_end = y_end;
    this->cell_size = cell_size;

    this->initialised = true;

    //std::cout<<"used_width = "<<cell_size * cols<<"\n";
    //std::cout<<"used_height = "<<cell_size * rows<<"\n";
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

    //tło
    Color background = {30, 30, 30, 0};
    DrawRectangle(x_start, y_start, x_end, y_end, background);

    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            int px = offset_x + i * adaptive_cell_size;
            int py = offset_y + j * adaptive_cell_size;

            Color col = BLACK;

            //drzewo zyje
            if(pole[i][j].state == 0){
                col = GREEN;
            }

            //plonie
            else if(pole[i][j].state == 1){
                col = RED;
            }
            //splonelo
            else if(pole[i][j].state == 2){
                col = DARKGRAY;
            }

            else if(pole[i][j].state == 3){
                col = BLUE;
            }

            DrawRectangle(px, py, adaptive_cell_size, adaptive_cell_size, col);
            Color gridCol = { 100, 100, 100, 100 };
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

    // Oblicz rozmiar komórki (tak samo jak w visualise)
    int cell_width = board_width / cols;
    int cell_height = board_height / rows;
    int adaptive_cell_size = std::min(cell_width, cell_height);
    
    int offset_x = x_start;
    int offset_y = y_start;

    // Sprawdź czy kursor jest w obszarze planszy
    if (mouse_x < offset_x || mouse_x >= offset_x + cols * adaptive_cell_size ||
        mouse_y < offset_y || mouse_y >= offset_y + rows * adaptive_cell_size) {
        return {-1, -1}; // Poza planszą
    }

    // Oblicz indeksy komórki
    int col = (mouse_x - offset_x) / adaptive_cell_size;
    int row = (mouse_y - offset_y) / adaptive_cell_size;

    // Sprawdź czy indeksy są w zakresie (dodatkowe zabezpieczenie)
    if (col >= 0 && col < cols && row >= 0 && row < rows) {
        return {col, row};
    }

    return {-1, -1}; 
}

double Automaty::calculate_ignition_prop(int x, int y, int cols_num, int rows_num) {
    double base_probability = 0.0;
    
    const double BASE_SPREAD_RATE = 0.1;
    const double DIAGONAL_FACTOR = 0.7;
    const double WIND_BOOST_STRONG = 2.1;
    const double WIND_BOOST_SIDE = 1.1;
    const double WIND_PENALTY_AGAINST = 0.3;

    
    //kierunki
    int wind_dx = 0, wind_dy = 0;
    switch(this->wind_direction) {
        case 1: wind_dy = -1; break; // polnoc
        case 2: wind_dx = 1;  break; // wschod
        case 3: wind_dy = 1; break; // poludnie
        case 4: wind_dx = -1;  break; // zachod
    }
    
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            
            int nx = x + dx;
            int ny = y + dy;
            
            if (nx < 0 || nx >= cols_num || ny < 0 || ny >= rows_num) continue;
            
            if (this->pole[nx][ny].state != 1) {
                continue;
            }
            
            double spread_chance = BASE_SPREAD_RATE;
            
            if (abs(dx) + abs(dy) == 2) {
                spread_chance *= DIAGONAL_FACTOR;
            }
            
            if (this->wind_direction != 0) {
                int spread_dx = -dx; // odwrotny kierunek
                int spread_dy = -dy;
                
                int wind_alignment = spread_dx * wind_dx + spread_dy * wind_dy;
                
                if (wind_alignment > 0) {
                    spread_chance *= WIND_BOOST_STRONG;
                } else if (wind_alignment < 0) {
                    spread_chance *= WIND_PENALTY_AGAINST;
                } else {
                    spread_chance *= WIND_BOOST_SIDE;
                }
            }
            
            base_probability += spread_chance;
        }
    }
    
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

            //wypala sie
            if (current_state == 1) {
                if(this->pole[x][y].iterations_burning == 0){
                    next[x][y].state = 2;
                }
                else{
                    next[x][y].iterations_burning--;
                }

            }

            //spalone drzewo moze odrosnac
            /*
            if (current_state == 2 && iteration_count > 1000) {
                if ((rand() % 100000000000000) == 1) { 
                    next[x][y].state = 0;
                }
                continue; 
            }*/

            //drzewo moze sie zapalic
            if (current_state == 0) {
                double ignition_prob = calculate_ignition_prop(x, y, cols_num, rows_num);
                


                double moisture_factor = 1.0 - (this->moisture / 100.0);
                ignition_prob *= moisture_factor;
                

                if (rand() % 100000 < (int)(ignition_prob * 100000)/2) {
                    next[x][y].state = 1;
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
        }

    }
}
