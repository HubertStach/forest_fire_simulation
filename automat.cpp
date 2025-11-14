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
            if (current_state == 2 && iteration_count > 800) {
                if ((rand() % 100000000000000) == 1) { 
                    next[x][y].state = 0;
                }
                continue; 
            }

            //zyjace drzewo moze zaplonac
            if (current_state == 0) {
                int burning_neighbors = 0;

                int burning_left = 0;
                int burning_right = 0;
                int burning_top = 0;
                int burning_bottom = 0;
                
                int dx=0;
                int dy=0;
                
                int bot_burn=0;
                int effective = 0;
                
                //zliczanie wszystkich
                for (int dx = -1; dx <= 1; ++dx) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        if (dx == 0 && dy == 0) continue; 
                        
                        int nx = x + dx;
                        int ny = y + dy;
                        
                        if (nx >= 0 && nx < cols_num && ny >= 0 && ny < rows_num) {
                            if (this->pole[nx][ny].state == 1) { 
                                burning_neighbors++;
                            }
                        }
                    }
                }
                if (burning_neighbors > 0) {
                    switch (this->wind_direction)
                    {
                        case 0: //brak wiatru -> pozar rozprzestrzenia sie z akzdej strony na kazda
                            if ((rand() % (401/burning_neighbors)) < this->burn_prop) {
                                next[x][y].state = 1;
                            }
                        
                        break;

                        case 1: { // wiatr polnocny -> pozar rozprzestrzenia się w gore
                            // Zliczamy palących się sąsiadów z wagami zależnie od kierunku
                            effective = 0;
                            for (int dx2 = -1; dx2 <= 1; ++dx2) {
                                for (int dy2 = -1; dy2 <= 1; ++dy2) {
                                    if (dx2 == 0 && dy2 == 0) continue;
                                    int nx = x + dx2;
                                    int ny = y + dy2;
                                    if (nx >= 0 && nx < cols_num && ny >= 0 && ny < rows_num) {
                                        if (this->pole[nx][ny].state == 1) {
                                            if (ny == y + 1) {
                                                // sąsiedzi z południa (downwind) – silnie zwiększają szansę
                                                effective += 3;
                                            } else if (ny == y) {
                                                // boczni – niewielki wpływ
                                                effective += 1;
                                            } else {
                                                // sąsiedzi z północy (upwind) – pomijamy / bardzo mały wpływ
                                                // effective += 0;
                                            }
                                        }
                                    }
                                }
                            }

                            if (effective > 0) {
                                int mod = 501 / effective;
                                if (mod < 1) mod = 1;
                                mod = std::max(1, mod/2);

                                if ((rand() % mod) < this->burn_prop) {
                                    next[x][y].state = 1;
                                }
                            }
                        } 
                        break;

                        case 3://wiatr poludniowy
                            effective = 0;
                            // Zliczamy palących się sąsiadów z wagami zależnie od kierunku
                            for (int dx2 = -1; dx2 <= 1; ++dx2) {
                                for (int dy2 = -1; dy2 <= 1; ++dy2) {
                                    if (dx2 == 0 && dy2 == 0) continue;
                                    int nx = x + dx2;
                                    int ny = y + dy2;
                                    if (nx >= 0 && nx < cols_num && ny >= 0 && ny < rows_num) {
                                        if (this->pole[nx][ny].state == 1) {
                                            if (ny == y + 1) {
                                                // sąsiedzi z południa (downwind) – pomijamy / bardzo mały wpływ
                                                //effective += 3;
                                            } else if (ny == y) {
                                                // boczni – niewielki wpływ
                                                effective += 1;
                                            } else {
                                                // sąsiedzi z północy (upwind) – silnie zwiekszaja szanse
                                                effective += 3;
                                            }
                                        }
                                    }
                                }
                            }

                            if (effective > 0) {
                                int mod = 501 / effective;
                                if (mod < 1) mod = 1;
                                mod = std::max(1, mod/2);

                                if ((rand() % mod) < this->burn_prop) {
                                    next[x][y].state = 1;
                                }
                            }
                        
                        break;

                        case 2://wiatr wschodni
                            effective = 0;
                            // Zliczamy palących się sąsiadów z wagami zależnie od kierunku
                            for (int dx2 = -1; dx2 <= 1; ++dx2) {
                                for (int dy2 = -1; dy2 <= 1; ++dy2) {
                                    if (dx2 == 0 && dy2 == 0) continue;
                                    int nx = x + dx2;
                                    int ny = y + dy2;
                                    if (nx >= 0 && nx < cols_num && ny >= 0 && ny < rows_num) {
                                        if (this->pole[nx][ny].state == 1) {
                                            if (nx == x + 1) {
                                                //effective += 3;
                                            } else if (nx == x) {

                                                effective += 1;
                                            } else {
                                                
                                                effective += 3;
                                            }
                                        }
                                    }
                                }
                            }

                            if (effective > 0) {
                                int mod = 501 / effective;             
                                if (mod < 1) mod = 1;
                                mod = std::max(1, mod/2);

                                if ((rand() % mod) < this->burn_prop) {
                                    next[x][y].state = 1;
                                }
                            }
                        break;

                        case 4://wiatr zachodni
                            effective = 0;
                            // Zliczamy palących się sąsiadów z wagami zależnie od kierunku
                            for (int dx2 = -1; dx2 <= 1; ++dx2) {
                                for (int dy2 = -1; dy2 <= 1; ++dy2) {
                                    if (dx2 == 0 && dy2 == 0) continue;
                                    int nx = x + dx2;
                                    int ny = y + dy2;
                                    if (nx >= 0 && nx < cols_num && ny >= 0 && ny < rows_num) {
                                        if (this->pole[nx][ny].state == 1) {
                                            if (nx == x + 1) {
                                                effective += 3;
                                            } else if (nx == x) {

                                                effective += 1;
                                            } else {
                                                
                                                //effective += 3;
                                            }
                                        }
                                    }
                                }
                            }

                            if (effective > 0) {
                                int mod = 501 / effective;             
                                if (mod < 1) mod = 1;
                                mod = std::max(1, mod/2);

                                if ((rand() % mod) < this->burn_prop) {
                                    next[x][y].state = 1;
                                }
                            }
                        break;
                    
                    default:
                        break;
                    }
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
