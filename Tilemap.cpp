#ifndef TILEMAP
#define TILEMAP

#include "ObjectClasses/BaseClasses.cpp"
#include "Random.cpp"

#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <curses.h>
#include <fstream>


string remove_character(string string_to_strip, char black_list){

    string final_string {""};

    for(char element : string_to_strip){
        if(element != black_list){
            final_string += element;
        }
    }

    return final_string;
}

bool compare(BaseCharacter* obj_one, BaseCharacter* obj_two){
    return obj_one->priority > obj_two->priority;
}


class Tilemap{

    public:

        vector<vector<vector<BaseCharacter*>>> tilemap_data; // 3D Tilemap holding our data
        vector<BaseCharacter*> entities_in_tilemap; // Vector containing all known entities inside of our tilemap. This saves us time so we don't have 
                                            // to search for them every time the input loop resets 

        int width, height;
        
        Tilemap(int targ_width, int targ_height){
            
            width = targ_width;
            height = targ_height;

            create_tilemap();
        }

        Tilemap(int targ_width, int targ_height, BaseCharacter &fill){

            width = targ_width; // Width of the tilemap
            height = targ_height; // Height of the tilemap

            create_tilemap(fill);
        }

        void create_tilemap(BaseCharacter &fill){

            for(int i = 0; i < height; i++){
                
                tilemap_data.push_back(vector<vector<BaseCharacter *>>{});

                for(int j = 0; j < width; j++){

                    tilemap_data.at(i).push_back(vector<BaseCharacter *>{});
                    tilemap_data.at(i).at(j).push_back(&fill);
                }
            }

        }

        bool targ_is_in_vector(string name, string targ, int x, int y){

            /*
            Takes in a name, a string that represents how we will conduct our search and a pair of coordinates. A search is conducted on the vector inside our tilemap at the 
            passed coordinates. If any object's attribute inside the searched tilemap matches the passed string, the function returns true. Else false.
            */

            if(targ == "name"){

                for(int z = 0; z < tilemap_data.at(y).at(x).size(); z++){
                    if(getObj(x, y, z)->name == name){
                        return true;
                    }
                }

                return false;
            }

            else if(targ == "type"){
                
                for(int z = 0; z < tilemap_data.at(y).at(x).size(); z++){
                    if(getObj(x, y, z)->type_obj == name){
                        return true;
                    }
                }

                return false;
            }

            return false;
        }

        void fill(BaseCharacter &fill, bool no_duplicates = true){
            
            if(no_duplicates){
                
                for(int y = 0; y < height; y++){

                    for(int x = 0; x < width; x++){
                        
                        if(not targ_is_in_vector(fill.name, "name", x, y)){
                            
                            add(fill, x, y);
                            continue;
                        }
                    }
                }

                return;
            }

            for(int y = 0; y < height; y++){

                for(int x = 0; x < width; x++){
                    
                    add(fill, x, y);
                }
            }
        }

        void create_tilemap(){

            for(int i = 0; i < height; i++){
                
                tilemap_data.push_back(vector<vector<BaseCharacter *>>{});

                for(int j = 0; j < width; j++){

                    tilemap_data.at(i).push_back(vector<BaseCharacter *>{});

                }
            }

        }

        void add(BaseCharacter& object_to_add, int x, int y){
            
            /*
            Adds a BaseCharacter object to the tilemap and sort the array where it is inserted

            param objectToAdd: Instance of BaseCharacter that we are adding
            param: x = X coordinate to place BaseCharacter Object
            param: y = Y coordinate to place BaseCharacter Object
            */  
           
            // If this object is of Entity type, add it to our vector of known entities
            if(object_to_add.type_obj == "Entity"){

                //cout << object_to_add.name << " is an Entity. Add it\n";
                entities_in_tilemap.push_back(&object_to_add);
            }

            tilemap_data.at(y).at(x).push_back(&object_to_add);
            object_to_add.set_position(x, y);
            sort(tilemap_data.at(y).at(x).begin(), tilemap_data.at(y).at(x).end(), compare);
            
        }

        void add_row(){
            
            /*
            Adds a new row to the bottom of the tilemap
            */

            vector<vector<BaseCharacter*>> vector_to_add;

            for(int i = 0; i < width; i++){
                vector_to_add.push_back(vector<BaseCharacter*>{});
            }

            height += 1;
            tilemap_data.push_back(vector_to_add);
        }

        void add_column(){

            /*
            Adds a new row to the bottom of the tilemap
            */

            for(int i = 0; i < height; i++){
                tilemap_data.at(i).push_back(vector<BaseCharacter*>{});
            }

            width += 1;
        }

        void delete_obj(BaseCharacter &object_to_delete, int xPos = -1, int yPos = -1, bool deconstruct = false){

            int z {0};

            if(xPos == -1){
                xPos = object_to_delete.xPos;
            }

            if(yPos == -1){
                yPos = object_to_delete.yPos;
            }

            for(BaseCharacter* element : tilemap_data.at(yPos).at(xPos)){

                if(element == &object_to_delete){
                    
                    if(object_to_delete.type_obj == "Entity"){
                        entities_in_tilemap.erase(
                            remove(entities_in_tilemap.begin(), entities_in_tilemap.end(), &object_to_delete), entities_in_tilemap.end());;
                    }

                    tilemap_data.at(yPos).at(xPos).
                        erase(tilemap_data.at(yPos).at(xPos).begin() + z);
                    
                    if(deconstruct){
                        delete &object_to_delete;
                    }
                    
                    return;
                }

                z += 1;
            }

        }

        void delete_all_of_obj(BaseCharacter& object_to_delete, bool deconstruct = false){
            
            int x, y = 0;

            vector<BaseCharacter*> objects_to_deconstruct;

            for(vector<vector<BaseCharacter*>> row : tilemap_data){

                x = 0;

                for(vector<BaseCharacter*> column : row){

                    for(BaseCharacter* stack : column){

                        if(stack->name == object_to_delete.name){

                            delete_obj(*stack, x, y, false);
                            if(find(objects_to_deconstruct.begin(), objects_to_deconstruct.end(), stack) == objects_to_deconstruct.end()){
                                objects_to_deconstruct.push_back(stack);
                            }
                        }

                    }

                    x += 1;
                }
                
                y += 1;
            }

            if(not deconstruct){
                return;
            }

            BaseCharacter* targ_obj;

            for(int i = 0; i < objects_to_deconstruct.size(); i++){
                
                targ_obj = objects_to_deconstruct.at(i);
                objects_to_deconstruct.erase(objects_to_deconstruct.begin() + i);
                delete targ_obj;
            }

        }

        void delete_all(vector<BaseCharacter*> black_list, bool deconstruct = false){

            int x, y = 0;

            vector<BaseCharacter*> objects_to_deconstruct;

            for(vector<vector<BaseCharacter*>> row : tilemap_data){

                x = 0;

                for(vector<BaseCharacter*> column : row){

                    for(BaseCharacter* stack : column){
                       
                        if(find(black_list.begin(), black_list.end(), stack) == black_list.end()){

                            if(find(objects_to_deconstruct.begin(), objects_to_deconstruct.end(), stack) == objects_to_deconstruct.end()){
                                objects_to_deconstruct.push_back(stack);
                            }
                            delete_obj(*stack, x, y, false);
                        }

                        cout << "\n";
                    }

                    x += 1;
                }
                
                y += 1;
            }

            if(not deconstruct){
                return;
            }

            cout << "Before Deconstruct\n";

            BaseCharacter* targ_obj;

            for(int i = 0; i < objects_to_deconstruct.size(); i++){
                
                targ_obj = objects_to_deconstruct.at(i);
                objects_to_deconstruct.erase(objects_to_deconstruct.begin() + i);
                delete targ_obj;
            }
        }

        void move(BaseCharacter &object_to_move, int x, int y){
            
            /*
            Moves an Entity to a different location in the tilemap

            param: objectToMove = The object we are moving
            param: x = X coordinate to move the object to
            param: y = Y coordinate to move the object to

            */

            int coordinates[] {0, 0, 0, 0};
            int* objectLocation = find_obj(object_to_move, coordinates, object_to_move.xPos, object_to_move.yPos);

            // If the object was found
            if(objectLocation[3] == 1){

                if(bound_check(x, y, object_to_move.ignore_non_traversables)){

                    delete_obj(object_to_move);

                    add(object_to_move, x, y);
                    object_to_move.set_position(x, y);

                }
            }

        }

        void move_all_entities(){

            for(BaseCharacter* element : entities_in_tilemap){

                if(element->should_move){
                    
                    vector<pair<int, int>> move_options;

                    Entity *new_obj = (Entity *) *&element;

                    // Get a random number
                    int random_num = get_random_num(1, 10);

                    // If the random number is less than or equal to the chance to move given by the entity, then we shall move the entity
                    if(random_num <= new_obj->move_chance){

                        int y_max = new_obj->yPos + 1;
                        int x_max = new_obj->xPos + 1;

                        for(int y = new_obj->yPos - 1; y <= y_max; y++){

                            for(int x = new_obj->xPos - 1; x <= x_max; x++){

                                // Trying to access this object's tile
                                if(y != new_obj->yPos || x != new_obj->xPos){

                                    if(bound_check(x, y)){

                                        move_options.push_back(pair<int, int>(x, y));
                                    }
                                }
                            }
                        }
                        
                        if(move_options.size() == 0){
                            continue;
                        }

                        else if(move_options.size() == 1){
                            pair<int, int> targ_move = move_options.at(0);
                            move(*element, targ_move.first, targ_move.second);
                            continue;
                        }

                        pair<int, int> targ_move = move_options.at(get_random_num(0, move_options.size()));
                        move(*element, targ_move.first, targ_move.second);
                    }
                }
            }
        }

        bool bound_check(int x, int y, bool ignore_non_traversables = false){

            /*
            Checks whether the passed coordinates are within the tilemaps boundaries

            param: x = X coordinate we are checking
            param: y = Y coordinate we are checking

            */

            bool found_non_traversable {false}; // Whether a non traversable object resides in the coordinates we are bound checking

            if(y >= 0 && y < tilemap_data.size() && x >= 0 && x < tilemap_data.at(y).size()){

                if(not ignore_non_traversables){

                    for(BaseCharacter* element : tilemap_data.at(y).at(x)){

                        // If this object's traversable bool is false
                        if(not element->traversable){
                            return false;
                        }
                    }

                }

                return true;
            }

             return false;
        }

        int* find_obj(BaseCharacter &object_to_find, int coordinateArray[3], int x = -1, int y = -1){

            /*
            Returns an array of coordinates of where the object was found. Takes in an x an y value to speed up search time if we 
            already have a decent idea of where the object is

            param : objectToFind = Object we want to find
            param : coordinateArray = Array we are filling with the 3 values of the Object's position
            param: x coordinate to search in
            param: y coordinate to serach in
            return: a tuple of coordinates of where the object was found.
            */

            if(y != -1 && x != -1){

                for(int k = 0; k < tilemap_data.at(y).at(x).size(); k++){

                    if(getObj(x, y, k) == &object_to_find){

                        coordinateArray[0] = x;
                        coordinateArray[1] = y;
                        coordinateArray[2] = k;
                        coordinateArray[3] = 1; 

                        return coordinateArray;

                    }

                }

            }

            else if(y != -1){

               for(int j = 0; j < tilemap_data.at(y).size(); j++){

                    for(int k = 0; k < tilemap_data.at(y).at(j).size(); k++){

                        if(getObj(j, y, k) == &object_to_find){

                            coordinateArray[0] = j;
                            coordinateArray[1] = y;
                            coordinateArray[2] = k;
                            coordinateArray[3] = 1;

                            return coordinateArray;
                        }
                    }
                }
           }

            else if(x != -1){

                for(int i = 0; i < tilemap_data.size(); i++){
                    
                    for(int k = 0; i < tilemap_data.at(i).at(x).size(); k++){
                        
                        if(getObj(x, i, k) == &object_to_find){

                            coordinateArray[0] = x;
                            coordinateArray[1] = i;
                            coordinateArray[2] = k;
                            coordinateArray[3] = 1;

                            return coordinateArray;

                        }

                    }
                }

            }

            else{

                for(int i = 0; i < tilemap_data.size(); i++){

                    for(int j = 0; j < tilemap_data.at(i).size(); j++){

                        for(int k = 0; k < tilemap_data.at(i).at(j).size(); k++){

                            if(getObj(j, i, k) == &object_to_find){

                                coordinateArray[0] = j;
                                coordinateArray[1] = i;
                                coordinateArray[2] = k;
                                coordinateArray[3] = 1;

                                return coordinateArray;
                            }

                        }
                    }
                }
            }

            
            return coordinateArray;
        }

        BaseCharacter* find_obj_by_name(string name){
            
            for(int i = 0; i < tilemap_data.size(); i++){

                    for(int j = 0; j < tilemap_data.at(i).size(); j++){

                        for(int k = 0; k < tilemap_data.at(i).at(j).size(); k++){

                            BaseCharacter* targ_obj = getObj(j, i, k);

                            if(targ_obj->name == name){

                                return targ_obj;
                            }
                        }
                    }
                }

            return NULL;
        }

        BaseCharacter* getObj(int x, int y, int z){

            /*
            Returns a BaseCharacter Object at the specified coordinates of the tilemap

            param: x = X coordinate to return value from
            param: y = Y coordinate to return value from
            param: z = Z coordinate to return value from
            */

            return tilemap_data.at(y).at(x).at(z);

        }

};


class TilemapLoader{

    string targ_path;

    fstream file_stream;

    map<string, vector<pair<pair<int, int>, BaseCharacter*>>> file_contents;
    map<string, BaseCharacter*> map_of_tile_names;

    public:

        TilemapLoader(string new_targ_path, map<string, BaseCharacter*>& targ_map_of_tile_names){

            targ_path = new_targ_path;
            map_of_tile_names = targ_map_of_tile_names;

        }

        
        void save_file(Tilemap& tilemap){
            
            file_contents.clear();

            BaseCharacter* targ_obj;

            for(int y = 0; y < tilemap.tilemap_data.size(); y++){

                for(int x = 0; x < tilemap.tilemap_data.at(y).size(); x++){

                    for(int z = 0; z < tilemap.tilemap_data.at(y).at(x).size(); z++){
                        
                        //targ_obj = tilemap.tilemap_data.at(y).at(x).at(z);
                        targ_obj = tilemap.getObj(x, y, z);

                        if(targ_obj->type_obj != "Player"){

                            if(file_contents.find(targ_obj->name) != file_contents.end()){
                                
                                file_contents[targ_obj->name].push_back(pair<pair<int, int>, BaseCharacter*>{pair<int, int>{x, y}, targ_obj});
                                continue;
                            }
                            
                            file_contents[targ_obj->name] = vector<pair<pair<int, int>, BaseCharacter*>>{{pair<int, int>{x, y}, targ_obj}};
                            continue;
                            
                        }

                    }
                }
            }
        

            file_stream.open(targ_path, ios::out);

            if(file_stream.is_open()){

                int count {0};
                int max {200};

                file_stream << "#" << tilemap.width << ":" << tilemap.height << "\n";

                for(pair<string, vector<pair<pair<int, int>, BaseCharacter*>>> element : file_contents){
                    

                    /*
                    if(count + element.first.length() + 2 > max){
                        count = element.first.length() + 2;
                        file_stream << "\n";
                    }

                    */

                    file_stream << "#" << element.first << "\n";
                    //count + element.first.length() + 2;

                    for(pair<pair<int, int>, BaseCharacter*> coordinates : element.second){

                        /*

                        if(count + 4 > max){
                            count = 4;
                            file_stream << "\n";
                        }
            
                        */

                        file_stream << coordinates.first.first << "," << coordinates.first.second << " #" << coordinates.second << "\n";
                        //count += 4;
                    }
                }

                file_stream.close();

            }
        }

        Tilemap load_file(Tilemap& tilemap){

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

                //BaseCharacter* new_obj;

                getline(file_stream, line);
                
                if(line.length() == 0){
                    tilemap = Tilemap(10, 10);
                    return tilemap;
                }

                // Accessing size of the new Tilemap
                width = 0;
                height = 0;
                i = 1;

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

                //Loop through the Tilemap and delete the objects from memory so we don't have memory leaks
                for(int y = 0; y < tilemap.height; y++){

                    for(int x = 0; x < tilemap.tilemap_data.at(y).size(); x++){

                        for(int z = 0; z < tilemap.tilemap_data.at(y).at(x).size(); z++){

                            tilemap.delete_obj(*tilemap.getObj(x, y, z), true);
                        }
                    }
                }

                tilemap = Tilemap(width, height);

                // Start parsing text file
                while(getline(file_stream, line)){

                    //line = remove_character(line, ' ');

                    if(line.length() > 0){

                        if(line.substr(0, 1) == "#"){

                            line = line.substr(1, line.length());

                            i = 0;
                            tile_name = "";

                            while(i < line.length()){
                                tile_name += line.substr(i, 1);
                                i += 1;
                            }

                            continue;
                        }

                        targ_num = "";
                        i = 0;

                        while(i < line.length() && line.substr(i, 1) != ","){
                            targ_num += line.substr(i, 1);
                            i += 1;
                        }

                        i += 1; //Skip past ','

                        xPos = stoi(targ_num);
                        targ_num = "";

                        while(i < line.length() && line.substr(i, 1) != "#"){
                            targ_num += line.substr(i, 1);
                            i += 1;
                        }

                        yPos = stoi(targ_num);
                        
                        BaseCharacter* targ_obj = map_of_tile_names[tile_name];
                        BaseCharacter* new_obj;

                        if (targ_obj->type_obj == "Entity"){
                            new_obj = new Entity(*(Entity *) targ_obj);
                        }


                        else if(targ_obj->type_obj == "Liquid"){

                            new_obj = new Liquid(*(Liquid *) targ_obj);
                        }

                        else{
                            new_obj = new BaseCharacter(*targ_obj); 
                        }
                        

                        tilemap.add(*new_obj, xPos, yPos);

                    }
                }

                file_stream.close();
            }

            return tilemap;

        }


};

#endif