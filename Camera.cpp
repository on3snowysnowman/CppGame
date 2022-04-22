#include <vector>

#include "Tilemap.cpp"
#include "Renderer.cpp"

class Camera{

    public:

        Tilemap *tilemap; //Tilemap instance
        Renderer *renderer; // Renderer instance

        Camera(Tilemap &tilemap_instance, Renderer &renderer_instance){

            tilemap = &tilemap_instance;
            renderer = &renderer_instance;
        }

        void flush(){

            /*
            Flushes the data to render from our tilemap object to our renderer
            */

            for(vector<vector<BaseCharacter *>> column : tilemap->tilemap_data){

                for(vector<BaseCharacter *> row : column){
     
                    renderer->add_content(row.at(0)->character, false, row.at(0)->color);
                }

                renderer->add_new_line();
            }
        }

};