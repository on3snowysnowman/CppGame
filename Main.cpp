#include "GameHandler.cpp"

int main()
{
    
    Renderer renderer;
    DisplayTool display_tool(renderer);
    FloorTile floor_tile;
    Tilemap tilemap(15, 10, floor_tile);
    Player player("Joel", 10, 10, tilemap, display_tool);
    Camera camera(tilemap, renderer, player);

    GameHandler game_handler(renderer, display_tool, camera, player);
    game_handler.start();

    return 0;

}