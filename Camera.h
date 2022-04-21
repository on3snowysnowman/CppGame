#include <iostream>
#include <vector>
#include <tilemap.h>
#include <Renderer.h>

class Camera{

    public:

        //Tilemap instance
        vector<vector<vector<BaseCharacter *>>> *tilemap;
        Renderer *renderer;

        Camera(vector<vector<vector<BaseCharacter *>>>& tilemapInstance, Renderer *targ_renderer){

            tilemap = &tilemapInstance;
            renderer = targ_renderer;

        }

        void flush(){

            /*
            Flushes the data to render from our tilemap object to our renderer
            */

            for(vector<vector<BaseCharacter *>> column : *tilemap){

                for(vector<BaseCharacter *> row : column){
     
                    renderer->add_content(to_string(row.at(0)->character));
                }

                renderer->add_content("\n");
            }
        }
};