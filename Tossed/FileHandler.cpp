#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <algorithm>

using namespace std;

string remove_characters(string string_to_strip, vector<char> black_list){

    string final_string {""};

    for(char element : string_to_strip){
        if(find(black_list.begin(), black_list.end(), element) == black_list.end()){
            final_string += element;
        }
    }

    return final_string;
}

string remove_character(string string_to_strip, char black_list){

    string final_string {""};

    for(char element : string_to_strip){
        if(element != black_list){
            final_string += element;
        }
    }

    return final_string;
}

class FileHandler{

    public:

        fstream myFile; //File loader
        string active_file_path {""}; //Path to our active txt file that we are accessing
        map<string, string> file_contents; //File contents that will be filled on file load

        FileHandler(){}

        void set_path(string path){
            active_file_path = path;
        }

        void load_file(){

            file_contents.clear();

            myFile.open(active_file_path, ios::in);

            if(myFile.is_open()){

                string line;
                while(getline(myFile, line)){
                   
                    line = remove_character(line, ' ');
                   
                    if(line.substr(0, 6) == "String"){
                        _place_string(line);
                    }

                    else if(line.substr(0, 3) == "Int"){
                        _place_int(line);
                    }

                    else if(line.substr(0, 6) == "Vector"){
                        _place_vector(line);
                    }

                    else if(line.substr(0, 3) == "Map"){
                        _place_map(line);
                    }

                }

                myFile.close();
            }

        }

        void save_file(vector<vector<string>> save_contents){

            myFile.open(active_file_path, ios::out);

            if(myFile.is_open()){

                for(vector<string> element : save_contents){
                    
                    myFile << element.at(0) + "{" + element.at(1) + ":" + element.at(2) + "}\n";
                    myFile << "\n";
                }

                myFile.close();
            }

        }

        string get_string(string name){
            if(file_contents.count(name) > 0){
                
                return file_contents[name];
            }
            return "";
        }

        int get_int(string name){
            if(file_contents.count(name) > 0){
                return stoi(file_contents[name]);
            }
            return 0;
        }

        vector<string> get_vector(string name){

            if(file_contents.count(name) > 0){
                string target_data = file_contents[name];

                string type_vector{""};
                int i {1};
                while(target_data.substr(i, 1) != ">"){
                    type_vector += target_data.substr(i, 1);
                    i += 1;
                }

                //Trim the data type off the string
                target_data = target_data.substr(i + 2, target_data.size() - i);
                i = 0;

                if(type_vector == "String"){

                    vector<string> final_vector;

                    string new_element {""};

                    while(i < target_data.length()){

                        if(target_data.substr(i, 1) == ","){
                            final_vector.push_back(new_element);
                            new_element = "";
                            i += 1;
                            continue;
                        }

                        new_element += target_data.substr(i, 1);
                        i += 1;
                    }

                    //Add the final element to the vector
                    final_vector.push_back(new_element);

                    return final_vector;

                }
    
                /*
                else if(type_vector == "Map"){

                    i += 2; //Increment i by 2 to pass '{[' in the string

                    vector<map<string, string>> final_vector;

                    map<string, string> map_to_add;

                    string key_name {""};
                    string value_name {""};

                    while(true){

                        while(i != target_data.size() && target_data.substr(i, 1) != "}"){

                            if(target_data.substr(i, 1) == ":"){
                                
                                i += 1;

                                while(target_data.substr(i, 1) != "]"){
                                    value_name += target_data.substr(i, 1);
                                    i += 1;
                                }

                                map_to_add[key_name] = value_name;
                                key_name = "";
                                value_name = "";

                            }

                            key_name += target_data.substr(i, 1);
                            i += 1;
                        }

                        final_vector.push_back(map_to_add);

                        i += 1;

                        if(i >= target_data.length()){
                            break;
                        }

                        i += 3;

                    }
                }
                */

            }

            return vector<string>{};
        } 

        map<string, string> get_map(string name){

            if(file_contents.count(name) > 0){

                string target_data = file_contents[name];
                map<string, string> final_map;

                int i {1};

                string new_key {""};
                string new_value {""};

                string *target = &new_key;

                while(i < target_data.length()){
                    
                    if(target_data.substr(i, 1) == "]"){
                        final_map[new_key] = new_value;
                        new_key = "";
                        new_value = "";
                        target = &new_key;
                        i += 3;
                        continue;
                    }

                    else if(target_data.substr(i, 1) == ":"){
                        
                        target = &new_value;
                        i += 1;
                        continue;
                    }

                    *target += target_data.substr(i, 1);
                    i += 1;
                }

                return final_map;

            }
        
            return map<string, string>{};
        }



    private:

        void _place_string(string line){

            int i {7}; //Start at 7 to skip past 'String{'
            string targ_string_name {""};

            while(line.substr(i, 1) != ":" && i != line.length()){
                
                targ_string_name += line.substr(i, 1);
                i += 1;
            }

            string targ_string_data {""};

            //Increment i by 1 to pass the ":" inside the string line
            i += 1;

            while(line.substr(i, 1) != "}" && i != line.length()){
                targ_string_data += line.substr(i, 1);
                i += 1;
            }

            file_contents[targ_string_name] = targ_string_data;
        }

        void _place_int(string line){

            int i {4}; 
            string targ_int_name {""};

            while(line.substr(i, 1) != ":" && i != line.length()){
                
                targ_int_name += line.substr(i, 1);
                i += 1;
            }

            string targ_int_data {""};

            //Increment i by 1 to pass the ":" inside the string line
            i += 1;

            while(line.substr(i, 1) != "}" && i != line.length()){
                targ_int_data += line.substr(i, 1);
                i += 1;
            }

            file_contents[targ_int_name] = targ_int_data;
        }

        void _place_vector(string line){

            //Determine which vector we are dealing with 
            int i {7};
            string type_vector {""};

            while(line.substr(i, 1) != ">" && i < line.length()){
                type_vector += line.substr(i, 1);
                i += 1;
            }

            i += 2; //Increment i by 2 to pass the ">{" inside the string line

            if(type_vector == "String"){

                string final_vector{};

                string targ_vector_name {""};

                while(line.substr(i, 1) != ":" && i < line.length()){
                    
                    targ_vector_name += line.substr(i, 1);
                    i += 1;
                }

                string targ_vector_element {""};

                //Increment i by 1 to pass the ":" inside the string line
                i += 1;

                while(line.substr(i, 1) != "}" && i < line.length()){

                    if(line.substr(i, 1) == ","){
                        final_vector += (targ_vector_element + ",");
                        targ_vector_element = "";
                        i += 1;
                        continue;
                    }

                    targ_vector_element += line.substr(i, 1);
                    i += 1;
                }

                //Add the final element to the final vector after the while loop breaks
                final_vector += (targ_vector_element);
                file_contents[targ_vector_name] = "<" + type_vector + ">:" + final_vector;
            }       

            else if(type_vector == "Map"){

                string targ_vector_name {""};

                while(line.substr(i, 1) != ":" && i < line.length()){
                    
                    targ_vector_name += line.substr(i, 1);
                    i += 1;
                }

                string targ_vector_data {""};

                i += 1; // Increment i by 1 to pass the ':' in the string line

                while(i < line.length() && line.substr(i, 1) != "*"){
                    targ_vector_data += line.substr(i, 1);
                    i += 1;
                }

                file_contents[targ_vector_name] = "<" + type_vector + ">:" + targ_vector_data;

            }

        }

        void _place_map(string line){

            string final_map{""};

            int i {4};
            string targ_map_name {""};

            while(i + 1 < line.length() && line.substr(i, 1) != ":"){
                
                targ_map_name += line.substr(i, 1);
                i += 1;
            }

            string targ_map_element_key {""};
            string targ_map_element_value {""};

            //Increment i by 2 to pass the ":[" inside the string line
            i += 2;

            while(i + 1 < line.length() && line.substr(i, 1) != "}"){

                if(line.substr(i, 1) == ":"){
                    
                    //Increment i by 1 to pass the ":"
                    i += 1;

                    while(i + 1 < line.length() && line.substr(i, 1) != "]"){

                        targ_map_element_value += line.substr(i, 1);
                        i += 1;
                    }

                    //Increment i by 3 to pass the "],["
                    i += 3;

                    final_map += "[" + targ_map_element_key + ":" + targ_map_element_value + "],";
                    targ_map_element_key = "";
                    targ_map_element_value = "";
                    continue;
                }

                targ_map_element_key += line.substr(i, 1);
                i += 1;

            }          

            //Trim the last ',' off the final_map string
            final_map = final_map.substr(0, final_map.length() - 1);
            file_contents[targ_map_name] = final_map;

        }
};