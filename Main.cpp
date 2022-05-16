#include "GameHandler.cpp"
#include "Random.cpp"
#include "HelpfulFunctions.cpp"


//void main_input_loop(GameHandler* game_handler){

    //game_handler->input_loop();
    
//}

int main()
{
    seed_random();

      map<string, BaseCharacter*> map_of_tile_names {
                                    {"Stone Wall", new StoneWall()},
                                    {"Wood Door", new WoodDoor()},
                                    {"Dirt", new Dirt()},
                                    {"Grass", new Grass()},
                                    {"Water", new Water(5)},
                                    {"Magma", new Magma(5)},
                                    {"Goblin", new Enemy("Goblin", "G", 4, 10, 10, 4, "Green")},
                                    {"Unseeable", new Unseeable()},
                                    {"Void", new EmptyVoid()}
                                };
    
    Renderer renderer;
    DisplayTool display_tool(renderer);
    Tilemap tilemap(0, 0);
    Player player("Joel", 10, 10, tilemap, display_tool);
    Camera camera(tilemap, renderer, player);
    TilemapLoader tilemap_loader = TilemapLoader("Saves/FirstFloor.txt", map_of_tile_names);
    GameHandler game_handler(renderer, display_tool, camera, player, tilemap_loader);
    //thread player_input_thread(main_input_loop, &game_handler); 
    game_handler.start();

    return 0;

}