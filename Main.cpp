#include "GameHandler.cpp"
#include "Random.cpp"
#include "HelpfulFunctions.cpp"

void main_input_loop(GameHandler* game_handler){

    game_handler->input_loop();
    
}

int main()
{
    seed_random();

     map<string, BaseCharacter*> map_of_tile_names {
                                    {"Wall", new Wall},
                                    {"Floor", new FloorTile},
                                    {"Goblin", new Enemy("Goblin", "G", 4, 10, 10, 4, "Green")},
                                    {"Void", new EmptyVoid()}
                                };
    
    Renderer renderer;
    DisplayTool display_tool(renderer);
    FloorTile floor_tile;
    Tilemap tilemap(15, 10, floor_tile);
    Player player("Joel", 10, 10, tilemap, display_tool);
    Camera camera(tilemap, renderer, player);
    TilemapLoader tilemap_loader = TilemapLoader("Saves/FirstFloor.txt", map_of_tile_names);

    GameHandler game_handler(renderer, display_tool, camera, player, tilemap_loader);
    thread player_input_thread(main_input_loop, &game_handler); 
    game_handler.start();

    return 0;

}