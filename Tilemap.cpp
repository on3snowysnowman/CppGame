#ifndef TILEMAP
#define TILEMAP

#include "ObjectClasses/BaseClasses.cpp"
#include <vector>
#include <algorithm>
#include <iostream>
#include <curses.h>


bool compare(BaseCharacter* obj_one, BaseCharacter* obj_two){
    return obj_one->priority > obj_two->priority;
}

class Tilemap{

    public:

        vector<vector<vector<BaseCharacter*>>> tilemap_data; // 3D Tilemap holding our data
        vector<BaseCharacter*> entities_in_tilemap; // Vector containing all known entities inside of our tilemap. This saves us time so we don't have 
                                            // to search for them every time the input loop resets 

        int width, height;

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

        void add(BaseCharacter &object_to_add, int x, int y){
            
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

        void delete_obj(BaseCharacter &object_to_delete, bool deconstruct = false){

            int z {0};

            for(BaseCharacter* element : tilemap_data.at(object_to_delete.yPos).at(object_to_delete.xPos)){

                if(element == &object_to_delete){
                    
                    if(object_to_delete.type_obj == "Entity"){
                        entities_in_tilemap.erase(
                            remove(entities_in_tilemap.begin(), entities_in_tilemap.end(), &object_to_delete), entities_in_tilemap.end());;
                    }

                    tilemap_data.at(object_to_delete.yPos).at(object_to_delete.xPos).
                        erase(tilemap_data.at(object_to_delete.yPos).at(object_to_delete.xPos).begin() + z);
                    
                    if(deconstruct){
                        delete &object_to_delete;
                    }
                    
                    return;
                }

                z += 1;
            }

        }

        void move(Entity &object_to_move, int x, int y){
            
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
            
                if (bound_check(x, y)){

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
                    new_obj->random_move();

                    for(int y = new_obj->yPos - 1; y < y + 3; y++){

                        for(int x = new_obj->xPos - 1; x < x + 3; x++){

                            // Trying to access this object's tile
                            if(y != 1 || x != 1){

                                if(bound_check(x, y)){

                                    move_options.push_back(pair<int, int>(x, y));
                                }

                            }
                        }
                    }

                    for(pair<int, int> element: move_options){

                        cout << element.first << ", " << element.second << "\n";
                    }
                    
                    endwin();
                    exit(0);

                }
            }

        }

        bool bound_check(int x, int y){

            /*
            Checks whether the passed coordinates are within the tilemaps boundaries

            param: x = X coordinate we are checking
            param: y = Y coordinate we are checking

            */

            bool found_non_traversable {false}; // Whether a non traversable object resides in the coordinates we are bound checking

            if(y >= 0 && y < tilemap_data.size() && x >= 0 && x < tilemap_data.at(y).size()){

                for(BaseCharacter* element : tilemap_data.at(y).at(x)){

                    // If this object's traversable bool is false
                    if(not element->traversable){
                        return false;
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

#endif