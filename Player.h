#include <string>
#include <Classes.h>

using namespace std;

class Player: Entity{

    public:

        string name;
        int health;
        int health_max;

        Player(string targ_name, int targ_health, int targ_health_max){

            name = targ_name;
            health = targ_health;
            health_max = targ_health_max;
            character = 'P';
            priority = 0;
        }

        void change_health(int amount){
            
            if(health + amount < 0){
                health = 0;
                return;
            }

            else if(health + amount > health_max){
                health = health_max;
                return;
            }

            health += amount;
            return;

        }

};
