#include "Renderer.h"
#include "InputData.h"
#include <iostream>
#include <variant>
#include <cmath>
#include <algorithm>

using namespace std;

class DisplayTool{

    public:

        Renderer *renderer;

        DisplayTool(Renderer *targ_renderer){

            renderer = targ_renderer;

        }

        void display_variant(variant<Variable, SimpleList, List, Choice, Selection> *variant_obj, bool hovered, bool selected){

            renderer->add_content(" ", false);

            //Get the index of the variant to know what object we are dealing with
            switch(variant_obj->index()){

                //Variable
                case 0:
                    {
                        bool is_escape_key {false};

                        if(get<Variable>(*variant_obj).is_escape){
                                is_escape_key = true;
                            }

                        //Item is selected
                        if(selected){

                            renderer->add_content("<", false);
                            renderer->add_content(get<Variable>(*variant_obj).display, false, renderer->selection_color);
                            renderer->add_content(">: ", false);
                            renderer->add_content("[", false, renderer->selection_color);
                            renderer->add_content(get<Variable>(*variant_obj).content, false, renderer->selection_color);
                            renderer->add_content("]", true, renderer->selection_color);
                            return;
                        }

                        //Item is hovered by the cursor
                        else if(hovered){

                            if(is_escape_key){
                                
                                renderer->add_content("<", false);
                                renderer->add_content(get<Variable>(*variant_obj).display, false, renderer->selection_color);
                                renderer->add_content(">");
                                return;
                            }

                            renderer->add_content("<", false);
                            renderer->add_content(get<Variable>(*variant_obj).display, false, renderer->selection_color);
                            renderer->add_content(">: ", false);
                            renderer->add_content(get<Variable>(*variant_obj).content, true);
                            return;
                        }

                        if(is_escape_key){
                                
                            renderer->add_content(get<Variable>(*variant_obj).display);
                            return;
                        }

                        renderer->add_content(get<Variable>(*variant_obj).display, false);
                        renderer->add_content(": ", false);
                        renderer->add_content(get<Variable>(*variant_obj).content);
                        return;
                    }

                //SimpleList
                case 1:

                    //Item is selected
                    if(selected){

                        renderer->add_content(get<SimpleList>(*variant_obj).display, false);
                        renderer->add_content(":");
                        
                        //Simple List index of where our cursor or selected index element exists, saves us the time of running an if statement for each index
                        int cursor_or_selected_index {get<SimpleList>(*variant_obj).inside_cursor_index};
                        
                        //Loop through the elements before the cursor or selected index 
                        for(int i = 0; i < cursor_or_selected_index; i++){
                            
                            renderer->add_content("   ", false);
                            renderer->add_content(get<SimpleList>(*variant_obj).content.at(i));
                        }

                        //The element at the cursor's position is selected
                        if(get<SimpleList>(*variant_obj).inside_selected_index >= 0){
                            renderer->add_content("    [", false, renderer->selection_color);
                            renderer->add_content(get<SimpleList>(*variant_obj).content.at(get<SimpleList>(*variant_obj).inside_selected_index),
                                                  false, renderer->selection_color);
                            renderer->add_content("]", true, renderer->selection_color);
                        }

                        else{

                            //The element is not selected and is just being hovered over
                            renderer->add_content("   <", false);
                                renderer->add_content(get<SimpleList>(*variant_obj).content.at(get<SimpleList>(*variant_obj).inside_cursor_index),
                                                    false, renderer->selection_color);
                                renderer->add_content(">", true);

                        }

                        //Loop through the rest of the elements 
                        for(int i = cursor_or_selected_index + 1; i < get<SimpleList>(*variant_obj).content.size(); i++){
                            renderer->add_content("   ", false);
                            renderer->add_content(get<SimpleList>(*variant_obj).content.at(i));
                        }

                        return;
                    }

                    //Item is hovered by the cursor
                    else if(hovered){
                        renderer->add_content("<", false);
                        renderer->add_content(get<SimpleList>(*variant_obj).display, false, renderer->selection_color);
                        renderer->add_content(">:");

                        //Loop through each of the elements
                        for(int i = 0; i < get<SimpleList>(*variant_obj).content.size(); i++){
                            renderer->add_content("    ", false);
                            renderer->add_content(get<SimpleList>(*variant_obj).content.at(i));
                        }

                        return;
                    }

                    renderer->add_content(get<SimpleList>(*variant_obj).display, false);
                    renderer->add_content(":");

                    //Loop through each of the elements
                    for(int i = 0; i < get<SimpleList>(*variant_obj).content.size(); i++){
                        renderer->add_content("   ", false);
                        renderer->add_content(get<SimpleList>(*variant_obj).content.at(i));
                    }
                    return;

                //List
                case 2:

                    //Item is selected
                    if(selected){

                        renderer->add_content(get<List>(*variant_obj).display, false);
                        renderer->add_content(":");
                        
                        //Simple List index of where our cursor or selected index element exists, saves us the time of running an if statement for each index
                        int cursor_or_selected_index {get<List>(*variant_obj).inside_cursor_index};
                        
                        //Loop through the elements before the cursor or selected index 
                        for(int i = 0; i < cursor_or_selected_index; i++){
                            
                            renderer->add_content("   ", false);
                            renderer->add_content(get<List>(*variant_obj).content.at(i).at(0), false);
                            renderer->add_content(": ", false);
                            renderer->add_content(get<List>(*variant_obj).content.at(i).at(1));
                        }
 
                        //The element at the cursor's position is selected
                        if(get<List>(*variant_obj).inside_selected_index >= 0){

                            renderer->add_content("   <", false);
                            renderer->add_content(get<List>(*variant_obj).content.at(get<List>(*variant_obj).inside_selected_index).at(0),
                                                  false, renderer->selection_color);
                            renderer->add_content(">: ", false);
                            renderer->add_content("[", false, renderer->selection_color);
                            renderer->add_content(get<List>(*variant_obj).content.at(get<List>(*variant_obj).inside_selected_index).at(1),
                                                  false, renderer->selection_color);
                            renderer->add_content("]", true, renderer->selection_color);
                        }

                        else{

                            //The element is not selected and is just being hovered over
                            renderer->add_content("   <", false);
                            renderer->add_content(get<List>(*variant_obj).content.at(get<List>(*variant_obj).inside_cursor_index).at(0),
                                                  false, renderer->selection_color);
                            renderer->add_content(">: ", false);
                            renderer->add_content(get<List>(*variant_obj).content.at(get<List>(*variant_obj).inside_cursor_index).at(1),
                                                  true);

                        }

                        //Loop through the rest of the elements 
                        for(int i = cursor_or_selected_index + 1; i < get<List>(*variant_obj).content.size(); i++){
                            renderer->add_content("   ", false);
                            renderer->add_content(get<List>(*variant_obj).content.at(i).at(0), false);
                            renderer->add_content(": ", false);
                            renderer->add_content(get<List>(*variant_obj).content.at(i).at(1));
                        }

                        return;
                    }

                    //Item is hovered by the cursor
                    else if(hovered){
                        
                        renderer->add_content("<", false);
                        renderer->add_content(get<List>(*variant_obj).display, false, renderer->selection_color);
                        renderer->add_content(">:");

                        //Loop through each of the elements
                        for(int i = 0; i < get<List>(*variant_obj).content.size(); i++){
                            renderer->add_content("    ", false);
                            renderer->add_content(get<List>(*variant_obj).content.at(i).at(0), false);
                            renderer->add_content(": ", false);
                            renderer->add_content(get<List>(*variant_obj).content.at(i).at(1));
                        }

                        return;
                    }

                    renderer->add_content(get<List>(*variant_obj).display, false);
                    renderer->add_content(":");

                    //Loop through each of the elements
                    for(int i = 0; i < get<List>(*variant_obj).content.size(); i++){
                        renderer->add_content("   ", false);
                            renderer->add_content(get<List>(*variant_obj).content.at(i).at(0), false);
                            renderer->add_content(": ", false);
                            renderer->add_content(get<List>(*variant_obj).content.at(i).at(1));
                    }

                    return;

                //Choice
                case 3:

                    //Item is selected
                    if(selected){

                        renderer->add_content(get<Choice>(*variant_obj).display, false);
                        renderer->add_content(":");
                        
                        //Loop through the elements before the cursor
                        for(int i = 0; i < get<Choice>(*variant_obj).inside_cursor_index; i++){
                            
                            renderer->add_content("   ", false);
                            renderer->add_content(get<Choice>(*variant_obj).content.at(i));
                        }

                        //Display the element highlighted by the cursor
                        renderer->add_content("   <", false);
                            renderer->add_content(get<Choice>(*variant_obj).content.at(get<Choice>(*variant_obj).inside_cursor_index),
                                                false, renderer->selection_color);
                            renderer->add_content(">", true);

                        //Loop through the rest of the elements 
                        for(int i = get<Choice>(*variant_obj).inside_cursor_index + 1; i < get<Choice>(*variant_obj).content.size(); i++){
                            renderer->add_content("   ", false);
                            renderer->add_content(get<Choice>(*variant_obj).content.at(i));
                        }

                        return;
                    }

                    //Item is hovered by the cursor
                    else if(hovered){
                        renderer->add_content("<", false);
                        renderer->add_content(get<Choice>(*variant_obj).display, false, renderer->selection_color);
                        renderer->add_content(">: ", false);
                        renderer->add_content(get<Choice>(*variant_obj).content.at(get<Choice>(*variant_obj).current_choice_index));
                        return;
                    }

                    renderer->add_content(get<Choice>(*variant_obj).display, false);
                    renderer->add_content(": ", false);
                    renderer->add_content(get<Choice>(*variant_obj).content.at(get<Choice>(*variant_obj).current_choice_index));
                    return;
            }

            /*

            else if(choice_type == "Choice"){

                //Item is hovered by the cursor
                if(hovered){
                    renderer->add_content("<", false);
                    renderer->add_content(display, false, renderer->selection_color);
                    renderer->add_content(">");
                }

                renderer->add_content(display);
                return;

            }

            else if(choice_type == "SimpleList"){

            }

            else if(choice_type == "List"){
                
            }

            else if(choice_type == "Selection"){

                if(hovered){
                    renderer->add_content("<", false);
                    renderer->add_content(display, false, renderer->selection_color);
                    renderer->add_content(">");
                    return;
                }

                renderer->add_content(display);

            }

            */

        }

        void display_selection(string display, bool hovered){

            if(hovered){
                renderer->add_content(" <", false);
                renderer->add_content(display, false, renderer->selection_color);
                renderer->add_content(">");
                return;
            }

            renderer->add_content(" ", false);
            renderer->add_content(display);

        }

        string determine_color(int value, int maximum, vector<string> color_list){

            /*
            Takes two integers, and devides the value by the maximum. A color from the passed list is selected based on the ratio of 
            value to maximum
            */

            //If we only have one color, return that 
            if(color_list.size() == 1){
                return color_list.at(0);
            }

            //If the value is greater than or equal to the maximum, we know we should return the last value of the color_list
            if(value >= maximum){
                return color_list.at(color_list.size() - 1);
            }

            //If the value is 0, we know we should return the first value
            else if (value == 0){
                return color_list.at(0);
            }

            //Calculate the ratio between the value and the maximum
            float ratio = value/(maximum * 1.0);

            return color_list.at(ceil(color_list.size() * ratio) - 1);

        }

        void display_meter(int value, int maximum, string display = "",
                           vector<string> order = vector<string>{"Green", "Yellow", "Orange", "Red"}){

            // We don't want to divide by 0
            if(maximum == 0){
                renderer->add_content("0 <> 0");
            }

            string current_color = determine_color(value, maximum, order);

            if(display.length() > 0){
                renderer->add_content(display);
            }
            renderer->add_content(to_string(value), false);

            renderer->add_content(" <", false);

            for(int i = 0; i < value; i++){
                
                renderer->add_content("|", false, current_color);
            }

            for(int i = 0; i < maximum - value; i++){

                renderer->add_content(".", false);
            }

            
            renderer->add_content("> ", false);
            renderer->add_content(to_string(maximum));

        }

        void display_number(int value, int maximum, string display = "", vector<string> order = 
                            vector<string>{"Green", "Yellow", "Orange", "Red"}){
            
            string current_color = determine_color(value, maximum, order);

            if(display.length() > 0){
                renderer->add_content(display, false);
            }
            renderer->add_content(": ", false);
            renderer->add_content(to_string(value), true, current_color);
        }

        pair<int, int> dynamic_selection(Selection *sel_obj, int index, string display = ""){

            if (display.length() > 0){
                renderer->add_content(display, false);
                renderer->add_content(":");
                renderer->add_new_line();
            }

            int count {0};
            bool hovered {false};

            //Loop through every element in our choices
            for(string element : sel_obj->content){
                
                hovered = false;

                //If we are printing out the index the cursor is currently hovering over
                if(count == index){
                    hovered = true;
                }

                display_selection(element, hovered);
                count += 1;
            }

            renderer->render();
            
            int inp {0};
            inp = getch();

            //Input is w
            if(inp == 119){
               
               //Move the cursor up an element if there is room
                if(index - 1 >= 0){
                    return pair<int, int>(index - 1, 0);
                }

                //We have reached the top of the list, place cursor at the bottom 
                return pair<int, int>(sel_obj->content.size() - 1, 0);

                
            }

            //Input is s
            else if(inp == 115){

                //Move the cursor down an element if there is room
                if(index + 1 < sel_obj->content.size()){
                    return pair<int, int>(index + 1, 0);
                }

                //We have reached the bottom of the list, place cursor at the top
                return pair<int, int>(0, 0);

            }

            //Input is q
            else if(inp == 113){

                //Place cursor at the top
                return pair<int, int>(0, 0);
            }

            //Input is e
            else if(inp == 101){

                //Place cursor at the bottom
                return pair<int, int>(sel_obj->content.size() - 1, 0);

            }

            //Input is the Enter key
            else if(inp == 13){

                //Select the value that the cursor is hovering over by returning a 1 in the second element of the pair
                return pair<int, int>(index, 1);

            }

            return pair<int, int>(index, 0);

        }

        pair<int, int> dynamic_input(vector<variant<Variable, SimpleList, List, Choice, Selection>> *input_data, 
                                     pair<int, int> last_index, vector<int> list_escape_index = {}, string display = ""){
            
            int selected_index {last_index.second};
            int cursor_index {last_index.first};

            if(display.length() > 0){
                renderer->add_content(display, false);
                renderer->add_content(":");
                renderer->add_new_line();
            }

            int count {0};

            //Loop through each element in our input data
            for(variant<Variable, SimpleList, List, Choice, Selection> element : *input_data){
                
                //This element is selected
                if(count == selected_index){

                    display_variant(&element, false, true);
                    count += 1;
                    continue;
                }

                //This element is hovered by the cursor
                else if(count == cursor_index){

                    display_variant(&element, true, false);
                    count += 1;
                    continue;
                }

                display_variant(&element, false, false);
                count += 1;
            }

            renderer->render();

            int character {0};
            character = getch();

            //User does not have anything selected, input will be directed to moving the cursor
            if(selected_index == -1){
 
                switch(character){

                    //Input is the Enter key
                    case 13:

                        //User has selected the escape key
                        if(find(list_escape_index.begin(), list_escape_index.end(), cursor_index) != list_escape_index.end()){
                            //Return a -1 in the cursor's position, which tells us that an escape key has been pressed, and the cursor index 
                            //in the selected index to let us know which escape key has been pressed by index
                            return pair<int, int>(-1, cursor_index);
                        }

                        //Return the index of where the cursor is in the second index of pair, this tells us something is selected
                        return pair<int, int>(cursor_index, cursor_index);

                    case 115:

                        //User has reached the bottom of the content, send cursor to the top
                        if(cursor_index + 1 == input_data->size()){
                            return pair<int, int>(0, selected_index);
                        }

                        return pair<int, int>(cursor_index + 1, selected_index);
                    
                    //Input is w
                    case 119:

                        //User has reached the top of the content, send cursor to the bottom 
                        if(cursor_index - 1 < 0){
                            return pair<int, int>(input_data->size() - 1, selected_index);
                        }

                        return pair<int, int>(cursor_index - 1, selected_index);

                }
            }

            //User has something selected, input will be directed into whatever object they have selected
            else{

                switch(input_data->at(selected_index).index()){

                    //Variable
                    case 0:

                        return get<Variable>(input_data->at(selected_index)).handle_input(character, cursor_index, selected_index);

                    //SimpleList
                    case 1:

                        return get<SimpleList>(input_data->at(selected_index)).handle_input(character, cursor_index, selected_index);

                    //List
                    case 2:

                        {

                            pair<int, int> final = get<List>(input_data->at(selected_index)).handle_input(character, cursor_index, selected_index);

                            //An escape key was pressed
                            if(final.first == -1){

                                pair<int, int> last_index {0, 0};
                                Selection selections(vector<string>{"Number", "Text", " <- Back"});

                                bool break_loop {false};
                                string type_val{"str"};

                                //Prompt the user to choose whether the type of the input for their new element is of string or int value
                                while(true){

                                    clear();
                                    renderer->clear_content();
                                    renderer->add_content("What is the type of the value for the new attribute?\n");
                                    renderer->render();

                                    last_index = dynamic_selection(&selections, last_index.first);

                                    if (last_index.second == 1){
                                        
                                        string choice = selections.content.at(last_index.first);

                                        if(choice == "Number"){
                                            type_val = "int";
                                        }
                                        
                                        //We don't need to check if the input is text, because that is the default
                                        
                                        else if(choice == " <- Back"){
                                            break_loop = true;
                                        }

                                        break;

                                    }

                                }

                                if(break_loop){
                                    break;
                                }

                                pair<int, int> second_last_index {0, 0};
                                vector<vector<string>> list_elements {vector<string>{"Name", "", "str", "-1"}, vector<string>{"Value", "", type_val, "-1"}};

                                vector<variant<Variable, SimpleList, List, Choice, Selection>> second_input_data{
                                                                                        List("Elements", list_elements),
                                                                                        Variable("(Complete Entry)", "", true),
                                                                                    };

                                while(true){
                                    
                                    clear();
                                    renderer->clear_content();
                                    renderer->add_content("Enter the data for the new attribute\n");

                                    second_last_index = dynamic_input(&second_input_data, second_last_index, vector<int> {1});

                                    if(second_last_index.first == -1){

                                        //If the length of the name entered by the user is greater than 1
                                        if(get<List>(second_input_data.at(0)).content.at(0).at(1).size() > 0){

                                            //Insert the new value right before the add new attribute escape key inside our list
                                            get<List>(input_data->at(selected_index)).add_new_attrib(get<List>(second_input_data.at(0)).content.at(0).at(1),
                                                                                                get<List>(second_input_data.at(0)).content.at(1).at(1),
                                                                                                get<List>(second_input_data.at(0)).content.at(1).at(2));
                                        }

                                        break;
                                    }
                                } 
                                return pair<int, int>(cursor_index, selected_index);
                            }

                            return final;

                        }

                    //Choice
                    case 3:

                        return get<Choice>(input_data->at(selected_index)).handle_input(character, cursor_index, selected_index);

                    //Selection
                    case 4:

                        break;

                }

            }

            return pair<int, int>(cursor_index, selected_index);

        }

};
