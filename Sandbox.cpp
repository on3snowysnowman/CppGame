#include "Player.cpp"
#include "Camera.cpp" 

#include <fstream>

class Sandbox{

    public:

        Renderer renderer;
        DisplayTool display_tool = DisplayTool(renderer);
        Tilemap tilemap = Tilemap(0, 0);
        Camera camera = Camera(tilemap, renderer, player);
        Player player = Player("Constructor", 10, 10, tilemap, display_tool);

        string targ_path = "Saves/FirstFloor.txt";
        vector<string> main_choices = {"Ground/Floor", "Vegetation", "Walls", "Liquids", "Close Menu"};
        vector<string> floors = {"Dirt", "Void", "Unseeable", " <- Back"};
        vector<string> vegetation = {"Grass", " <- Back"};
        vector<string> walls = {"Stone Wall", "Wood Door"," <- Back"};
        vector<string> liquids = {"Water", "Magma"," <- Back"};

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

        TilemapLoader tilemap_loader = TilemapLoader("Saves/FirstFloor.txt", map_of_tile_names);
 
        BaseCharacter* current_obj = map_of_tile_names["Stone Wall"];

        Sandbox(){

            start();
        }

        void start(){

            //for(pair<string, BaseCharacter*> element: map_of_tile_names){
                //choices.push_back(element.first);
            //}
            //choices.push_back("Close Menu");

            tilemap_loader.load_file(tilemap);
            player.set_god_mode(true);
            tilemap.add(player, 0, 0);
            input_loop();
        }

        void input_loop(){
            
            int character;

            while(true){
                
                int xPos = player.xPos;
                int yPos = player.yPos;

                renderer.clear_content();
                camera.flush();
                renderer.add_new_line(2);

                renderer.add_content("Selected Object: ", false);
                renderer.add_content(current_obj->name, false, renderer.selection_color);
                renderer.add_content(" (", false);
                renderer.add_content(current_obj->character, false, current_obj->color);
                renderer.add_content(")");
                renderer.render();
                character = getch();

                // Enter Key - Place object
                if(character == 13){

                    if(not current_obj->can_place_twice){
                        if(not tilemap.targ_is_in_vector(current_obj->name, "name", xPos, yPos)){
                            
                            BaseCharacter* targ_obj;

                            if(current_obj->type_obj == "Entity"){

                                targ_obj = new Entity(*(Entity *) current_obj);
                            }

                            else if(current_obj->type_obj == "Liquid"){

                                targ_obj = new Liquid(*(Liquid *) current_obj);
                            }

                            else{
                                
                                targ_obj = new BaseCharacter(*current_obj);
                            }

                            tilemap.add(*targ_obj, player.xPos, player.yPos);
                        }
                    }                                        
                }

                // Backspace - Delete object
                else if(character == 8){

                    for(BaseCharacter* element : tilemap.tilemap_data.at(player.yPos).at(player.xPos)){
                        
                        if(element->name == current_obj->name){
                            tilemap.delete_obj(*element, xPos, yPos);
                            break;
                        }
                    }
                }

                // w - Move Up
                else if(character == 119){
                    
                    tilemap.move(player, xPos, yPos - 1);
                }

                // d - Move Right
                else if(character == 100){

                    tilemap.move(player, xPos + 1, yPos);
                }

                // s - Move Down
                else if(character == 115){

                    tilemap.move(player, xPos, yPos + 1);
                }

                // a - Move Left
                else if(character == 97){

                    tilemap.move(player, xPos - 1, yPos);
                }

                // Escape - Menu
                else if(character == 27){
                    menu();
                }

                // i - Select new object menu
                else if(character == 105){

                    select_new_obj_menu();
                }

                // f - Fill tilemap with selected object
                else if(character == 102){

                    Selection selections(vector<string>{"Yes", "Cancel"});
                    pair<int, int> last_index{0, 0};

                    while(true){

                        clear();
                        renderer.clear_content();
                        
                        renderer.add_content("Fill tilemap with: ", false);
                        renderer.add_content(current_obj->name, false, renderer.selection_color);
                        renderer.add_content("?");
                        last_index = display_tool.dynamic_selection(selections, last_index.first);

                        if(last_index.second == 1){

                            string choice = selections.content.at(last_index.first);

                            if(choice == "Yes"){
                                tilemap.fill(*current_obj);
                                break;
                            }

                            break;
                        }
                    }
                }

                // c -  Clear menu
                else if(character == 99){

                    clear_menu();
                }

                // . - Increase Radius
                else if(character == 46){
                    camera.increase_radius();
                }

                // , - Decrease Radius
                else if(character == 44){
                    camera.decrease_radius();
                }

                // [ - Add row to tilemap
                else if(character == 91){

                    tilemap.add_row();
                }

                // ] - Add coloumn to tilemap
                else if(character == 93){

                    tilemap.add_column();
                }

            }

        }

        void select_new_obj_menu(){

            Selection selections(main_choices);
            pair<int, int> last_index {0, 0};

            while(true){

                clear();
                renderer.clear_content();
                renderer.add_content("Current Selected Object: ", false);
                renderer.add_content(current_obj->name, false, renderer.selection_color);
                renderer.add_content(" (", false);
                renderer.add_content(current_obj->character, false, current_obj->color);
                renderer.add_content(")");
                renderer.add_new_line();

                last_index = display_tool.dynamic_selection(selections, last_index.first);

                if(last_index.second == 1){
                    
                    string choice = selections.content.at(last_index.first);

                    if(choice == "Ground/Floor"){

                        Selection second_selections(floors);
                        pair<int, int> second_last_index {0, 0};

                        while(true){
                            
                            clear();
                            renderer.clear_content();
                            renderer.add_content("Current Selected Object: ", false);
                            renderer.add_content(current_obj->name, false, renderer.selection_color);
                            renderer.add_content(" (", false);
                            renderer.add_content(current_obj->character, false, current_obj->color);
                            renderer.add_content(")");
                            renderer.add_new_line();

                            second_last_index = display_tool.dynamic_selection(second_selections, second_last_index.first, "Select a new object");

                            if(second_last_index.second == 1){

                                choice = second_selections.content.at(second_last_index.first);

                                if(map_of_tile_names.find(choice) != map_of_tile_names.end()){
                                    current_obj = map_of_tile_names[choice];
                                    return;
                                }

                                else if(choice == " <- Back"){
                                    break;
                                }
                            }

                        }

                    }

                    else if(choice == "Vegetation"){

                        Selection second_selections(vegetation);
                        pair<int, int> second_last_index {0, 0};

                        while(true){
                            
                            clear();
                            renderer.clear_content();
                            renderer.add_content("Current Selected Object: ", false);
                            renderer.add_content(current_obj->name, false, renderer.selection_color);
                            renderer.add_content(" (", false);
                            renderer.add_content(current_obj->character, false, current_obj->color);
                            renderer.add_content(")");
                            renderer.add_new_line();

                            second_last_index = display_tool.dynamic_selection(second_selections, second_last_index.first, "Select a new object");

                            if(second_last_index.second == 1){

                                choice = second_selections.content.at(second_last_index.first);

                                if(map_of_tile_names.find(choice) != map_of_tile_names.end()){
                                    current_obj = map_of_tile_names[choice];
                                    return;
                                }

                                else if(choice == " <- Back"){
                                    break;
                                }
                            }

                        }
                    }

                    else if(choice == "Walls"){

                        Selection second_selections(walls);
                        pair<int, int> second_last_index {0, 0};

                        while(true){
                            
                            clear();
                            renderer.clear_content();
                            renderer.add_content("Current Selected Object: ", false);
                            renderer.add_content(current_obj->name, false, renderer.selection_color);
                            renderer.add_content(" (", false);
                            renderer.add_content(current_obj->character, false, current_obj->color);
                            renderer.add_content(")");
                            renderer.add_new_line();

                            second_last_index = display_tool.dynamic_selection(second_selections, second_last_index.first, "Select a new object");

                            if(second_last_index.second == 1){

                                choice = second_selections.content.at(second_last_index.first);

                                if(map_of_tile_names.find(choice) != map_of_tile_names.end()){
                                    current_obj = map_of_tile_names[choice];
                                    return;
                                }

                                else if(choice == " <- Back"){
                                    break;
                                }
                            }

                        }
                    }

                    else if(choice == "Liquids"){

                        Selection second_selections(liquids);
                        pair<int, int> second_last_index {0, 0};

                        while(true){
                            
                            clear();
                            renderer.clear_content();
                            renderer.add_content("Current Selected Object: ", false);
                            renderer.add_content(current_obj->name, false, renderer.selection_color);
                            renderer.add_content(" (", false);
                            renderer.add_content(current_obj->character, false, current_obj->color);
                            renderer.add_content(")");
                            renderer.add_new_line();

                            second_last_index = display_tool.dynamic_selection(second_selections, second_last_index.first, "Select a new object");

                            if(second_last_index.second == 1){

                                choice = second_selections.content.at(second_last_index.first);

                                if(map_of_tile_names.find(choice) != map_of_tile_names.end()){
                                    current_obj = map_of_tile_names[choice];
                                    return;
                                }

                                else if(choice == " <- Back"){
                                    break;
                                }
                            }

                        }
                    }

                    else if(choice == "Close Menu"){
                        return;
                    }
                }
            
            }

        }

        void menu(){

            Selection selections(vector<string>{"Save File", "Load File", "Close Menu", "Exit"});
            pair<int, int> last_index {0, 0};

            while(true){

                clear();
                renderer.clear_content();

                last_index = display_tool.dynamic_selection(selections, last_index.first, "Select one of the following");

                if(last_index.second == 1){

                    string choice = selections.content.at(last_index.first);

                    if(choice == "Save File"){
                        tilemap_loader.save_file(tilemap);
                    }

                    else if(choice == "Load File"){
                        
                        //Remove our player from the tilemap so that they are not deconstrcuted in the following delete statment 
                        tilemap.delete_obj(player);
                        
                        tilemap_loader.load_file(tilemap);

                        if(player.xPos < tilemap.width && player.yPos < tilemap.height){
                            tilemap.add(player, player.xPos, player.yPos);
                        }
                        else{
                            tilemap.add(player, tilemap.width - 1, tilemap.height - 1);
                        }

                        camera.load_tilemap(tilemap);
                        player.load_tilemap(tilemap);
                    }

                    else if(choice == "Close Menu"){
                        return;
                    }

                    else if(choice == "Exit"){
                        endwin();
                        exit(0);
                    }
                    
                }

            }
        }

        void clear_menu(){

            Selection selections(vector<string>{"Clear Selected Item", "Clear Entire Map"});
            pair<int, int> last_index {0, 0};

            while(true){

                clear();
                renderer.clear_content();

                last_index = display_tool.dynamic_selection(selections, last_index.first, "Choose one of the Following");

                if(last_index.second == 1){

                    string choice = selections.content.at(last_index.first);

                    if(choice == "Clear Selected Item"){
                        tilemap.delete_all_of_obj(*current_obj, true);
                        return;
                    }

                    else if(choice == "Clear Entire Map"){

                        tilemap.delete_all({&player}, true);
                        return;
                    }
                }
            }
        }
};  


int main(){

    seed_random();
    Sandbox sandbox;

}