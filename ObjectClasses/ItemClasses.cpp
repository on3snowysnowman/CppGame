#include "BaseClasses.cpp"

class Weapon: public BaseCharacter{

    public:

        Weapon(){

            character  = "W";
            type_obj = "Weapon";
            color = "Orange";
            priority = 1;
        }

};

