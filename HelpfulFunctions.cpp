#ifndef HELPFULFUNCTIONS
#define HELPFULFUNCTIONS

#include <unistd.h>

int convert_to_thousand(int num){

return int((num / 255.0) * 1000);

}

void delay(float num){

    usleep(num * 1000000);
}


#endif

