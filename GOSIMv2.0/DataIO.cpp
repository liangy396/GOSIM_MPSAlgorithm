#include "GOSIM.h"
#include "stdio.h"
#include <cmath>

//------------------------------------------------------------------------------
//load training image, the format should look like the example TI
//------------------------------------------------------------------------------

Grid *GOSIM::LoadData(string filename) {

    FILE *f = fopen(filename.c_str(), "r");

    int length = 0;
    int width = 0;
    int height = 0;

    //number of variables, should be 1 in current version
    int var_num = 1;

    fseek(f, 0, SEEK_SET);
    fscanf(f, "%d ", &length);
    fscanf(f, "%d ", &width);
    fscanf(f, "%d\n", &height);

    char c;
    //skip two lines of the input file
    do {
        c = fgetc(f);
    } while(c !='\n');

    do {
        c = fgetc(f);
    } while(c !='\n');

    Grid *data = new Grid(length, width, height, var_num);

    for (int z = 0; z < height; z++) {
        for (int y = 0; y < width; y++) {
            for (int x = 0; x < length; x++) {

                fscanf(f, "%f\n", &(*data)(x, y, z, 0));

            }
        }
    }

    fclose(f);

    return data;
}

//------------------------------------------------------------------------------
//save the realization to SGEMS files
//------------------------------------------------------------------------------

void GOSIM::SaveData(Grid *grid, string filename, string var_name) {

    FILE *f = fopen(filename.c_str(), "w");
    fprintf(f, "%d %d %d\n", grid->length, grid->width, grid->height);

    int var_num = 1;

    fprintf(f, "%d\n", var_num);
    fprintf(f, "%s\n", var_name.c_str());

    for (int z = 0; z < grid->height; ++z) {
        for (int y = 0; y < grid->width; ++y) {
            for (int x = 0; x < grid->length; ++x) {

                fprintf(f, "%10.3f\n", (*grid)(x, y, z, 0));

            }
        }
    }

    fclose(f);
}

//save the hard data for displaying
void GOSIM::SaveData2(Grid *grid, string filename, string var_name) {

    FILE *f = fopen(filename.c_str(), "w");

    fprintf(f, "hard_data\n");

    int var = 4;
    fprintf(f, "%d\n", var);
    fprintf(f, "X\n");
    fprintf(f, "Y\n");
    fprintf(f, "Z\n");
    fprintf(f, "%s\n", var_name.c_str());

    for (int z = 0; z < grid->height; ++z) {
        for (int y = 0; y < grid->width; ++y) {
            for (int x = 0; x < grid->length; ++x) {

                float value = (*grid)(x, y, z, 0);
                if (value != -996699)

                    fprintf(f,"%d %d %d %10.10f\n", x, y, z, value);

            }
        }
    }


    fclose(f);
}
