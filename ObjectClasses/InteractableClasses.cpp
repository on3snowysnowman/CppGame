#ifndef INTERACTABLECLASSES
#define INTERACTABLECLASSES

#include "BaseClasses.cpp"

class WoodDoor: public BaseCharacter{

    public:

        WoodDoor(){
            
            character = "D";
            name = "Wood Door";
            type_obj = "Interactable";
            color = "Brown";
            priority = 2;

        }
};

#endif