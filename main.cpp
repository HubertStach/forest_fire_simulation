#include "imgui.h"
#include "raylib.h"
#include "rlImGui.h"
#include "rlImGuiColors.h"

#include <algorithm>
#include <fstream>
#include <stdio.h>

#include "automat.h"
#include "image.h"


int screenWidth = 1280;
int screenHeight = 768;

int x_size=300;
int y_size=0;

bool auto_play = false;

int curr_turn=0;

Automaty automat;

int main(int argc, char* argv[]) {
    // Initialization

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Symualcja pozaru lasu");
    //SetTargetFPS(60);
    rlImGuiSetup(true);

    // Our UI state
    ImVec4 bkgndColour(0.1f, 0.1f, 0.1f, 1.0f);

    // Main game loop
    bool running = true;
    bool showDemoWindow = false;
    bool paint_fire = false;

    automat.burn_prop = 10;

    //wind directions
    int wind_direction = 0;

    //std::string file_path_string_fixed = "D:/Code/Uczelnia/S5/mod_dys/cw5/image3.png";
    std::string file_path_string_fixed;

    while (running && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(rlImGuiColors::Convert(bkgndColour));

        // start ImGui Content
        rlImGuiBegin();

        // Our menu bar
        if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Quit"))
					running = false;

				ImGui::EndMenu();
            }

			if (ImGui::BeginMenu("Window")) {
                if (ImGui::MenuItem("Demo Window", nullptr, showDemoWindow))
					showDemoWindow = !showDemoWindow;

                ImGui::EndMenu();
            }
			ImGui::EndMainMenuBar();
		}

        // This is the main display area, that fills the app's window
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        //float toolbarWidth = std::max(250.0f, ImGui::GetContentRegionAvail().x * 0.25f);
        float toolbarWidth = 250.0f;

        if (ImGui::Begin("Main Window", nullptr, 
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_NoBringToFrontOnFocus)) {

            // The toolbar
            const float minScale = 0.1f;
            const float maxScale = 4.0f;
            
            ImGui::BeginChild("Toolbar", ImVec2(toolbarWidth, 0.0f));
                    
            ImGui::Text("Rozmiar pola");
            ImGui::InputInt("X", &x_size);
            ImGui::InputInt("Y", &y_size);
            //ImGui::Checkbox("Losowe wartosci?", &is_random);

            ImGui::NewLine();

            if(ImGui::Button("Generuj pole z obrazu")){
                if(x_size != 0){

                    try{
                        std::ifstream text_file("D:/Code/Uczelnia/S5/mod_dys/cw5/image_path.txt");
                        
                        getline(text_file, file_path_string_fixed);

                        text_file.close();

                        std::vector<std::vector<int>> img_matrix = prepare_matrix(file_path_string_fixed, x_size);
                        automat.init(img_matrix, screenHeight, screenWidth);
                        automat.iteration_count = 0;
    
                        x_size = img_matrix.size();
                        y_size = img_matrix[0].size();
                    } catch(...){
                        std::cout<<"Nie udalo sie zaladowac pliku tekstowego\n";
                    }

                }
            }

            ImGui::NewLine();
            ImGui::InputInt("Kierunek wiatru", &automat.wind_direction);
            ImGui::Text("0 - brak");
            ImGui::Text("1 - polnocny");
            ImGui::Text("2 - wschodni");
            ImGui::Text("3 - poludniowy");
            ImGui::Text("4 - zachodni");

            if(automat.wind_direction > 4){automat.wind_direction = 0;}
            else if(automat.wind_direction <0){automat.wind_direction = 4;}

            ImGui::InputInt("Prawdopodobienstwo zaplonu", &automat.burn_prop);
            if(automat.burn_prop >= 100){automat.burn_prop = 100;}
            else if(automat.burn_prop <=0){automat.burn_prop = 0;}


            ImGui::NewLine();
            ImGui::Checkbox("Pozar", &paint_fire);
            ImGui::NewLine();
            ImGui::Checkbox("Automatyczne odtwarzanie", &auto_play);
            ImGui::LabelText("Tura", "%d", automat.iteration_count);

            bool next_turn = false;
            
            if(ImGui::Button("Nowa tura")){
                next_turn = true;
            }

            if((automat.initialised && next_turn) || automat.initialised && auto_play){
                automat.simulate_curr_state();
            }

            ImGui::EndChild();

            ImGui::SameLine();
                
            // The main display

            ImGui::PushStyleColor(ImGuiCol_ChildBg, bkgndColour);
            ImGui::BeginChild("Main", ImVec2(0.0f, 0.0f), 
                ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
        }
        ImGui::End();
        
        if(showDemoWindow) {        
            ImGui::ShowDemoWindow(&showDemoWindow);
        }

        // End ImGui Content
        rlImGuiEnd();

        // ##### INSERT ANY ADDITIONAL RENDERING YOU WANT TO DO OVER THE GUI HERE #####

        //mainStart to szerokość od której 
        float mainStart = toolbarWidth+15;
        
        if(automat.initialised){
            automat.visualise();

            if(paint_fire){
                automat.paint_fire();
            }
        }

        EndDrawing();
    }
    rlImGuiShutdown();

    // Cleanup
    rlImGuiShutdown();
    CloseWindow();

    return EXIT_SUCCESS;
} 