#include "FileHandler.h"

using namespace std;

int main(){

    FileHandler file_handler;
    vector<vector<string>> save_contents {
                                    {vector<string>{"String", "MyName", "Joel"}},
                                    {vector<string>{"Int", "Age", "18"}}
                                    };

    file_handler.set_path("Saves/testSave.txt");
    file_handler.save_file(save_contents);
    file_handler.load_file();
    cout << file_handler.get_int("Age") << "\n";
    cout << file_handler.get_string("MyName");

    return 0;
}