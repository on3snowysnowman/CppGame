#include "BaseClasses.cpp"

class FloorTile: public BaseCharacter{

    public:

        FloorTile(){

            character = ".";
            name = "Floor";
            color = "Kashmir Blue";
            priority = 0;
            can_place_twice = false;
        }

};

class Wall: public BaseCharacter{

    public: 

        Wall(){
            
            character = "W";
            name = "Wall";
            color = "Light Gray";
            priority = 1;
            traversable = false;
            can_place_twice = false;
        }
            

};


class EmptyVoid: public BaseCharacter{

    public:

        EmptyVoid(){

            character = "*";
            name = "Void";
            color = "Light Gray";
            priority = 1;
            traversable = false;
            can_place_twice = false;
        }
};