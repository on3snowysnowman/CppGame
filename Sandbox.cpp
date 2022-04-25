#include "Player.cpp"
#include "Camera.cpp" 

#include "fstream"

class Sandbox{

    public:

        Wall* wall = new Wall();
        FloorTile* floor_tile = new FloorTile();
        Enemy* goblin = new Enemy("Goblin", "G", 4, 10, 10, "Green");

        Renderer renderer;
        DisplayTool display_tool = DisplayTool(renderer);
        Tilemap tilemap = Tilemap(15, 10, *floor_tile);
        Camera camera = Camera(tilemap, renderer);
        Player player = Player("Joel", 10, 10, tilemap, display_tool);

        string targ_path = "Saves/FirstFloor.txt";
        vector<string> choices {"Wall", "Floor", "Goblin", "Close Menu"};

        fstream file_stream;

        map<string, vector<pair<int, int>>> file_contents;
 
        BaseCharacter* current_obj = wall;

        Sandbox(){

            start();
        }

        void start(){

            tilemap.add(player, 5, 5);
            input_loop();
        }

        void input_loop(){
            
            int character;

            while(true){
                
                int xPos = player.xPos;
                int yPos = player.yPos;

                renderer.clear_content();
                camera.flush();
                renderer.render();
                character = getch();

                // Enter Key - Place object
                if(character == 13){

                    tilemap.add(*current_obj, player.xPos, player.yPos);
                }

                // Backspace - Delete object
                else if(character == 8){

                    for(BaseCharacter* element : tilemap.tilemap_data.at(player.yPos).at(player.xPos)){
                        
                        if(element == current_obj){
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

                //a - Move Left
                else if(character == 97){

                    tilemap.move(player, xPos - 1, yPos);
                }

                //m - Menu
                else if(character == 109){
                    menu();
                }

                //i - Select new object menu
                else if(character == 105){

                    select_new_obj_menu();
                }
            }

        }

        void save_file(){
            
            file_contents.clear();

            for(int y = 0; y < tilemap.tilemap_data.size(); y++){

                for(int x = 0; x < tilemap.tilemap_data.at(y).size(); x++){

                    for(int z = 0; z < tilemap.tilemap_data.at(y).at(x).size(); z++){

                        BaseCharacter* targ_obj = tilemap.tilemap_data.at(y).at(x).at(z);

                        if(targ_obj->type_obj != "Player"){

                            if(file_contents.find(targ_obj->name) != file_contents.end()){

                                file_contents[targ_obj->name].push_back(pair<int, int>{x, y});
                                continue;
                            }

                            file_contents[targ_obj->name] = vector<pair<int, int>> {pair<int, int>{x, y}};

                        }
                    }
                }
            }
        
            file_stream.open(targ_path, ios::out);

            int count {0};
            int max {200};

            file_stream << "#" << tilemap.width << ":" << tilemap.height << "\n";

            for(pair<string, vector<pair<int, int>>> element : file_contents){
                

                /*
                if(count + element.first.length() + 2 > max){
                    count = element.first.length() + 2;
                    file_stream << "\n";
                }

                */

                file_stream << "#" << element.first << "\n";
                //count + element.first.length() + 2;

                for(pair<int, int> coordinates : element.second){

                    /*

                    if(count + 4 > max){
                        count = 4;
                        file_stream << "\n";
                    }
        
                    */

                    file_stream << coordinates.first << "," << coordinates.second << "\n";
                    //count += 4;
                }
            }

            file_stream.close();
        }

        void load_file(){


            file_stream.open(targ_path, ios::in);

            if(file_stream.is_open()){
                
                int xPos;
                int yPos;
                int i = 0;
                int width, height;
                string tile_name = "";
                string line;
                string targ_num = "";
                string first_int = "";
                string second_int = "";

                while(getline(file_stream, line)){

                    if(line.substr(0, 1) == "#"){

                        line = line.substr(1, line.length());

                        // Accessing size of the new Tilemap
                        if (isdigit(*line.substr(0, 1).c_str())){
                            
                            width = 0;
                            height = 0;
                            i = 0;

                            targ_num = "";


                            while(line.substr(i, 1) != ":"){
                                targ_num += line.substr(i, 1);
                                i += 1;
                            }

                            width = stoi(targ_num);
                            targ_num = "";
                            i += 1; // Skip past ":"

                            while(i < line.length()){
                                targ_num += line.substr(i, 1);
                                i += 1;
                            }

                            height = stoi(targ_num);

                            tilemap = Tilemap(width, height, *floor_tile);
                            camera.load_tilemap(tilemap);
                            continue;
                        }
                    
                        // Accessing name of a new tile
                        else{

                            i = 0;
                            tile_name = "";

                            while(i < line.length()){
                                tile_name += line.substr(i, 1);
                                i += 1;
                            }

                            continue;

                        }
                    }

                    targ_num = "";
                    i = 0;

                    while(line.substr(i, 1) != ","){
                        targ_num += line.substr(i, 1);
                        i += 1;
                    }

                    i += 1; //Skip past ','

                    xPos = stoi(targ_num);
                    targ_num = "";

                    while(i < line.length()){
                        targ_num += line.substr(i, 1);
                        i += 1;
                    }

                    yPos = stoi(targ_num);

                    if(tile_name == "Wall"){
                        tilemap.add(*wall, xPos, yPos);
                    }

                    else if(tile_name == "Floor"){
                        tilemap.add(*floor_tile, xPos, yPos);
                    }

                    else if(tile_name == "Goblin"){

                        tilemap.add(*goblin, xPos, yPos);
                    }
                }

                if(player.xPos < tilemap.width && player.yPos < tilemap.height){
                    tilemap.add(player, player.xPos, player.yPos);
                }
                else{
                    tilemap.add(player, tilemap.width - 1, tilemap.height - 1);
                }

                file_stream.close();
            }


        }

        void select_new_obj_menu(){

            Selection selections(choices);
            pair<int, int> last_index {0, 0};

            while(true){

                clear();
                renderer.clear_content();

                last_index = display_tool.dynamic_selection(selections, last_index.first, "Choose a new Object");

                if(last_index.second == 1){
                    
                    string choice = selections.content.at(last_index.first);

                    if(choice == "Wall"){
                        current_obj = wall;
                    }

                    else if(choice == "Floor"){
                        current_obj = floor_tile;
                    }

                    else if(choice == "Goblin"){
                        current_obj = goblin;
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
                        save_file();
                    }

                    else if(choice == "Load File"){
                        load_file();
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

};  


int main(){

    Sandbox sandbox;

}