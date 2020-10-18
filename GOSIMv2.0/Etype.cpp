#include "GOSIM.h"

Grid* GOSIM::Etype() {

    int length = Reals[0]->length;
    int width = Reals[0]->width;
    int height = Reals[0]->height;

    Grid *grid = new Grid(length, width, height, 1);

    for(int i = 0; i < Reals.size(); ++i)
        for (int z = 0; z < height; ++z)
            for (int y = 0; y < width; ++y)
                for (int x = 0; x < length; ++x) {

                    (*grid)(x, y, z, 0) += (*Reals[i])(x, y, z, 0);

                }

    for (int z = 0; z < height; ++z)
        for (int y = 0; y < width; ++y)
            for (int x = 0; x < length; ++x) {

                (*grid)(x, y, z, 0) /= Reals.size();

            }

    return grid;

}


