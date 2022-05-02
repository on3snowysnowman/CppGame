#include "HelpfulFunctions.cpp"

#include<iostream>
#include <thread>

using namespace std;

bool run = true;

void print_loop(){
    while(run){
        cout << "Running";
        delay(1);
    }
}
 
int main()
{
    thread input_loop_thread(print_loop);
    cin;
    input_loop_thread.join();
}