#include "Player.cpp"
#include "DisplayTool.cpp"
#include "Camera.cpp" 

#include <iostream>

class GameHandler{

    public:

        Renderer* renderer;
        DisplayTool* display_tool;
        Camera* camera;
        Player* player;
        

        //bool receiving_user_input {true}; // If we are receiving user input 
        bool run {false}; // If the game should be running

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
                        camera->tilemap->add(*goblin, 4, 4);
                        run = true;
                        input_loop();
                    }

                    else if(choice == "Exit"){
                        
                        endwin();
                        exit(0);
                    }
                }
            }
        }

        void input_loop(){

            int character;

            while(run){

                renderer->clear_content();
                camera->flush();
                renderer->render();
                character = getch();

                player->handle_input(character);
                camera->tilemap->move_all_entities();
                endwin();
                exit(0);

            }            
        }

};
