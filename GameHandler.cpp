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
        TilemapLoader* tilemap_loader;
        Camera* camera;
        Player* player;      

        //bool receiving_user_input {true}; // If we are receiving user input
        bool run_thread = true;
        bool lock_refresh = false;

        GameHandler(Renderer &renderer_instance, DisplayTool &display_tool_instance, Camera &camera_instance,
                    Player &player_instance, TilemapLoader& tilemap_loader_instance){ 

            renderer = &renderer_instance;
            display_tool = &display_tool_instance;
            camera = &camera_instance;
            player = &player_instance;
            tilemap_loader = &tilemap_loader_instance;
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
                        
                        //Remove our player from the tilemap so that they are not deconstrcuted in the following delete statment 
                        camera->tilemap->delete_obj(*player);
                        
                        tilemap_loader->load_file(*camera->tilemap);

                        if(player->xPos < camera->tilemap->width && player->yPos < camera->tilemap->height){
                            camera->tilemap->add(*player, player->xPos, player->yPos);
                        }
                        else{
                            camera->tilemap->add(*player, camera->tilemap->width - 1, camera->tilemap->height - 1);
                        }

                        player->load_tilemap(*camera->tilemap);


                        /*
                        camera->tilemap->add(*player, 0, 0);
                        Enemy* goblin = new Enemy("Goblin", "G", 4, 10, 10, 4, "Green");
                        Wall* wall = new Wall();
                        camera->tilemap->add(*wall, 2, 1);
                        camera->tilemap->add(*wall, 3, 1);
                        camera->tilemap->add(*wall, 4, 1);
                        camera->tilemap->add(*wall, 5, 1);
                        camera->tilemap->add(*wall, 6, 1);
                        camera->tilemap->add(*wall, 7, 1);
                        camera->tilemap->add(*goblin, 2, 0);
                        */
                        player->run = true;
                        player->accepting_input = true;
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

        bool refresh_content(){
            
            // If lock_refresh is false, then we can refresh the screen
            if(not lock_refresh){
                // Set the lock to true so if any other function tries to access this method while we are refreshing they can't
                lock_refresh = true;
                renderer->clear_content();
                camera->flush();
                renderer->render();
                lock_refresh = false;
                return true;
            }
            return false;
            
        }

        void input_loop(){

            while(run_thread){

                if(player->accepting_input){
                    player->handle_input(getch());
                    
                    //Wait until we can refresh the screen
                    while(not refresh_content()){
                        delay(.05);
                    }
                    continue;
                }
                delay(.3);
            }
        }
        
        void game_loop(){

            while(true){

                if(player->run){

                    refresh_content();
                    camera->tilemap->move_all_entities();
                    delay(.2); 
                    continue;
                }      
                delay(1);
            }            
        }
};

