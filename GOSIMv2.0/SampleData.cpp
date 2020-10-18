#include "GOSIM.h"

Grid * GOSIM::SampleData(int dimension, int SampleNum) {

    Grid *condata = new Grid(Real->length, Real->width, Real->height, 1);

    if (dimension == 3) {

        for (int k = 0; k < Real->height; ++k)
            for (int j = 0; j < Real->width; ++j)
                for (int i = 0; i < Real->length; ++i) {

                        (*condata)(i, j, k, 0) = -996699;

        }

        for (int i = 0; i < SampleNum; i++) {

            int delta = 0;

            int rx = randomInt(0, Real->length - 1);
            int ry = randomInt(0, Real->width - 1);

            for (int z = 0; z < Real->height; ++z) {

                float value = (*TI)(rx+delta, ry+delta, z, 0);

                (*condata)(rx, ry, z, 0) = value;

            }

        }

    }

    return condata;


}
