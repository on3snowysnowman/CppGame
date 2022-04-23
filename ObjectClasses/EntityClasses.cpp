#include "BaseClasses.cpp"

class Enemy : public Entity{

    public:

        Enemy(string targ_name, string targ_character = "0", int targ_priority = 0, int targ_hitpoints = 0,
            int targ_max_hitpoints = 0, string targ_color = "White", bool is_traversable = false){

            name = targ_name;
            character = targ_character;
            priority = targ_priority;
            hitpoints = targ_hitpoints;
            max_hitpoints = targ_max_hitpoints;
            color = targ_color;
            traversable = is_traversable;

        }
};