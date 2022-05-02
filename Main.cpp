#include "GameHandler.cpp"

void main_input_loop(GameHandler* game_handler){

    game_handler->input_loop();
    
}

int main()
{
    
    Renderer renderer;
    DisplayTool display_tool(renderer);
    FloorTile floor_tile;
    Tilemap tilemap(15, 10, floor_tile);
    Player player("Joel", 10, 10, tilemap, display_tool);
    Camera camera(tilemap, renderer, player);

    GameHandler game_handler(renderer, display_tool, camera, player);
    thread player_input_thread(main_input_loop, &game_handler); 
    game_handler.start();

    return 0;

}