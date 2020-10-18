#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#include <string>
#include <vector>
#include <iostream>
#include "Grid.h"
#include "utilities.h"
#include <cmath>
#include "Resample.h"
#include "Classify.h"
#include <sstream>


class GOSIM {

public:

    std::vector<Grid *> Reals; // a vector of realizations

    Grid *TI; // training image
    Grid *Real; // realization
    Grid *ConData; // conditioning data

    GOSIM();

    GOSIM(int varnum, int patx, int paty, int patz,
            int searchiter, int scale_num, std::vector<int> emiters,
            bool categorical, bool threeDimension, bool conditionalsim,
            bool histmatch, int categorynum, std::vector<float> w,
            std::string varianame, std::string outprefix,
            int factorx, int factory, int factorz,
            int simgridx, int simgridy, int simgridz,
            string ti_filename, string con_filename,
            float low, float high); //construct function

    //initialize realization grid from grid size and variable numbers(only 1 variable is supported now)
    Grid *CreateGrid(int length, int width, int height, int varnum, float low, float high);

    //E-step: search the most similar patterns
    Grid *PatternSearch(Grid *ti, Grid *real, Grid *condata);

    //M-step: update the realization grid
    void GridUpdate(Grid *ti, Grid *real, Grid *errmap);

    //run GOSIM
    void GOSIM_run(Grid *ti, Grid *real, int scale, int real_num);

    Grid *LoadData(std::string filename);//load data to a Grid, support specific format only

    void SaveData(Grid *grid, std::string filename, std::string variable_name);//save data to a csv file, the same format as input

    void SaveData2(Grid *grid, std::string filename, std::string variable_name);//save hard data for displaying

    Grid *Etype();

    Grid *SampleData(int dimension, int samplenum);

private:

    bool IsCategorical;  // is the data categorical?
    bool Is3D; //is it 3D data?
    bool HistMatch; // need to match histogram?
    bool IsConditional; //conditional simulation?

    int VarNum; //how many variables used for calculation?
    std::string variable_name;
    std::string output_prefix;
    int category_num;

    int scales; //Multi-scale. How many scales?
    int currentscale; //store current scale
    int resizefactorx;
    int resizefactory;
    int resizefactorz;

    int Search_iterations;//the iteration number for pattern search process
    std::vector<int> EM_iterations;//the iteration number for EM-iterations
    std::vector<float> w;//conditioning data weight

    int patternSizex;
    int halfPatSizex;//half of the pattern size x

    int patternSizey;
    int halfPatSizey;//half of the pattern size y

    int patternSizez;
    int halfPatSizez;//half of the pattern size z

    //relocation of conditioning data
    Grid *Relocation(Grid *condata, int resizex, int resizey, int resizez);

    //Calculate the histogram
    Grid *HistCalc(Grid *grid, int buckets, float minVal, float maxVal);

    float calc_distance(Grid *ti, Grid *real, Grid *condata,
                        int ti_x, int ti_y, int ti_z,
                        int real_x, int real_y, int real_z,
                        int halfPatSizex, int halfPatSizey, int halfPatSizez,
                        float threshold);// calculate the distance, used for pattern search

    float find_min(Grid *grid);

    float find_max(Grid *grid);

    float minvalue_ti;
    float maxvalue_ti;
    float minvalue_real;
    float maxvalue_real;

    void Classify(Grid *grid, int category_num, int varnum);

};


#endif // SIMULATION_H_INCLUDED
