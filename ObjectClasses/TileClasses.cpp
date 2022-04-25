#include "BaseClasses.cpp"

class FloorTile: public BaseCharacter{

    public:

        FloorTile(){

            character = ".";
            name = "Floor";
            color = "Kashmir Blue";
            priority = 0;
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
        }
            

};

