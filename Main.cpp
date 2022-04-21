#include "ColorFunctions.h"
#include "Player.h"
#include <iostream>


void settings(Renderer &renderer, DisplayTool &display_tool){

    pair<int, int> last_index {0, 0};
    Selection selections(vector<string>{"Modify Terminal Colors", "Modify Colors", "Input Menu", "Close Menu"});

    while(true){

        clear();
        renderer.clear_content();
        renderer.add_content("Settings");
        renderer.add_new_line();

        last_index = display_tool.dynamic_selection(selections, last_index.first, "Select One of the Following");

        if(last_index.second == 1){

            string choice = selections.content.at(last_index.first);

            if(choice == "Modify Colors"){
                color_menu(renderer, display_tool);
            }

            else if(choice == "Modify Terminal Colors"){

                terminal_edit_menu(renderer, display_tool);

            }

            else if(choice == "Input Menu"){

                vector<vector<string>> list_elements {vector<string>{"Element 1", "", "str", "-1", "true"}, vector<string>{"Element 2", "", "int", "-1", "true"},
                                                    vector<string>{"(Add New Attribute)", "", "str", "-1", "true"}};

                vector<variant<Variable, SimpleList, List, Choice, Selection>> input_data{Variable("Name", "str", false),
                                                                                        SimpleList("Simple List", 
                                                                                                    vector<string>{"Note 1",
                                                                                                                   "Note 2"}),
                                                                                        List("Elements", list_elements, 2),
                                                                                        Choice("Choices", vector<string>{"Choice 1", "Choice 2"}),
                                                                                        Variable("(Complete Entry)", "", true),
                                                                                    };
                pair<int, int> last_index(0, -1);

                while(true){

                    clear();
                    renderer.clear_content();
                    last_index = display_tool.dynamic_input(input_data, last_index, vector<int> {4}, "Press Enter to Edit");

                    //User selected the escape key
                    if(last_index.first == -1){
                        break;
                    } 
                }
            }
            
            else if(choice == "Close Menu"){
                return;
            }
        }
    }
}

int main()
{
    
    Renderer renderer;
    DisplayTool display_tool(&renderer);

    Player player("Joel", 10, 10);

    renderer.set_selection_color("Blue");

    Selection selections(vector<string>{"Start Game", "Increment Health", "Decrement Health", "Settings", "Exit"});
    pair<int, int> last_index {0, 0};

    while(true){

        clear();
        renderer.clear_content();
        renderer.add_content("Main Menu\n\n");

        display_tool.display_meter(player.health, player.health_max, "Health", vector<string>{"Red", "Orange", "Yellow", "Green"});
        renderer.add_new_line(2);

        last_index = display_tool.dynamic_selection(selections, last_index.first);

        if(last_index.second == 1){

            string choice = selections.content.at(last_index.first);

            if(choice == "Start Game"){
                
                Player player("Joel", 10, 10);

            }

            else if(choice == "Increment Health"){

                player.change_health(1);
            }

            else if(choice == "Decrement Health"){

                player.change_health(-1);
            }

            else if(choice == "Settings"){
                
                settings(renderer, display_tool);
            }

            else if(choice == "Exit"){
                
                endwin();
                exit(0);
            }

        }
    
    }

    
    return 0;

}