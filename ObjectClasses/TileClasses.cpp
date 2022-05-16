#include "BaseClasses.cpp"
#include "..\Random.cpp"

#include <vector>
#include <iostream>

class Liquid: public BaseCharacter{

    public:

        int liquid_level = 0;

        Liquid(int targ_liquid_level){

            type_obj = "Liquid";
            liquid_level = targ_liquid_level;
            character = to_string(targ_liquid_level);
        }

        Liquid(Liquid& old_obj){
            
            character = old_obj.character;
            color = old_obj.color;
            name = old_obj.name;
            priority = old_obj.priority;
            traversable = old_obj.traversable;
            should_move = old_obj.should_move;
            ignore_non_traversables = old_obj.ignore_non_traversables;
            type_obj = old_obj.type_obj;
            liquid_level = old_obj.liquid_level;
        }

        void raise_level(int amount = 1){
            
            if(liquid_level + amount > 5){
                liquid_level = 5;
                return;
            }

            liquid_level += amount;
        }

};


class Dirt: public BaseCharacter{

    public:

        Dirt(){

            character = ".";
            name = "Dirt";
            color = "Brown";
            priority = 0;
            can_place_twice = false;
        }

};


class Grass: public BaseCharacter{

    public:

        Grass(){
            
            //int ran_num = get_random_num(0, 3);
            //vector<string> choices = {",", "\"", "^"};

            character = "\"";
            name = "Grass";
            color = "Green";
            priority = 1;
            can_place_twice = false;
        }
};


class Water: public Liquid{

    public:

        Water(int targ_liquid_level) : Liquid(targ_liquid_level){

            name = "Water";
            color = "Blue";
            priority = 1;
            can_place_twice = false;
        }

};


class Magma: public Liquid{

    public:

        Magma(int targ_liquid_level) : Liquid(targ_liquid_level){

            name = "Magma";
            color = "Orange";
            priority = 1;
            can_place_twice = false;
        }
};


class StoneWall: public BaseCharacter{

    public: 

        StoneWall(){
            
            character = "W";
            name = "Stone Wall";
            color = "Light Gray";
            priority = 2;
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

class Unseeable: public BaseCharacter{

    public: 

        Unseeable(){
            
            character = " ";
            name = "Unseeable";
            color = "White";
            priority = 6;
            traversable = false;
            can_place_twice = false;
        }
};