#ifndef BASECLASSES
#define BASECLASSES

#include <string>

using namespace std;

class BaseCharacter{

    public:

        string character; //Character that will be rendered
        string color; // Color of this Character
        string name; //Name of this Character
        int priority; //The priority of where this will be placed in comparison to other BaseCharacteres in a vector, 
                    //the higher the number, the higher the priority.
        bool traversable {true}; // Determines whether other BaseCharacter objects can be placed on the same 'tile' as this object

        int xPos = 0; // X position in the tilemap
        int yPos = 0; // Y position in the tllemap

        BaseCharacter(){}

        BaseCharacter(string targ_name, string targ_character = "0", int targ_priority = 0, string targ_color = "White"){

            name = targ_name;
            character = targ_character;
            priority = targ_priority;
            color = targ_color;
        }

        string get_name(){
            return name;
        }

        int get_x(){
            return xPos;
        }

        int get_y(){
            return yPos;
        }

        int get_priority(){
            return priority;
        }

        void set_position(int x, int y){
            
            xPos = x;
            yPos = y;
        }
};


class Entity: public BaseCharacter{

    /*
    Any character that can be classified as 'alive', or sentient and movable
    */

    public:

        int hitpoints; // Hitpoints of this entity
        int max_hitpoints; //Maximum hitpoints of this entity
        int damage {0}; // How much damage this entity deals
        bool movement_random {true}; // Determines whether this entity's movement is random, or it is tracking something

        Entity(){}

        int get_hitpoints(){
            return hitpoints;
        }

        int get_max_hitpoints(){
            return max_hitpoints;
        }

        int get_damage(){
            return damage;
        }

        void set_traversable(bool is_traversable){
            
            traversable = is_traversable;
        }
};


#endif