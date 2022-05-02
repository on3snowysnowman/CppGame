#ifndef BASECLASSES
#define BASECLASSES

#include <string>
#include <iostream>

using namespace std;

class BaseCharacter{

    public:

        string character; //Character that will be rendered
        string color; // Color of this Character
        string name; // Name of this Character
        string type_obj {"BaseCharacter"}; // What class this object is derived from, ei: "BaseCharacter" or "Entity"
        int priority; //The priority of where this will be placed in comparison to other BaseCharacteres in a vector, 
                    //the higher the number, the higher the priority.
        bool traversable {true}; // Determines whether other BaseCharacter objects can be placed on the same 'tile' as this object
        bool should_move {false}; // If this object should be called to move inside of our input loop 
        bool ignore_non_traversables {false}; // Whether this entity can traverse over non traversable objects
        bool can_place_twice; // Whether two or more of these objects can be placed on the same tile when inside our sandbox constructor

        int xPos = 0; // X position in the tilemap
        int yPos = 0; // Y position in the tllemap

        BaseCharacter(){}

        BaseCharacter(string targ_name, string targ_character = "0", int targ_priority = 0, string targ_color = "White", bool targ_place_twice = true){

            name = targ_name;
            character = targ_character;
            priority = targ_priority;
            color = targ_color;
            can_place_twice = targ_place_twice;
        }

        BaseCharacter(const BaseCharacter &old_obj){
            
            character = old_obj.character;
            color = old_obj.color;
            name = old_obj.name;
            priority = old_obj.priority;
            traversable = old_obj.traversable;
            should_move = old_obj.should_move;
            ignore_non_traversables = old_obj.ignore_non_traversables;
            type_obj = old_obj.type_obj;

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
        int move_chance {3}; // Chance out of 10 if it will move when called move
        bool invulnerable = false;
        
        Entity(){

            type_obj = "Entity";
            should_move = true;
        }

        Entity(const Entity& old_obj){
            
            character = old_obj.character;
            color = old_obj.color;
            name = old_obj.name;
            priority = old_obj.priority;
            traversable = old_obj.traversable;
            should_move = old_obj.should_move;
            ignore_non_traversables = old_obj.ignore_non_traversables;
            type_obj = old_obj.type_obj;
            move_chance = old_obj.move_chance;
            hitpoints = old_obj.hitpoints;
            max_hitpoints = old_obj.max_hitpoints;
        }

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

        void random_move(){ 
        }

};


#endif