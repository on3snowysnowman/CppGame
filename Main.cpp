#include "DisplayTool.cpp"
#include "Player.cpp"
#include "Camera.cpp"

int main()
{
    
    Renderer renderer;
    DisplayTool display_tool(&renderer);
    FloorTile floor_tile;
    Tilemap tilemap(15, 10, floor_tile);
    Camera camera(tilemap, renderer);

    Player player("Joel", 10, 10, tilemap, display_tool);

    renderer.set_selection_color("Blue");

    Selection selections(vector<string>{"Start Game", "Exit"});
    pair<int, int> last_index {0, 0};

    while(true){

        clear();
        renderer.clear_content();
        renderer.add_content("Main Menu\n\n");

        last_index = display_tool.dynamic_selection(selections, last_index.first);

        if(last_index.second == 1){

            string choice = selections.content.at(last_index.first);

            if(choice == "Start Game"){
                

                Entity goblin("Goblin", "G", 2, 10, 10, "Green");
                Weapon sword;

                int character;

                tilemap.add(player, 1, 1);
                tilemap.add(goblin, 4, 4);
                tilemap.add(sword, 3, 1);

                while(true){

                    renderer.clear_content();

                    camera.flush();
                    renderer.render();
                    character = getch();

                    player.handle_input(character);

                }
            }

            else if(choice == "Exit"){
                
                endwin();
                exit(0);
            }

        }
    
    }

    return 0;

}