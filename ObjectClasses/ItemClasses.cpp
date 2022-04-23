#include "BaseClasses.cpp"

class Weapon: public BaseCharacter{

    public:

        Weapon(){

            character  = "W";
            name = "Weapon";
            color = "Orange";
            priority = 1;
        }

};

