#ifndef RENDERER
#define RENDERER

    #include <curses.h>
    #include <string>
    #include <map>
    #include <vector>

    using namespace std;

    int convert_to_thousand(int num){

    return int((num / 255.0) * 1000);

    }

    pair<pair<map<string, int>, map<string, vector<int>>>, int> create_colors(int count = 1){

    // Map of color names tied to their rgb values
    map<string, vector<int>> color_rgb_vals = {  
                                            {"White", vector<int>{255, 255, 255}},
                                            {"Blue", vector<int>{0, 120, 255}},
                                            {"Kashmir Blue", vector<int>{78, 94, 160}},
                                            {"Light Blue", vector<int>{74, 231, 255}},
                                            {"Red", vector<int>{255, 75, 50}},
                                            {"Green", vector<int>{70, 255, 81}},
                                            {"Teal", vector<int>{74, 255, 190}},
                                            {"Yellow", vector<int>{255, 252, 77}},
                                            {"Purple", vector<int>{185, 92, 247}},
                                            {"Magenta", vector<int>{255, 0, 255}},
                                            {"Orange", vector<int>{255, 150, 70}},
                                        }; 



    map<string, int> color_pair_vals; // Map of color names tied to their color pair numbers

    int r {0};
    int g {0};
    int b {0};

    //Loop through our map of colors to their respective r g b values and create the color pairs for curses
    for(pair<string, vector<int>> element : color_rgb_vals){

        r = convert_to_thousand(element.second.at(0));
        g = convert_to_thousand(element.second.at(1));
        b = convert_to_thousand(element.second.at(2));

        init_color(count, r, g, b);
        init_pair(count, count, 0);
        color_pair_vals[element.first] = count; // Add the new color to the pair vals map, with the value of its color pair

        count += 1;

    }

    return pair<pair<map<string, int>, map<string, vector<int>>>, int>
        (pair<map<string, int>, map<string, vector<int>>>{color_pair_vals, color_rgb_vals}, count);

    }


    class Renderer{

    public:

        map<string, int> color_map; // This gives us our list of color names to their respective pairs
        map<string, vector<int>> color_rgb_map; // This gives us our list of color names to their respective rgb values
        int current_color_pair {1}; // Number of the color pair we are currently on
        vector<pair<string, int>> content; // Content that will be rendered on next render call, stored in a pair resembling the text to be rendered, and the color pair it 
                                                // corresponds to

        string terminal_text_color = "White"; // The color that is displayed for default terminal text
        string selection_color {"Blue"}; // The color that is displayed when the cursor hovers over an element
        vector<int> overwritable_color_pairs {}; // Color pairs that have been deleted and can be re-created

        Renderer(){

            start_curses();
            setup_color();
            
        }

        void start_curses(){

            /*
            Sets up our curses windows and starts our standard curses values
            */
            
            initscr();			/* Start curses mode 		*/
            start_color();
            curs_set(0); //Hide cursor
            raw();
            scrollok(stdscr, true);
            idlok(stdscr, true);
            keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
            noecho();			/* Don't echo() while we do getch */
        
        }

        void setup_color(){
            
            
            pair<pair<map<string, int>, map<string, vector<int>>>, int> color_info = create_colors();

            color_map = color_info.first.first;
            color_rgb_map = color_info.first.second;
            current_color_pair = color_info.second;

        }

        void add_color(string color_name, vector<int> rgb_values, bool temp_color = false){
            
            for(int i = 0; i < rgb_values.size(); i++){
                if(rgb_values.at(i) > 255){
                    rgb_values.at(i) = 255;
                }
            }

            int r = convert_to_thousand(rgb_values.at(0));
            int g = convert_to_thousand(rgb_values.at(1));
            int b = convert_to_thousand(rgb_values.at(2));
            
            init_color(current_color_pair, r, g, b);
            init_pair(current_color_pair, current_color_pair, 0);

            color_map[color_name] = current_color_pair;
            color_rgb_map[color_name] = rgb_values;

            if(not temp_color){
                current_color_pair += 1;
            }
        }

        void change_color(string color_to_change_name, string new_color_name, vector<int> rgb_values){

            int changed_color_pair = color_map[color_to_change_name];

            int r = convert_to_thousand(rgb_values.at(0));
            int g = convert_to_thousand(rgb_values.at(1));
            int b = convert_to_thousand(rgb_values.at(2));

            init_color(changed_color_pair, r, g, b);
            init_pair(changed_color_pair, changed_color_pair, 0);

            color_map.erase(color_to_change_name);
            color_map[new_color_name] = changed_color_pair;

            color_rgb_map.erase(color_to_change_name);
            color_rgb_map[new_color_name] = rgb_values;

            if(terminal_text_color == color_to_change_name){
                terminal_text_color = new_color_name;
            }
            
            if(selection_color == color_to_change_name){
                selection_color = new_color_name;
            }

        }

        void set_selection_color(string color){

            /*
            Sets our cursors select color to the passed color
            */

            selection_color = color;

        }

        void set_terminal_text_color(string color){
            terminal_text_color = color;
        }

        void add_content(string new_content, bool new_line = true, string color = ""){

            if(color.length() == 0){
                color = terminal_text_color;
            }

            content.push_back(pair<string, int>(new_content, color_map[color]));
            if(new_line){
                content.push_back(pair<string, int>("\n", 0));
            }

        }

        void add_new_line(int num = 1){
            
            for(int i = 0; i < num; i++){
                content.push_back(pair<string, int>("\n", 0));
            }

        }

        void clear_content(){
            content.clear();
        }

        void render(){

            clear();

            for(pair<string, int> element : content){

                attron(COLOR_PAIR(element.second));
                addstr(element.first.c_str());
                attroff(COLOR_PAIR(element.second));

            }

            refresh();

        }
    };

#endif