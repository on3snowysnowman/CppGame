#ifndef PLAYER
#define PLAYER

#include "ObjectClasses/TileClasses.cpp"
#include "ObjectClasses/EntityClasses.cpp"
#include "ObjectClasses/ItemClasses.cpp"
#include "ObjectClasses/InteractableClasses.cpp"
#include "Tilemap.cpp"
#include "Renderer.cpp"
#include "DisplayTool.cpp"

#include <string>
#include <thread>

using namespace std;

void display_colors(Renderer &renderer){

    renderer.add_content("These are your current colors:");
    renderer.add_new_line();

    for(pair<string, int> element : renderer.color_map){
        
        renderer.add_content(element.first, true, element.first);
    }

}

void add_color_menu(Renderer &renderer, DisplayTool &display_tool){

    vector<vector<string>> list_elements {vector<string>{"r", "255", "int", "3", "false"}, vector<string>{"g", "255", "int", "3", "false"}, vector<string>{"b", "255", "int", "3", "false"}};

    vector<variant<Variable, SimpleList, List, Choice, Selection>> input_data{Variable("Color Name", "str"),
                                                                            List("Color RGB Values", list_elements),
                                                                            Variable("(Cancel Entry)", "", true),
                                                                            Variable("(Complete Entry)", "", true)};
                
    pair<int, int> last_index {0, -1};

    while(true){
        
        clear();
        renderer.clear_content();

        if(get<List>(input_data.at(1)).content.at(0).at(1).size() > 0
            && get<List>(input_data.at(1)).content.at(1).at(1).size() > 0
            && get<List>(input_data.at(1)).content.at(2).at(1).size() > 0){

            renderer.add_color("Sample", vector<int>{stoi(get<List>(input_data.at(1)).content.at(0).at(1)),
                                                stoi(get<List>(input_data.at(1)).content.at(1).at(1)),
                                                stoi(get<List>(input_data.at(1)).content.at(2).at(1))}, true);
            renderer.add_content("Sample", true, "Sample");
            renderer.add_new_line();
        }

        else{

            renderer.add_new_line(2);
        }

        last_index = display_tool.dynamic_input(input_data, last_index, vector<int> {2, 3});

        if(last_index.first == -1){

            renderer.color_map.erase("Sample");

            //Pressed Cancel Entry
            if(last_index.second == 2){
                return;
            }

            if(get<Variable>(input_data.at(0)).content.length() > 0){
                
                renderer.add_color(get<Variable>(input_data.at(0)).content, 
                                    vector<int>{stoi(get<List>(input_data.at(1)).content.at(0).at(1)),
                                        stoi(get<List>(input_data.at(1)).content.at(1).at(1)),
                                        stoi(get<List>(input_data.at(1)).content.at(2).at(1))});
            }

            return;
        }

    }

}

void change_color_menu(Renderer &renderer, DisplayTool &display_tool){

    vector<string> choices {};
    
    for(pair<string, int> element : renderer.color_map){
        choices.push_back(element.first);
    }

    choices.push_back(" <- Back");

    Selection selections(choices);
    pair<int, int> last_index {0, 0};

    while(true){

        clear();
        renderer.clear_content();
        renderer.add_content("Settings/Modify Colors/Change Color\n\n");
        display_colors(renderer);
        renderer.add_new_line();

        last_index = display_tool.dynamic_selection(selections, last_index.first, "Choose which color to modify");

        if(last_index.second == 1){

            string choice = selections.content.at(last_index.first);

            if(choice == " <- Back"){
                return;
            }

            vector<int> rgb_vals = renderer.color_rgb_map[choice];

            vector<vector<string>> list_elements {vector<string>{"r", to_string(rgb_vals.at(0)), "int", "3", "false"}, 
                vector<string>{"g", to_string(rgb_vals.at(1)), "int", "3", "false"}, vector<string>{"b", to_string(rgb_vals.at(2)), "int", "3", "false"}};

            vector<variant<Variable, SimpleList, List, Choice, Selection>> input_data{Variable("Color Name", "str", false, choice),
                                                                            List("Color RGB Values", list_elements),
                                                                            Variable("(Cancel Entry)", "", true),
                                                                            Variable("(Complete Entry)", "", true)};

            pair<int, int> second_last_index {0, -1}; 

            while(true){

                clear();
                renderer.clear_content();

                if(get<List>(input_data.at(1)).content.at(0).at(1).size() > 0
                    && get<List>(input_data.at(1)).content.at(1).at(1).size() > 0
                    && get<List>(input_data.at(1)).content.at(2).at(1).size() > 0){

                    renderer.add_color("Sample", vector<int>{stoi(get<List>(input_data.at(1)).content.at(0).at(1)),
                                                        stoi(get<List>(input_data.at(1)).content.at(1).at(1)),
                                                        stoi(get<List>(input_data.at(1)).content.at(2).at(1))}, true);
                    renderer.add_content("Sample", true, "Sample");
                    renderer.add_new_line();
                }

                else{
                    renderer.add_new_line(2);
                }

                second_last_index = display_tool.dynamic_input(input_data, second_last_index, {2, 3}, "Press Enter to Select");

                if(second_last_index.first == -1){

                    renderer.color_map.erase("Sample");

                    if(second_last_index.second == 2){
                        return;
                    }

                    //Make sure the User isn't trying to add a color with no name
                    if(get<Variable>(input_data.at(0)).content.length() > 0){

                        renderer.change_color(choice, get<Variable>(input_data.at(0)).content, vector<int>{
                                                                                                stoi(get<List>(input_data.at(1)).content.at(0).at(1)),
                                                                                                stoi(get<List>(input_data.at(1)).content.at(1).at(1)),
                                                                                                stoi(get<List>(input_data.at(1)).content.at(2).at(1))
                                                                                                });
                    }
                    return;
                }  
            }
        }
    }
}

void terminal_edit_menu(Renderer &renderer, DisplayTool &display_tool){

    pair<int, int> last_index {0, 0};
    Selection selections(vector<string> {"Terminal Color", "Cursor Color", " <- Back"});

    while(true){

        clear();
        renderer.clear_content();
        renderer.add_content("Settings/Modify Terminal Colors\n\n");
        renderer.add_content("Terminal Color", true, renderer.terminal_text_color);
        renderer.add_content("Cursor Color", true, renderer.selection_color);
        renderer.add_new_line();

        last_index = display_tool.dynamic_selection(selections, last_index.first);

        if(last_index.second == 1){

            string choice = selections.content.at(last_index.first);

            if(choice == "Terminal Color" || choice == "Cursor Color"){

                pair<int, int> second_last_index {0, 0};
                vector<string> second_choices {};

                for(pair<string, int> element : renderer.color_map){
                    second_choices.push_back(element.first); 
                }

                second_choices.push_back(" <- Back");

                Selection second_selections(second_choices);

                while(true){
                    
                    clear();
                    renderer.clear_content();

                    for(pair<string, int> element : renderer.color_map){
                        renderer.add_content(element.first, true, element.first);
                    }
                    renderer.add_new_line();

                    renderer.add_content("Select New ", false);
                    renderer.add_content(choice);

                    second_last_index = display_tool.dynamic_selection(second_selections, second_last_index.first, "");

                    if(second_last_index.second == 1){

                        string second_choice = second_selections.content.at(second_last_index.first);

                        if(second_choice == " <- Back"){

                            break;
                        }

                        //User wants to change the terminal text color
                        else if(choice == "Terminal Color"){
                            renderer.terminal_text_color = second_choice;
                            break;
                        }

                        else if(choice == "Cursor Color"){

                            //User wants to change the selection color
                            renderer.selection_color = second_choice;;
                            break;

                        }
                    }

                }

            }

            else if(choice == " <- Back"){
                return;
            }

        }

    }

}

void color_menu(Renderer &renderer, DisplayTool &display_tool){
    
    Selection selections(vector<string>{"Add New Color", "Change Color", " <- Back"});
    pair<int, int> last_index {0, 0};

    while(true){
        
        renderer.clear_content();
        renderer.add_content("Settings/Modify Colors\n\n");
        display_colors(renderer);
        renderer.add_new_line();
        last_index = display_tool.dynamic_selection(selections, last_index.first);

        if(last_index.second == 1){

            string choice = selections.content.at(last_index.first);

            if(choice == "Change Color"){

                change_color_menu(renderer, display_tool);                
            }

            else if(choice == "Add New Color"){

                add_color_menu(renderer, display_tool);
            }

            else if(choice == " <- Back"){
                return;
            }

        }

    }
}

void settings(Renderer &renderer, DisplayTool &display_tool){

    pair<int, int> last_index {0, 0};
    Selection selections(vector<string>{"Modify Terminal Colors", "Modify Colors", "Input Menu", "Close Menu", "Exit Game"});

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
       
            else if(choice == "Exit Game"){

                endwin();
                exit(0);
            }
        }
    }
}


class Player: public Entity{

    public:

        Tilemap *tilemap;
        DisplayTool *display_tool;
        bool accepting_input {false};
        bool run {false};

        Player(string targ_name, int targ_hitpoints, int targ_max_hitpoints, Tilemap &tilemap_instance, DisplayTool &display_tool_instance){

            tilemap = &tilemap_instance; // Tilemap instance
            display_tool = &display_tool_instance; // DisplayTool instance

            name = targ_name;
            character = "P";
            priority = 5;
            hitpoints = targ_hitpoints;
            max_hitpoints = targ_max_hitpoints;
            color = "Blue";
            type_obj = "Player";
            should_move = false;
            traversable = false;
            ignore_non_traversables = false;
            

        }

        void pause_display(){
            run = false;
            accepting_input = false;
        }

        void start_display(){
            run = true;
            accepting_input = true;
        }

        void set_god_mode(bool set_boolean){
            if(set_boolean){
                ignore_non_traversables = true;
                invulnerable = true;
                return;
            }

            ignore_non_traversables = false;
            invulnerable = false;
        }

        void change_health(int amount){
            
            if(hitpoints + amount < 0){
                hitpoints = 0;
                return;
            }

            else if(hitpoints + amount > max_hitpoints){
                hitpoints = max_hitpoints;
                return;
            }

            hitpoints += amount;
            return;

        }

        void handle_input(int character){

            /*
            Input from the user is sent here to be handled
            :param character: Int from a getch() call that will be handled
            */

            // w - Move Up
            if(character == 119){
                
                tilemap->move(*this, xPos, yPos - 1);
            }

            // d - Move Right
            else if(character == 100){

                tilemap->move(*this, xPos + 1, yPos);
            }

            // s - Move Down
            else if(character == 115){

                tilemap->move(*this, xPos, yPos + 1);
            }

            //a - Move Left
            else if(character == 97){

                tilemap->move(*this, xPos - 1, yPos);
            }

            //m - Menu
            else if(character == 109){
                
                pause_display();
                settings(*display_tool->renderer, *display_tool);
                start_display();
            }

        }

        void load_tilemap(Tilemap &tilemap_instance){
            tilemap = &tilemap_instance;
        }
};

#endif