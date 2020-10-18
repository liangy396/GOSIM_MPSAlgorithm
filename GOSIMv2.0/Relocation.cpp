#include "GOSIM.h"

Grid *GOSIM::Relocation(Grid *condata, int resizex, int resizey, int resizez) {

    int length = condata->length / resizex;
    int width = condata->width / resizey;
    int height = condata->height / resizez;

    Grid *newgrid = new Grid(length, width, height, VarNum);

    for (int v = 0; v < VarNum; ++v) {
        for (int z = 0; z < newgrid->height; ++z) {
            for (int y = 0; y < newgrid->width; ++y) {
                for (int x = 0; x < newgrid->length; ++x) {

                    float val = 0.0;
                    int hard_count = 0;
                    float scale = 1.0;

                    for (int dz = 0; dz < resizez; ++dz) {
                        for (int dy = 0; dy < resizey; ++dy) {
                            for (int dx = 0; dx < resizex; ++dx) {

                                if ((*condata)(x*resizex+dx, y*resizey+dy, z*resizez+dz, v) != -996699) {

                                    hard_count++;

                                    val += (*condata)(x*resizex+dx, y*resizey+dy, z*resizez+dz, v);

                                }

                            }
                        }
                    }

                    if (hard_count != 0) {
                        scale = 1.0f/hard_count;
                        (*newgrid)(x, y, z, v) = val * scale;
                    }
                    else
                        (*newgrid)(x, y, z, v) = -996699;

                }
            }
        }
    }

    return newgrid;

}
