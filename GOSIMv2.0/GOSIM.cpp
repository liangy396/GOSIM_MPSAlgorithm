#include "GOSIM.h"
#include "stdio.h"
#include <algorithm>

GOSIM::GOSIM() {}

GOSIM::GOSIM(int varnum, int patx, int paty, int patz,
            int searchiter, int scale_num, vector<int> emiters,
            bool categorical, bool is3d, bool iscon,
            bool histmatch, int categorynum, vector<float> cdweight,
            string varianame, string outprefix,
            int factorx, int factory, int factorz,
            int simgridx, int simgridy, int simgridz,
            string ti_filename, string conda_filename,
            float low, float high) {

    VarNum = varnum;
    category_num = categorynum;

    // set the pattern size
    patternSizex = patx;
    patternSizey = paty;
    patternSizez = patz;

    Search_iterations = searchiter; // the iteration number for pattern search

    //the iteration number for each scale
    scales = scale_num;
    EM_iterations = emiters;

    IsCategorical = categorical; // is it categorical data?
    Is3D = is3d;
    HistMatch = histmatch; // does it need any highlight of some features?
    IsConditional = iscon;
    w = cdweight;

    variable_name = varianame;
    output_prefix = outprefix;

    resizefactorx = factorx;
    resizefactory = factory;
    resizefactorz = factorz;

    //calculate half of the pattern size
    halfPatSizex = patternSizex/2;
    halfPatSizey = patternSizey/2;
    halfPatSizez = patternSizez/2;

    TI = LoadData(ti_filename);
    ConData = LoadData(conda_filename);

    Real = CreateGrid(simgridx, simgridy, simgridz, VarNum, low, high);

}

Grid *GOSIM::CreateGrid(int length, int width, int height, int varnum, float low, float high) {

    Grid *newgrid = new Grid(length, width, height, varnum);

    for (int v = 0; v < varnum; ++v)
        for (int z = 0; z < height; ++z)
            for (int y = 0; y < width; ++y)
                for (int x = 0; x < length; ++x) {

                    (*newgrid)(x, y, z, v) = randomFloat(low, high);

                }

    return newgrid;

}

void GOSIM::GOSIM_run(Grid *ti, Grid *real, int scale, int real_num) {

    currentscale = scale;

    int resizex = 1;
    int resizey = 1;
    int resizez = 1;

    for (int i = 0; i < scale-1; ++i) {

        resizex *= resizefactorx;
        resizey *= resizefactory;
        resizez *= resizefactorz;

    }

    Grid *condata = Relocation(ConData, resizex, resizey, resizez);

    minvalue_ti = find_min(ti);
    maxvalue_ti = find_max(ti);
    minvalue_real = find_min(real);
    maxvalue_real = find_max(real);

    //EM-iterations
    for (int i = 0; i < EM_iterations[scales - scale]; ++i) {

        printf("."); fflush(stdout);

        Grid *errmap = PatternSearch(ti, real, condata); //E-step:search for the most similar patterns

        GridUpdate(ti, real, errmap);//M-step:update the realization

    }

    //k-means for transforming continuous variables to categorical variables
    if (scale == 1) {

        if (IsCategorical) {

            Classify(real, category_num, 1);

            vector<float> indicators;
            indicators.push_back((*real)(0, 0, 0, 0));

            for (int z = 0; z < real->height; ++z) {
                for (int y = 0; y < real->width; ++y) {
                    for (int x = 0; x < real->length; ++x) {

                        int i = 0;
                        while (i < indicators.size()) {

                            if ((*real)(x, y, z, 0) == indicators[i])
                                break;
                            else
                                ++i;
                        }

                        if (i == indicators.size())
                            indicators.push_back((*real)(x, y, z, 0));

                        if (indicators.size() >= category_num)
                            break;
                    }
                }
            }

            sort(indicators.begin(), indicators.end());

            for (int z = 0; z < real->height; ++z) {
                for (int y = 0; y < real->width; ++y) {
                    for (int x = 0; x < real->length; ++x) {
                        for (int i = 0; i < indicators.size(); ++i) {

                            if ((*real)(x, y, z, 0) == indicators[i])
                                (*real)(x, y, z, 0) = i;

                        }
                    }
                }
            }

        }

        Real = real;

        Reals.push_back(Real);

    }

    printf("\n");

}

float GOSIM::find_min(Grid *grid) {

    float minvalue = (*grid)(0,0,0,0);

    for (int z = 0; z < grid->height; ++z) {
        for (int y = 0; y < grid->width; ++y) {
            for (int x = 0; x < grid->length; ++x) {
                float temp = (*grid)(x, y, z, 0);
                minvalue = min(temp, minvalue);
            }
        }
    }

    return minvalue;
}

float GOSIM::find_max(Grid *grid) {

    float maxvalue = (*grid)(0,0,0,0);

    for (int z = 0; z < grid->height; ++z) {
        for (int y = 0; y < grid->width; ++y) {
            for (int x = 0; x < grid->length; ++x) {
                float temp = (*grid)(x, y, z, 0);
                maxvalue = max(temp, maxvalue);
            }
        }
    }

    return maxvalue;
}
