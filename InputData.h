#include <vector>
#include <string>

using namespace std;


class Variable{

    public: 

        string display {""};
        string data_type {""};
        string content{""};
        bool is_escape {false};

        Variable(string name, string targ_data_type, bool escape_key = false, string targ_data = ""){
            display = name;
            data_type = targ_data_type;
            is_escape = escape_key;
            content = targ_data;
        }

        pair<int, int> handle_input(int character, int cursor_index, int selected_index){

            switch(character){

                //Input is Backspace
                case 8:

                    //There is more than one character in this string
                    if(content.length() > 0){
                        
                        //Shorten the string by one character
                        content = content.substr(0, content.length() - 1);
                    }

                    return pair<int, int>(cursor_index, selected_index);

                //Input is Ctrl + Backspace
                case 127:

                    content = "";
                    return pair<int, int>(cursor_index, selected_index);

                //Input is the Enter Key
                case 13:

                    return pair<int, int>(cursor_index, -1);

                //Input is a character
                default:

                    //If input type of the selected element is 'int'
                    if(data_type == "int"){

                        //If the character is not a number
                        if(character < 48 && character > 57){
                            return pair<int, int>(cursor_index, selected_index);
                        }
                    }

                    //Add the character to the string
                    content += char(character);
                
                    return pair<int, int>(cursor_index, selected_index);
            }
        }
};


class SimpleList{

    public:

        string display {""}; //What will be displayed as the title for this SimpleList
        int inside_cursor_index {0}; //Index the cursor is hovering over
        int inside_selected_index {-1}; //Index of the selected item, -1 means no item is selected
        vector<string> content {}; //Data of the SimpleList

        SimpleList(string name, vector<string> simple_list_elements){
            
            display = name;
            content = simple_list_elements;
            content.push_back("(Add New Attribute)");
        }

        pair<int, int> handle_input(int character, int cursor_index, int selected_index){

            //User has an element inside Simple List selected
            if(inside_selected_index >= 0){

                switch(character){

                    //Input is Backspace
                    case 8:

                        //There is more than one character in this string
                        if(content.at(inside_selected_index).length() > 0){
                            
                            //Shorten the string by one character
                            content.at(inside_selected_index) = 
                                content.at(inside_selected_index).substr(0, content.at(inside_selected_index).length() - 1);
                        }

                        return pair<int, int>(cursor_index, selected_index);

                    //Input is the Enter Key
                    case 13:

                        //Check if the length of the selected element is 0, if so remove it
                        if(content.at(inside_selected_index).length() == 0){

                            //Delete the empty element
                            content.erase(content.begin() + inside_selected_index);

                            inside_selected_index = -1;

                            return pair<int, int> (cursor_index, selected_index);

                        }

                        //If the length is not 0, deselect this item
                        inside_selected_index = -1;
                        return pair<int, int> (cursor_index, selected_index);

                    //Input is Ctrl + Backspace
                    case 127:

                        content.at(inside_selected_index) = "";
                        return pair<int, int>(cursor_index, selected_index);

                    default:

                        //Add the character to the string
                        content.at(inside_selected_index) = content.at(inside_selected_index) + char(character);
                        return pair<int, int> (cursor_index, selected_index);

                }
            }
        
            //User is traversing the elements in the Simple List
            switch(character){

                //Input is Backspace
                case 8:

                    //Make sure that the User isn't trying to delete the last element of the list, which is the escape key to create a new attribute
                    if(inside_cursor_index == content.size() - 1){
                        return pair<int, int>(cursor_index, selected_index);
                    }

                    //If it is not the escape key, remove the element from the Simple List
                    content.erase(content.begin() + inside_cursor_index);

                    if(inside_cursor_index - 1 >= 0){
                        inside_cursor_index -= 1;
                    }

                    return pair<int, int> (cursor_index, selected_index);


                //Input is the Enter Key
                case 13:

                    //User is selecting an element at the end of the simple list, it must be the escape key to add a new attribute
                    if(inside_cursor_index == content.size() - 1){
                        content.insert(content.end() - 1, "Text");

                        //Increment the cursor so its position stays on the escape key
                        inside_cursor_index += 1;

                        return pair<int, int>(cursor_index, selected_index);
                    }

                    //User is selecting a Simple List value
                    inside_selected_index = inside_cursor_index;
                    return pair<int, int>(cursor_index, selected_index);

                //Input is f
                case 102:

                    //Reset cursor to the top
                    inside_cursor_index = 0;
                    
                    //Return a -1 in place of the selected_index which means we will no longer have this Simple List item selected
                    return pair<int, int>(cursor_index, -1);

                //Input is w
                case 119:

                    //User has reached the top of the Simple List elements, send the cursor to the bottom
                    if(inside_cursor_index - 1 < 0){
                        inside_cursor_index = content.size() - 1;
                        return pair<int, int>(cursor_index, selected_index);
                    }

                    inside_cursor_index -= 1;
                    return pair<int, int>(cursor_index, selected_index);

                //Input is s
                case 115:

                    //User has reached the bottom of the Simple List elements, send the cursor to the top
                    if(inside_cursor_index + 1 == content.size()){
                        inside_cursor_index = 0;
                        return pair<int, int>(cursor_index, selected_index);
                    }

                    inside_cursor_index += 1;
                    return pair<int, int>(cursor_index, selected_index);
            }
        
            return pair<int, int> (cursor_index, selected_index);
        }
};


class List{

    public: 

        string display {}; //What will be displayed as the title for this List
        int inside_cursor_index {0}; //Index the cursor is hovering over
        int inside_selected_index {-1}; //Index of the selected item, -1 means no item is selected
        int exit_key_index {-1};
        vector<vector<string>> content {}; //Data of the List
        //Each element should look like this : vector<string>{"Element 1", "", "str", "-1", "true"}
        // 0 Index : Name
        // 1 Index : Data
        // 2 Index : Type Data
        // 3 Index : Character length limit of data
        // 4 Index : Bool whether this item can be deleted from the list

        List(string name, vector<vector<string>> list_elements, int exit_index = -1){

            display = name; 
            content = list_elements;
            exit_key_index = exit_index;
        }

        void add_new_attrib(string name, string data, string type){

            content.insert(content.begin() + content.size() - 1, vector<string>{name, data, type});
            
            //Increment both our cursor and the exit key, since they've been pushed back one due to adding an element
            inside_cursor_index += 1;
            exit_key_index += 1;
        }

        pair<int, int> handle_input(int character, int cursor_index, int selected_index){

            //User has an element inside the List selected
            if(inside_selected_index >= 0){

                //Backspace
                if(character == 8){

                    //There is more than one character in this string
                    if(content.at(inside_selected_index).at(1).length() > 0){
                        
                        //Shorten the string by one character
                        content.at(inside_selected_index).at(1) = 
                            content.at(inside_selected_index).at(1).substr(0, content.at(inside_selected_index).at(1).length() - 1);
                    }

                    return pair<int, int>(cursor_index, selected_index);
                }

                //Enter Key
                else if(character == 13){
                    inside_selected_index = -1;
                    return pair<int, int>(cursor_index, selected_index);
                }

                //Ctrl + Backspace
                else if(character == 127){
                    content.at(inside_selected_index).at(1) = "";
                    return pair<int, int>(cursor_index, selected_index);
                }

                //If input type of the selected element is 'int'
                if(content.at(inside_selected_index).at(2) == "int"){

                    /*
                    if(character == 115){

                        int data_int = stoi(content.at(inside_selected_index).at(1)); 

                        //If the value of the selected element's is atleast 1, decrement it
                        if(data_int - 1 >= 0){
                            content.at(inside_selected_index).at(1) = to_string(data_int - 1);
                        }
                        
                        return pair<int, int>(cursor_index, selected_index);
                    }
                    */

                    //If the character is not a number
                    if(character < 48 || character > 57){
                        return pair<int, int>(cursor_index, selected_index);
                    }
                }

                int len_limit = stoi(content.at(inside_selected_index).at(3));

                //If the length limit of this element is not -1, then it has a specified length limit
                if(len_limit >= 0){
                    
                    //Adding another character to this string will exceed its length limit
                    if(content.at(inside_selected_index).at(1).size() + 1 > len_limit){
                        return pair<int, int>(cursor_index, selected_index);
                    }
                }

                //Add the character to the string
                content.at(inside_selected_index).at(1) = content.at(inside_selected_index).at(1) + char(character);
                return pair<int, int>(cursor_index, selected_index);
            }

            //User is traversing the elements in the List
            switch(character){

                //Input is Backspace
                case 8:

                    //Make sure that the User isn't trying to delete the exit key
                    if(inside_cursor_index == exit_key_index){
                        return pair<int, int>(cursor_index, selected_index);
                    }

                    //Also check that this element in the list is able to be deleted
                    if(content.at(inside_cursor_index).at(4) == "false"){
                        return pair<int, int>(cursor_index, selected_index);
                    }

                    //If it is not the escape key, remove the element from the Simple List
                    content.erase(content.begin() + inside_cursor_index);

                    if(inside_cursor_index - 1 >= 0){
                        inside_cursor_index -= 1;
                    }

                    exit_key_index -= 1;
                    return pair<int, int>(cursor_index, selected_index);


                //Input is the Enter Key
                case 13:

                    //User is selecting the index of the exit_key
                    if(inside_cursor_index == exit_key_index){
                        
                        //Return -1 instead of the cursor's index spot so we know that an escape key was pressed to create a new element
                        return pair<int, int>(-1, -1);
                    }

                    //User is selecting a Simple List value
                    inside_selected_index = inside_cursor_index;
                    return pair<int, int>(cursor_index, selected_index);

                //Input is f
                case 102:

                    //Reset cursor to the top
                    inside_cursor_index = 0;
                    
                    //Return a -1 in place of the selected_index which means we will no longer have this Simple List item selected
                    return pair<int, int>(cursor_index, -1);

                //Input is w
                case 119:

                    //User has reached the top of the Simple List elements, send the cursor to the bottom
                    if(inside_cursor_index - 1 < 0){
                        inside_cursor_index = content.size() - 1;
                        return pair<int, int>(cursor_index, selected_index);
                    }

                    inside_cursor_index -= 1;
                    return pair<int, int>(cursor_index, selected_index);

                //Input is s
                case 115:

                    //User has reached the bottom of the Simple List elements, send the cursor to the top
                    if(inside_cursor_index + 1 == content.size()){
                        inside_cursor_index = 0;
                        return pair<int, int>(cursor_index, selected_index);
                    }

                    inside_cursor_index += 1;
                    return pair<int, int>(cursor_index, selected_index);
            }
        

            return pair<int, int>(cursor_index, selected_index);
        }
};


class Choice{

    public:

        string display {""}; //What will be displayed as the title for this List
        int inside_cursor_index {0}; //Index the cursor is hovering over
        int current_choice_index {0}; // Index of which choice is selected
        vector<string> content {}; //Choices for the user

        Choice(string name, vector<string> choices){
            
            content = choices;
            display = name;
            
            if(content.size() > 0){
                
                current_choice_index = 0;
            }
        }

        pair<int, int> handle_input(int character, int cursor_index, int selected_index){

            switch(character){

                //Input is the Enter Key
                case 13:

                    current_choice_index = inside_cursor_index;
                    return pair<int, int>(cursor_index, -1);
                
                //Input is f
                case 102:

                    //Reset cursor to the top
                    inside_cursor_index = 0;
                    
                    //Return a -1 in place of the selected_index which means we will no longer have this Simple List item selected
                    return pair<int, int>(cursor_index, -1);

                //Input is w
                case 119:

                    //User has reached the top of the Simple List elements, send the cursor to the bottom
                    if(inside_cursor_index - 1 < 0){
                        inside_cursor_index = content.size() - 1;
                        return pair<int, int>(cursor_index, selected_index);
                    }

                    inside_cursor_index -= 1;
                    return pair<int, int>(cursor_index, selected_index);

                //Input is s
                case 115:

                    //User has reached the bottom of the Simple List elements, send the cursor to the top
                    if(inside_cursor_index + 1 == content.size()){
                        inside_cursor_index = 0;
                        return pair<int, int>(cursor_index, selected_index);
                    }

                    inside_cursor_index += 1;
                    return pair<int, int>(cursor_index, selected_index);
            }

            return pair<int, int>(cursor_index, selected_index);
        }
};


class Selection{

    public:

        string choice_type{"SimpeList"};
        vector<string> content {};

        Selection(vector<string> selections){

            choice_type = "Selection";
            content = selections;
        }

};