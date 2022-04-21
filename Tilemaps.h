#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include <algorithm>
#include "classes.h"

using namespace std;

bool compare(BaseCharacter &objOne, BaseCharacter &objTwo){

    return objOne.priority < objTwo.priority;
}


class Tilemap{

    public:

        //Tilemap instance
        vector<vector<vector<BaseCharacter &>>> tilemap;

        int width;
        int height;

        Tilemap(int targWidth, int targHeight, BaseCharacter &fill){
    
            //Width of tilemap
            width = static_cast<const int>(targWidth);
           
           //Height of tilemap
            height = static_cast<const int>(targHeight);

            create_tilemap(fill);

        }

        void create_tilemap(BaseCharacter &fill){

            for(int i = 0; i < height; i++){
                
                tilemap.push_back(vector<vector<BaseCharacter &>>{});

                for(int j = 0; j < width; j++){

                    tilemap.at(i).push_back(vector<BaseCharacter &>{});
                    tilemap.at(i).at(j).push_back(fill);

                }
            }

        }

        void add(BaseCharacter &objectToAdd, int x, int y){

            /*
            Adds a BaseCharacter object to the tilemap and sort the array where it is inserted

            param objectToAdd: Instance of BaseCharacter that we are adding
            param: x = X coordinate to place BaseCharacter Object
            param: y = Y coordinate to place BaseCharacter Object
            */  
            
            tilemap.at(y).at(x).push_back(objectToAdd); 
            objectToAdd.xPos = x;
            objectToAdd.yPos = y;

            sort(tilemap.at(y).at(x).begin(), tilemap.at(y).at(x).end(), compare);

        }

        void move(Entity &objectToMove, int x, int y){

            /*
            Moves an object to a different location in the tilemap

            param: objectToMove = The object we are moving
            param: x = X coordinate to move the object to
            param: y = Y coordinate to move the object to

            */
            int coordinates[] = {0, 0, 0, 0};
            int* objectLocation = {find(objectToMove, objectToMove.xPos, objectToMove.yPos)};

            if(objectLocation[3] == 0){
                system("cls");
                cout<< "Object called to move was not found";
                exit(0);
            }

            if (bound_check(x, y)){

                tilemap.at(objectLocation[1]).at(objectLocation[0])
                .erase(tilemap.at(objectLocation[1]).at(objectLocation[0]).begin() + objectLocation[2]);
                
                add(objectToMove, x, y);
                objectToMove.set_position(x, y);

            }
                
        }

        bool bound_check(int x, int y){

            /*
            Checks whether the passed coordinates are within the tilemaps boundaries

            param: x = X coordinate we are checking
            param: y = Y coordinate we are checking

            */

           return(y >= 0 && y < tilemap.size() && x >= 0 && x < tilemap.at(y).size());

        }

        int* find(BaseCharacter &objectToFind, int x = -1, int y = -1){

            /*
            Returns an array of coordinates of where the object was found. Takes in an x an y value to speed up search time if we 
            already have a decent idea of where the object is

            param : objectToFind = Object we want to find
            param: x coordinate to search in
            param: y coordinate to serach in
            return: a tuple of coordinates of where the object was found.
            */

            int coordinateArray[4];
           
            if(y != -1 && x != -1){

                for(int k = 0; k < tilemap.at(y).at(x).size(); k++){

                    if(&getObj(x, y, k) == &objectToFind){

                        coordinateArray[0] = x;
                        coordinateArray[1] = y;
                        coordinateArray[2] = k;
                        coordinateArray[3] = 1;

                        return coordinateArray;

                    }

                }

            }

            else if(y != -1){

               for(int j = 0; j < tilemap.at(y).size(); j++){

                    for(int k = 0; k < tilemap.at(y).at(j).size(); k++){

                        if(&getObj(j, y, k) == &objectToFind){

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

                for(int i = 0; i < tilemap.size(); i++){
                    
                    for(int k = 0; i < tilemap.at(i).at(x).size(); k++){
                        
                        if(&getObj(x, i, k) == &objectToFind){

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

                for(int i = 0; i < tilemap.size(); i++){

                    for(int j = 0; j < tilemap.at(i).size(); j++){

                        for(int k = 0; k < tilemap.at(i).at(j).size(); k++){

                            if(&getObj(j, i, k) == &objectToFind){

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
        
        BaseCharacter &getObj(int x, int y, int z){

            /*
            Returns a BaseCharacter Object at the specified coordinates of the tilemap

            param: x = X coordinate to return value from
            param: y = Y coordinate to return value from
            param: z = Z coordinate to return value from
            */

            return tilemap.at(y).at(x).at(z);
        }

};

