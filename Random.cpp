#include <stdlib.h>   
#include <time.h>   
#include <iostream>

using namespace std;

void seed_random(){

    srand(time(NULL));
}

int get_random_num(int min, int max){
    
    return rand() % max + min;
}
