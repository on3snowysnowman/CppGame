#ifndef RANDOM
#define RANDOM

#include <stdlib.h>   
#include <time.h>   
#include <iostream>

using namespace std;

void seed_random(){

    srand(time(NULL));
}

int get_random_num(int min, int max){
    
    //Returns a random number between the range of min, and max - 1

    return rand() % max + min;
}

#endif