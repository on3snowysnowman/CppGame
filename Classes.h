#include <string>

using namespace std;


class BaseCharacter{

    public:

        char character = '0'; //Character that is rendered
        string name = "Default"; //What this is called
        int priority = 20; //The priority of where this will be placed in comparison to other BaseCharacteres in a vector, 
                      //the lower the number, the higher the priority.

        int xPos = 0; // X position in the tilemap
        int yPos = 0; // Y position in the tllemap

        BaseCharacter(){         

        }

        string get_name(){
            return name;
        }
};


class Entity: public BaseCharacter{

    public:

        int hitpoints = 0; // Hitpoints this entity has
        int maxHitpoints = 0; // Maximum hitpoints of this entity
        int damage = 0; // How much damage this entity does

        Entity(){
            
        }

        void set_position(int x, int y){
                xPos = x;
                yPos = y;
        }

        int get_hitpoints(){
            return hitpoints;
        }

        int get_maxHitpoints(){
            return maxHitpoints;
        }

        int get_damage(){
            return damage;
        }

};


class Enemy: public Entity{

    public:

        Enemy(){
            
            name = "Enemy";
            character = 'E';
            priority = 10;

        }

};


class Tile: public BaseCharacter{

    public:

        bool traversable = true; //Whether entities can walk over this object
        bool interactable = false; //Whether this object should trigger its interact function

        Tile(){}

        void interact(){}


};


class FloorTile: public Tile{

    public:

        FloorTile(){
            
            character = '.';
            name = "Floor";
            priority = 20;

        }   

};


class Wall: public Tile{

    public:

        Wall(){
            character = 'W';
            name = "Wall";
            priority = 20;
            traversable = false;
        }

};


class Item : public BaseCharacter{

    public:

        Item(){

            name = "item"; 
            priority = 15;

        }

};