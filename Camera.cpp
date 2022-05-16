#include <vector>

#include "Tilemap.cpp"
#include "Renderer.cpp"

class Camera{

    public:

        Tilemap *tilemap; //Tilemap instance
        Renderer *renderer; // Renderer instance
        BaseCharacter* focus_obj;
        int radius;

        Camera(Tilemap &tilemap_instance, Renderer &renderer_instance, BaseCharacter& targ_focus_obj){

            tilemap = &tilemap_instance;
            renderer = &renderer_instance;
            focus_obj = &targ_focus_obj;
            radius = 7;
            
        }

        void flush(){

            /*
            Flushes the data to render from our tilemap object to our renderer, and limits the amount of characters printed to the passed radius around the coordinate focus point
            */

            int x_pos = focus_obj->xPos;
            int y_pos = focus_obj->yPos;

            int start_x;
            int end_x;
            int start_y;
            int end_y;

            bool x_is_short = false;
            bool y_is_short = false;

            start_x = x_pos - radius;
            if(start_x < 0){
                x_is_short = true;
                start_x = 0;
            }

            end_x = x_pos + radius + 1;
            
            if(x_is_short){
                
                end_x += radius - x_pos;
                
                if(end_x > tilemap->width){
                    end_x = tilemap->width;
                }
            }    

            else{

                if(end_x > tilemap->width){
                    
                    end_x = tilemap->width;

                    start_x = (start_x - (radius - (tilemap->width - x_pos))) - 1; 

                    if(start_x < 0){
                        start_x = 0;
                    }
                }

            }


            start_y = y_pos - radius;
            if(start_y < 0){
                y_is_short = true;
                start_y = 0;
            }

            end_y = y_pos + radius + 1;
            
            if(y_is_short){
                
                end_y += radius - y_pos;
                
                if(end_y > tilemap->height){
                    end_y = tilemap->height;
                }
            }    

            else{

                if(end_y > tilemap->height){
                    
                    end_y = tilemap->height;

                    start_y = (start_y - (radius - (tilemap->height - y_pos))) - 1; 

                    if(start_y < 0){
                        start_y = 0;
                    }
                }

            }
          

            for(int y = start_y; y < end_y; y++){
                
                for(int x = start_x; x < end_x; x++){
                    
                    if(tilemap->tilemap_data.at(y).at(x).size() > 0){
                        
                        BaseCharacter* targ_obj = tilemap->getObj(x, y, 0);
                        renderer->add_content(targ_obj->character, false, targ_obj->color);
                        renderer->add_content(" ", false);
                        continue;
                    }

                    renderer->add_content("% ", false, "Light Gray");

                }
                renderer->add_new_line();
            }
        }

        void increase_radius(){
            
            if(((radius * 2) + 1) < int(renderer->terminal_max_x / 2) && ((radius * 2) + 1) < int(renderer->terminal_max_y / 2)){
                radius += 1;
            }
        }

        void decrease_radius(){
            if(radius - 1 >= 0){
                radius -= 1;
            }
        }

        void load_tilemap(Tilemap& tilemap_instance){

            tilemap = &tilemap_instance;
        }

};