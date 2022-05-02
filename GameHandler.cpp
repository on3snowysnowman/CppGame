#include "Player.cpp"
#include "HelpfulFunctions.cpp"
#include "DisplayTool.cpp"
#include "Camera.cpp" 

#include <iostream>
#include <thread>

using namespace std;

class GameHandler{

    public:

        Renderer* renderer;
        DisplayTool* display_tool;
        Camera* camera;
        Player* player;      

        //bool receiving_user_input {true}; // If we are receiving user input 
        bool run {false}; // If the game should be running
        bool run_thread = true;
        bool start_process {false};

        GameHandler(Renderer &renderer_instance, DisplayTool &display_tool_instance, Camera &camera_instance,
                    Player &player_instance){ 

            renderer = &renderer_instance;
            display_tool = &display_tool_instance;
            camera = &camera_instance;
            player = &player_instance;
        }

        void start(){
            
            renderer->set_selection_color("Blue");
            main_menu();
        }

        void main_menu(){

            Selection selections(vector<string>{"Start Game", "Exit"});
            pair<int, int> last_index {0, 0};

            while(true){

                clear();
                renderer->clear_content();
                renderer->add_content("Main Menu\n\n");

                last_index = display_tool->dynamic_selection(selections, last_index.first);

                if(last_index.second == 1){

                    string choice = selections.content.at(last_index.first);

                    if(choice == "Start Game"){
                        
                        camera->tilemap->add(*player, 1, 1);
                        Enemy* goblin = new Enemy("Goblin", "G", 4, 10, 10, "Green");
                        Wall* wall = new Wall();
                        camera->tilemap->add(*wall, 2, 1);
                        camera->tilemap->add(*wall, 3, 1);
                        camera->tilemap->add(*wall, 4, 1);
                        camera->tilemap->add(*wall, 5, 1);
                        camera->tilemap->add(*wall, 6, 1);
                        camera->tilemap->add(*wall, 7, 1);
                        camera->tilemap->add(*goblin, 1, 0);
                        run = true;
                        start_process = true;
                        game_loop();
                        return;
                    }

                    else if(choice == "Exit"){
                        
                        endwin();
                        exit(0);
                    }
                }
            }
        }

        void refresh_content(){
            renderer->clear_content();
            camera->flush();
            renderer->render();
        }

        void input_loop(){

            while(run_thread){

                if(player->accepting_input){
                    player->handle_input(getch());
                    refresh_content();
                }
            }
        }

        void game_loop(){

            while(true){

                if(run){
                    refresh_content();
                    camera->tilemap->move_all_entities();
                    delay(.2); 
                    continue;
                }      
                delay(1);
            }            
        }
};

