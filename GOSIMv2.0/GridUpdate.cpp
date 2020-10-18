#include "GOSIM.h"

void GOSIM::GridUpdate(Grid *ti, Grid *real, Grid *errmap) {

    //test
    int bin_num = 20;

    //calculate histogram each time
    Grid *hist_ti = HistCalc(ti, bin_num, minvalue_ti, maxvalue_ti);
    Grid *hist_real = HistCalc(real, bin_num, minvalue_real, maxvalue_real);

    Grid out(real->length, real->width, real->height, VarNum+1);

    //Define a random path
    Grid randompath(real->length, real->width, real->height, 3);

    //Initialize the random path
    for (int z = 0; z < real->height; ++z) {
        for (int y = 0; y < real->width; ++y) {
            for (int x = 0; x < real->length; ++x) {
                randompath(x, y, z, 0) = x;
                randompath(x, y, z, 1) = y;
                randompath(x, y, z, 2) = z;
            }
        }
    }

    //assign the random path
    for (int z = 0; z < real->height; ++z) {
        for (int y = 0; y < real->width; ++y) {
            for (int x = 0; x < real->length; ++x) {

                int rz = randomInt(0, real->height-1);
                int ry = randomInt(0, real->width-1);
                int rx = randomInt(0, real->length-1);
                int tempx = randompath(x, y, z, 0);
                int tempy = randompath(x, y, z, 1);
                int tempz = randompath(x, y, z, 2);
                randompath(x, y, z, 0) = randompath(rx, ry, rz, 0);
                randompath(x, y, z, 1) = randompath(rx, ry, rz, 1);
                randompath(x, y, z, 2) = randompath(rx, ry, rz, 2);
                randompath(rx, ry, rz, 0) = tempx;
                randompath(rx, ry, rz, 1) = tempy;
                randompath(rx, ry, rz, 2) = tempz;

            }
        }
    }

    if (HistMatch == true) {
    for (int h = 0; h < real->height; ++h) {
        for (int w = 0; w < real->width; ++w) {
            for (int l = 0; l < real->length; ++l) {

                int x = randompath(l, w, h, 0);
                int y = randompath(l, w, h, 1);
                int z = randompath(l, w, h, 2);

                float binwidth_real = bin_num / (maxvalue_real - minvalue_real);
                float binwidth_ti = bin_num / (maxvalue_ti - minvalue_ti);
                for (int dz = -halfPatSizez; dz <= halfPatSizez; ++dz) {
                    if (z+dz < 0)
                        continue;
                    if (z+dz >= out.height)
                        break;
                    for (int dy = -halfPatSizey; dy <= halfPatSizey; ++dy) {
                        if (y+dy < 0)
                            continue;
                        if (y+dy >= out.width)
                            break;
                        for (int dx = -halfPatSizex; dx <= halfPatSizex; ++dx) {
                            if (x+dx < 0)
                                continue;
                            if (x+dx >= out.length)
                                break;

                            int offsetx = (int)(*errmap)(x+dx, y+dy, z+dz, 0);
                            int offsety = (int)(*errmap)(x+dx, y+dy, z+dz, 1);
                            int offsetz = (int)(*errmap)(x+dx, y+dy, z+dz, 2);

                            float weight = 1.0/((*errmap)(x+dx, y+dy, z+dz, 3) + 1.0);
                            //float weight = 1.0;

                            //reweighting for histogram
                            float something = 0.0;

                            for (int v = 0; v < VarNum; ++v) {

                                float value = (*ti)(offsetx-dx, offsety-dy, offsetz-dz, v);

                                int bin_real = (int)((value - minvalue_real) * binwidth_real);
                                if (bin_real >= bin_num) {
                                    bin_real = bin_num-1;
                                }
                                if (bin_real < 0) {
                                    bin_real = 0;
                                }

                                int bin_ti = (int)((value - minvalue_ti) * binwidth_ti);
                                if (bin_ti >= bin_num) {
                                    bin_ti = bin_num-1;
                                }
                                if (bin_ti < 0) {
                                    bin_ti = 0;
                                }
                                something += max(0.0f, (*hist_real)(bin_real, 0, 0, v) - (*hist_ti)(bin_ti, 0, 0, v));
                            }
                            weight = weight / (1.0 + something);

                            for (int v = 0; v < VarNum; v++) {
                                out(x, y, z, v) += weight*(*ti)(offsetx-dx, offsety-dy, offsetz-dz, v);
                            }

                            out(x, y, z, VarNum) += weight;
                        }
                    }

                    //calculate the old bin
                    int bin_real_old = (int)(((*real)(x, y, z, 0) - minvalue_real) * binwidth_real);
                    if (bin_real_old >= bin_num) {
                        bin_real_old = bin_num-1;
                    }
                    if (bin_real_old < 0) {
                        bin_real_old = 0;
                    }

                    //update the voxel(assuming only 1 variable)
                    (*real)(x, y, z, 0) = out(x, y, z, 0) / out(x, y, z, 1);
                    float value = (*real)(x, y, z, 0);
                    minvalue_real = min(value, minvalue_real);
                    maxvalue_real = max(value, maxvalue_real);
                    binwidth_real = bin_num / (maxvalue_real - minvalue_real);

                    //calculate the new bin
                    int bin_real_new = (int)((value - minvalue_real) * binwidth_real);
                    if (bin_real_new >= bin_num) {
                        bin_real_new = bin_num-1;
                    }
                    if (bin_real_new < 0) {
                        bin_real_new = 0;
                    }

                    //if the same bin, do nothing
                    if (bin_real_old == bin_real_new)
                        continue;

                    else {
                        //update the histogram
                        double scale = 1.0 / (real->length * real->width * real->height);
                        (*hist_real)(bin_real_old, 0, 0, 0) = ((*hist_real)(bin_real_old, 1, 0, 0) - 1) * scale;
                        (*hist_real)(bin_real_new, 0, 0, 0) = ((*hist_real)(bin_real_new, 1, 0, 0) + 1) * scale;
                    }
                }
            }
        }
    }
    }

    else {
    for (int z = 0; z < real->height; ++z) {
        for (int y = 0; y < real->width; ++y) {
            for (int x = 0; x < real->length; ++x) {

                for (int dz = -halfPatSizez; dz <= halfPatSizez; ++dz) {
                    if (z+dz < 0)
                        continue;
                    if (z+dz >= out.height)
                        break;
                    for (int dy = -halfPatSizey; dy <= halfPatSizey; ++dy) {
                        if (y+dy < 0)
                            continue;
                        if (y+dy >= out.width)
                            break;
                        for (int dx = -halfPatSizex; dx <= halfPatSizex; ++dx) {
                            if (x+dx < 0)
                                continue;
                            if (x+dx >= out.length)
                                break;

                            int offsetx = (int)(*errmap)(x+dx, y+dy, z+dz, 0);
                            int offsety = (int)(*errmap)(x+dx, y+dy, z+dz, 1);
                            int offsetz = (int)(*errmap)(x+dx, y+dy, z+dz, 2);

                            float weight = 1.0/((*errmap)(x+dx, y+dy, z+dz, 3) + 1.0);
                            //float weight = 1.0;

                            for (int v = 0; v < VarNum; v++) {
                                out(x, y, z, v) += weight*(*ti)(offsetx-dx, offsety-dy, offsetz-dz, v);
                            }

                            out(x, y, z, VarNum) += weight;
                        }
                    }


                }

                (*real)(x, y, z, 0) = out(x, y, z, 0) / out(x, y, z, 1);
            }
        }
    }
    }



}


