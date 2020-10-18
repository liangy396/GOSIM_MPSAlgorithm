//This is the implementations of pattern search process used in GOSIM
//The code is implemented based on the PatchMatch algorithm proposed by Barnes et al., 2009.
//Some modifications have been added to the original PatchMatch
//in the case of 3D SIMULATION and CONDITIONAL SIMULATION

#include "GOSIM.h"

Grid *GOSIM::PatternSearch(Grid *ti, Grid *real, Grid *condata) {

    Grid *errmap = new Grid(real->length, real->width, real->height, 4);

    //random initialization of the candidates
    for (int z = 0; z < real->height; ++z) {
        for (int y = 0; y < real->width; ++y) {
            for (int x = 0; x < real->length; ++x) {

                //ignore the pattern along the boundaries
                (*errmap)(x, y, z, 0) = randomInt(halfPatSizex, ti->length-halfPatSizex-1);
                (*errmap)(x, y, z, 1) = randomInt(halfPatSizey, ti->width-halfPatSizey-1);
                (*errmap)(x, y, z, 2) = randomInt(halfPatSizez, ti->height-halfPatSizez-1);
                (*errmap)(x, y, z, 3) = calc_distance(ti, real, condata,
                                            (*errmap)(x, y, z, 0), (*errmap)(x, y, z, 1), (*errmap)(x, y, z, 2),
                                            x, y, z,
                                            halfPatSizex, halfPatSizey, halfPatSizez,
                                            HUGE_VAL);
            }
        }
    }

    for (int i = 0; i < Search_iterations; ++i) {

        if (Is3D && real->height >= 2) {

            // propagation
            if (i%2 == 0) {

                for (int z = 1; z < real->height; ++z) {
                    for (int y = 1; y < real->width; ++y) {
                        for (int x = 1; x < real->length ; ++x) {

                            float DistanceLeft = calc_distance(ti, real, condata,
                                                      (*errmap)(x-1, y, z, 0) + 1,
                                                      (*errmap)(x-1, y, z, 1),
                                                      (*errmap)(x-1, y, z, 2),
                                                      x, y, z,
                                                      halfPatSizex, halfPatSizey, halfPatSizez,
                                                      (*errmap)(x, y, z, 3));

                            if (DistanceLeft < (*errmap)(x, y, z, 3)) {
                                (*errmap)(x, y, z, 0) = (*errmap)(x-1, y, z, 0) + 1;
                                (*errmap)(x, y, z, 1) = (*errmap)(x-1, y, z, 1);
                                (*errmap)(x, y, z, 2) = (*errmap)(x-1, y, z, 2);
                                (*errmap)(x, y, z, 3) = DistanceLeft;
                            }

                            float DistanceFront = calc_distance(ti, real, condata,
                                                    (*errmap)(x, y-1, z, 0),
                                                    (*errmap)(x, y-1, z, 1) + 1,
                                                    (*errmap)(x, y-1, z, 2),
                                                    x, y, z,
                                                    halfPatSizex, halfPatSizey, halfPatSizez,
                                                    (*errmap)(x, y, z, 3));

                            if (DistanceFront < (*errmap)(x, y, z, 3)) {
                                (*errmap)(x, y, z, 0) = (*errmap)(x, y-1, z, 0);
                                (*errmap)(x, y, z, 1) = (*errmap)(x, y-1, z, 1) + 1;
                                (*errmap)(x, y, z, 2) = (*errmap)(x, y-1, z, 2);
                                (*errmap)(x, y, z, 3) = DistanceFront;
                            }

                            float DistanceUp = calc_distance(ti, real, condata,
                                                    (*errmap)(x, y, z-1, 0),
                                                    (*errmap)(x, y, z-1, 1),
                                                    (*errmap)(x, y, z-1, 2) + 1,
                                                    x, y, z,
                                                    halfPatSizex, halfPatSizey, halfPatSizez,
                                                    (*errmap)(x, y, z, 3));

                            if (DistanceUp < (*errmap)(x, y, z, 3)) {
                                (*errmap)(x, y, z, 0) = (*errmap)(x, y, z-1, 0);
                                (*errmap)(x, y, z, 1) = (*errmap)(x, y, z-1, 1);
                                (*errmap)(x, y, z, 2) = (*errmap)(x, y, z-1, 2) + 1;
                                (*errmap)(x, y, z, 3) = DistanceUp;
                            }

                        }
                    }
                }

            }

        else {

            for (int z = real->height-2; z >= 0; --z) {
                for (int y = real->width-2; y >= 0; --y) {
                    for (int x = real->length-2; x >= 0; --x) {

                            float DistanceRight = calc_distance(ti, real, condata,
                                                       (*errmap)(x+1, y, z, 0)-1,
                                                       (*errmap)(x+1, y, z, 1),
                                                       (*errmap)(x+1, y, z, 2),
                                                       x, y, z,
                                                       halfPatSizex, halfPatSizey, halfPatSizez,
                                                       (*errmap)(x, y, z, 3));

                            if (DistanceRight < (*errmap)(x, y, z, 3)) {
                                (*errmap)(x, y, z, 0) = (*errmap)(x+1, y, z, 0)-1;
                                (*errmap)(x, y, z, 1) = (*errmap)(x+1, y, z, 1);
                                (*errmap)(x, y, z, 2) = (*errmap)(x+1, y, z, 2);
                                (*errmap)(x, y, z, 3) = DistanceRight;
                            }

                            float DistanceBack = calc_distance(ti, real, condata,
                                                      (*errmap)(x, y+1, z, 0),
                                                      (*errmap)(x, y+1, z, 1)-1,
                                                      (*errmap)(x, y+1, z, 2),
                                                      x, y, z,
                                                      halfPatSizex, halfPatSizey, halfPatSizez,
                                                      (*errmap)(x, y, z, 3));

                            if (DistanceBack < (*errmap)(x, y, z, 3)) {
                                (*errmap)(x, y, z, 0) = (*errmap)(x, y+1, z, 0);
                                (*errmap)(x, y, z, 1) = (*errmap)(x, y+1, z, 1)-1;
                                (*errmap)(x, y, z, 2) = (*errmap)(x, y+1, z, 2);
                                (*errmap)(x, y, z, 3) = DistanceBack;
                            }

                            float DistanceDown = calc_distance(ti, real, condata,
                                                      (*errmap)(x, y, z+1, 0),
                                                      (*errmap)(x, y, z+1, 1),
                                                      (*errmap)(x, y, z+1, 2)-1,
                                                      x, y, z,
                                                      halfPatSizex, halfPatSizey, halfPatSizez,
                                                      (*errmap)(x, y, z, 3));

                            if (DistanceDown < (*errmap)(x, y, z, 3)) {
                                (*errmap)(x, y, z, 0) = (*errmap)(x, y, z+1, 0);
                                (*errmap)(x, y, z, 1) = (*errmap)(x, y, z+1, 1);
                                (*errmap)(x, y, z, 2) = (*errmap)(x, y, z+1, 2)-1;
                                (*errmap)(x, y, z, 3) = DistanceDown;
                            }

                        }
                    }
                }
            }

            // random search in exponentially decreasing windows
            for (int z = 0; z < real->height; ++z) {
                for (int y = 0; y < real->width; ++y) {
                    for (int x = 0; x < real->length; ++x) {

                        int windowSize = ti->length;
                        if (ti->length < ti->width)
                            windowSize = ti->width;

                        while (windowSize > halfPatSizez) {

                            int minX = (int)(*errmap)(x, y, z, 0) - windowSize;
                            int maxX = (int)(*errmap)(x, y, z, 0) + windowSize + 1;
                            int minY = (int)(*errmap)(x, y, z, 1) - windowSize;
                            int maxY = (int)(*errmap)(x, y, z, 1) + windowSize + 1;
                            int minZ = (int)(*errmap)(x, y, z, 2) - windowSize;
                            int maxZ = (int)(*errmap)(x, y, z, 2) + windowSize + 1;
                            if (minX < 0) { minX = 0; }
                            if (maxX > ti->length) { maxX = ti->length; }
                            if (minY < 0) { minY = 0; }
                            if (maxY > ti->width) { maxY = ti->width; }
                            if (minZ < 0) { minZ = 0; }
                            if (maxZ > ti->height) { maxZ = ti->height; }

                            int randomX = randomInt(minX, maxX-1);
                            int randomY = randomInt(minY, maxY-1);
                            int randomZ = randomInt(minZ, maxZ-1);
                            float DistanceRandom = calc_distance(ti, real, condata,
                                                    randomX, randomY, randomZ,
                                                    x, y, z,
                                                    halfPatSizex, halfPatSizey, halfPatSizez,
                                                    (*errmap)(x, y, z, 3));
                            if (DistanceRandom < (*errmap)(x, y, z, 3)) {
                            (*errmap)(x, y, z, 0) = randomX;
                            (*errmap)(x, y, z, 1) = randomY;
                            (*errmap)(x, y, z, 2) = randomZ;
                            (*errmap)(x, y, z, 3) = DistanceRandom;
                            }

                            windowSize = (int)(0.5*windowSize);

                        }
                    }
                }
            }

        }

        else {

            // propagation
            if (i%2 != 0) {

                for (int z = 0; z < real->height; ++z) {
                    for (int y = 1; y < real->width; ++y) {
                        for (int x = 1; x < real->length ; ++x) {

                                float DistanceLeft = calc_distance(ti, real, condata,
                                                      (*errmap)(x-1, y, z, 0) + 1,
                                                      (*errmap)(x-1, y, z, 1),
                                                      (*errmap)(x-1, y, z, 2),
                                                      x, y, z,
                                                      halfPatSizex, halfPatSizey, halfPatSizez,
                                                      (*errmap)(x, y, z, 3));

                                if (DistanceLeft < (*errmap)(x, y, z, 3)) {
                                    (*errmap)(x, y, z, 0) = (*errmap)(x-1, y, z, 0) + 1;
                                    (*errmap)(x, y, z, 1) = (*errmap)(x-1, y, z, 1);
                                    (*errmap)(x, y, z, 2) = (*errmap)(x-1, y, z, 2);
                                    (*errmap)(x, y, z, 3) = DistanceLeft;
                                }


                                float DistanceUp = calc_distance(ti, real, condata,
                                                    (*errmap)(x, y-1, z, 0),
                                                    (*errmap)(x, y-1, z, 1) + 1,
                                                    (*errmap)(x, y-1, z, 2),
                                                    x, y, z,
                                                    halfPatSizex, halfPatSizey, halfPatSizez,
                                                    (*errmap)(x, y, z, 3));

                                if (DistanceUp < (*errmap)(x, y, z, 3)) {
                                    (*errmap)(x, y, z, 0) = (*errmap)(x, y-1, z, 0);
                                    (*errmap)(x, y, z, 1) = (*errmap)(x, y-1, z, 1) + 1;
                                    (*errmap)(x, y, z, 2) = (*errmap)(x, y-1, z, 2);
                                    (*errmap)(x, y, z, 3) = DistanceUp;
                                }


                        }
                    }
                }

            }

            else {

                for (int z = real->height-1; z >= 0; --z) {
                    for (int y = real->width-2; y >= 0; --y) {
                        for (int x = real->length-2; x >= 0; --x) {

                                float DistanceRight = calc_distance(ti, real, condata,
                                                       (*errmap)(x+1, y, z, 0)-1,
                                                       (*errmap)(x+1, y, z, 1),
                                                       (*errmap)(x+1, y, z, 2),
                                                       x, y, z,
                                                       halfPatSizex, halfPatSizey, halfPatSizez,
                                                       (*errmap)(x, y, z, 3));

                                if (DistanceRight < (*errmap)(x, y, z, 3)) {
                                    (*errmap)(x, y, z, 0) = (*errmap)(x+1, y, z, 0)-1;
                                    (*errmap)(x, y, z, 1) = (*errmap)(x+1, y, z, 1);
                                    (*errmap)(x, y, z, 2) = (*errmap)(x+1, y, z, 2);
                                    (*errmap)(x, y, z, 3) = DistanceRight;
                                }

                                float DistanceDown = calc_distance(ti, real, condata,
                                                      (*errmap)(x, y+1, z, 0),
                                                      (*errmap)(x, y+1, z, 1)-1,
                                                      (*errmap)(x, y+1, z, 2),
                                                      x, y, z,
                                                      halfPatSizex, halfPatSizey, halfPatSizez,
                                                      (*errmap)(x, y, z, 3));

                                if (DistanceDown < (*errmap)(x, y, z, 3)) {
                                    (*errmap)(x, y, z, 0) = (*errmap)(x, y+1, z, 0);
                                    (*errmap)(x, y, z, 1) = (*errmap)(x, y+1, z, 1)-1;
                                    (*errmap)(x, y, z, 2) = (*errmap)(x, y+1, z, 2);
                                    (*errmap)(x, y, z, 3) = DistanceDown;
                                }


                        }
                    }
                }
            }

            // random search in exponentially decreasing windows
            for (int z = 0; z < real->height; ++z) {
                for (int y = 0; y < real->width; ++y) {
                    for (int x = 0; x < real->length; ++x) {

                        int windowSize = ti->length;
                        if (ti->length < ti->width)
                            windowSize = ti->width;

                        while (windowSize > halfPatSizex) {

                            int minX = (int)(*errmap)(x, y, z, 0) - windowSize;
                            int maxX = (int)(*errmap)(x, y, z, 0) + windowSize + 1;
                            int minY = (int)(*errmap)(x, y, z, 1) - windowSize;
                            int maxY = (int)(*errmap)(x, y, z, 1) + windowSize + 1;
                            if (minX < 0) { minX = 0; }
                            if (maxX > ti->length) { maxX = ti->length; }
                            if (minY < 0) { minY = 0; }
                            if (maxY > ti->width) { maxY = ti->width; }

                            int randomX = randomInt(minX, maxX-1);
                            int randomY = randomInt(minY, maxY-1);
                            int randomZ = randomInt(0, ti->height - 1);
                            float DistanceRandom = calc_distance(ti, real, condata,
                                                    randomX, randomY, randomZ,
                                                    x, y, z,
                                                    halfPatSizex, halfPatSizey, halfPatSizez,
                                                    (*errmap)(x, y, z, 3));
                            if (DistanceRandom < (*errmap)(x, y, z, 3)) {
                                (*errmap)(x, y, z, 0) = randomX;
                                (*errmap)(x, y, z, 1) = randomY;
                                (*errmap)(x, y, z, 2) = randomZ;
                                (*errmap)(x, y, z, 3) = DistanceRandom;
                            }

                            windowSize = (int)(0.5*windowSize);

                        }
                    }
                }
            }
        }

    }

    return errmap;
}

float GOSIM::calc_distance(Grid *ti, Grid *real, Grid *condata,
                           int ti_x, int ti_y, int ti_z,
                           int real_x, int real_y, int real_z,
                           int halfPatSizex, int halfPatSizey, int halfPatSizez,
                           float threshold) {

    if (ti_x < halfPatSizex ||
        ti_x >= ti->length-halfPatSizex ||
        ti_y < halfPatSizey ||
        ti_y >= ti->width-halfPatSizey ||
        ti_z < halfPatSizez ||
        ti_z >= ti->height-halfPatSizez) {
        return HUGE_VAL;
    }

    float distance = 0.0;

    int xmin = max(-halfPatSizex, -real_x, -ti_x);
    int xmax = min(halfPatSizex, -real_x+real->length-1, -ti_x+ti->length-1);
    int ymin = max(-halfPatSizey, -real_y, -ti_y);
    int ymax = min(halfPatSizey, -real_y+real->width-1, -ti_y+ti->width-1);
    int zmin = max(-halfPatSizez, -real_z, -ti_z);
    int zmax = min(halfPatSizez, -real_z+real->height-1, -ti_z+ti->height-1);

    if (IsConditional == false) {

        for (int v = 0; v < VarNum; ++v) {
            for (int z = zmin; z <= zmax; ++z) {
                for (int y = ymin; y <= ymax; ++y) {
                    for (int x = xmin; x <= xmax; ++x) {

                        float dist_temp = (*real)(real_x+x, real_y+y, real_z+z, v) - (*ti)(ti_x+x, ti_y+y, ti_z+z, v);
                        distance += dist_temp * dist_temp;

                        if (distance > threshold) {return HUGE_VAL;}

                    }
                }
            }
        }
    }

    else {

        float disthd = 0.0;
        float distco = 0.0;

        int hdcount = 0;
        int regionsize = 0;

        for (int v = 0; v < VarNum; ++v) {
            for (int z = zmin; z <= zmax; ++z) {
                for (int y = ymin; y <= ymax; ++y) {
                    for (int x = xmin; x <= xmax; ++x) {

                        if ((*condata)(real_x+x, real_y+y, real_z+z, v) != -996699) {
                            hdcount++;
                            float delta = (*condata)(real_x+x, real_y+y, real_z+z, v) - (*ti)(ti_x+x, ti_y+y, ti_z+z, v);
                            disthd += delta * delta;
                        }
                        else {
                            regionsize++;
                            float delta = (*real)(real_x+x, real_y+y, real_z+z, v) - (*ti)(ti_x+x, ti_y+y, ti_z+z, v);
                            distco += delta * delta;
                        }
                    }
                }
            }

            if (hdcount != 0)
                distance = (1-w[scales-currentscale])/regionsize*distco + w[scales-currentscale]/hdcount*disthd;
            else
                distance = distco/regionsize;

        }

        return distance;
    }
}
