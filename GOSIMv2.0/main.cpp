#include "GOSIM.h"
#include "utilities.h"
#include "stdio.h"

using namespace std;

int main() {

    struct timeval startTime;
    // get the starting time
    gettimeofday(&startTime, NULL);
    srand(startTime.tv_sec + startTime.tv_usec);
    rand();

///------------------------------------------------------------------------------
    //1. 2D binary channels unconditional simulation
    int VarNum = 1;

    // set the pattern size
    int patternSizex = 11;
    int patternSizey = 11;
    int patternSizez = 1;

    int Search_iterations = 5; // the iteration number for pattern search

    //the iteration number for each scale
    vector<int> EM_iterations;

    EM_iterations.push_back(20);
    EM_iterations.push_back(15);

    //it is OK to change the iteration numbers to 3, 2, 1 for the finer scales
    EM_iterations.push_back(5);
    EM_iterations.push_back(4);
    EM_iterations.push_back(3);

    int scales = EM_iterations.size();

    bool IsCategorical = true; // is it categorical data?
    bool histmatch = false; // does it need histogram matching?
    bool Is3D = false; // 3D data?
    bool IsConditional = true; // conditional simulation?

    vector<float> w; // conditioning data weight
    w.push_back(0.0); // zero for unconditional simulation is OK

    string variable_name = "facies";
    string output_prefix = "Output/stre_u_sim";

    int resizefactorx = 2;
    int resizefactory = 2;
    int resizefactorz = 1;

    int simgridx = 250;
    int simgridy = 250;
    int simgridz = 1;

    int CategoryNum = 2;

    float low = 0.0;
    float high = 1.0;

    GOSIM go_sim(VarNum, patternSizex, patternSizey, patternSizez,
                              Search_iterations, scales, EM_iterations,
                              IsCategorical, Is3D, IsConditional,
                              histmatch, CategoryNum, w,
                              variable_name, output_prefix,
                              resizefactorx, resizefactory, resizefactorz,
                              simgridx, simgridy, simgridz,
                              "Data/Stre.csv","Data/StreConData50.csv",
                              low, high);

    int real_num = 3;//how many realizations do you want?

    int resizex = 1;
    int resizey = 1;
    int resizez = 1;

    Grid *real = go_sim.Real;
    Grid *ti = go_sim.TI;

    for (int i = 0; i < real_num; ++i) {

        std::cout << "real_num = " << i+1 << "\n";
        clock_t start, finish, temp1, temp2;
        double totaltime, temptime;
        start = clock();

        for (int j = 0; j < scales-1; ++j) {

            resizex *= resizefactorx;
            resizey *= resizefactory;
            resizez *= resizefactorz;

        }

        //create simulation grid of the coarsest size
        real = go_sim.CreateGrid(real->length/resizex,
                        real->width/resizey, real->height/resizez, VarNum, low, high);

        for (int scale = scales; scale > 0; --scale) {

            temp1 = clock();

            resizex = 1;
            resizey = 1;
            resizez = 1;

            for (int j = 0; j < scale-1; ++j) {

                resizex *= resizefactorx;
                resizey *= resizefactory;
                resizez *= resizefactorz;

            }


            ti = Resample(go_sim.TI, go_sim.TI->length/resizex,
                                go_sim.TI->width/resizey, go_sim.TI->height/resizez, VarNum);

            go_sim.GOSIM_run(ti, real, scale, i);

            temp2 = clock();
            temptime = (double)(temp2-temp1)/CLOCKS_PER_SEC;
            cout << "time of scale " << scale << " is " << temptime <<"\n";

            if (scale > 1)

                real = Resample(real, go_sim.Real->length/(resizex/resizefactorx), go_sim.Real->width/(resizey/resizefactory),
                                go_sim.Real->height/(resizez/resizefactorz), VarNum);

        }

        finish = clock();
        totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
        cout <<"Totaltime of realization " << i + 1 << " is " << totaltime <<"\n";

        stringstream ss;
        ss<<go_sim.Reals.size();
        string suffix = ".csv";
        go_sim.SaveData(go_sim.Reals[i], output_prefix + ss.str() + suffix, variable_name);

        ti = go_sim.TI;

    }

///------------------------------------------------------------------------------

///------------------------------------------------------------------------------
//    //2. 3D facies unconditional simulation
//    int VarNum = 1;
//
//    // set the pattern size
//    int patternSizex = 7;
//    int patternSizey = 7;
//    int patternSizez = 5;
//
//    int Search_iterations = 5; // the iteration number for pattern search
//
//    //the iteration number for each scale
//    vector<int> EM_iterations;
//
//    EM_iterations.push_back(20);
//    EM_iterations.push_back(4);
//    EM_iterations.push_back(3);
//    EM_iterations.push_back(2);
//
//    int scales = EM_iterations.size();
//
//    bool IsCategorical = true; // is it categorical data?
//    bool histmatch = true; // does it need histogram matching?
//    bool Is3D = true;
//    bool IsConditional = false;
//
//    string variable_name = "facies";
//    string output_prefix = "Output/facies3_u_sim";
//
//    int resizefactorx = 2;
//    int resizefactory = 2;
//    int resizefactorz = 1;
//
//    int simgridx = 100;
//    int simgridy = 130;
//    int simgridz = 10;
//
//    int CategoryNum = 3;
//
//    float low = 0.0;
//    float high = 2.0;
//
//    GOSIM go_sim(VarNum, patternSizex, patternSizey, patternSizez,
//                              Search_iterations, scales, EM_iterations,
//                              IsCategorical, Is3D, IsConditional,
//                              EmphasizeFeature, CategoryNum,
//                              variable_name, output_prefix,
//                              resizefactorx, resizefactory, resizefactorz,
//                              "Data/facies3_ti.csv","Data/facies3_hd.csv",
//                              low, high);
//
//    int real_num = 2;//how many realizations do you want?
//
//    Grid *real = go_sim.Real;
//    Grid *ti = go_sim.TI;
//
//    int resizex = 1;
//    int resizey = 1;
//    int resizez = 1;
//
//    for (int i = 0; i < real_num; ++i) {
//
//        std::cout << "real_num = " << i+1 << "\n";
//
//        clock_t start, finish;
//        double totaltime;
//        start = clock();
//
//        for (int j = 0; j < scales-1; ++j) {
//
//            resizex *= resizefactorx;
//            resizey *= resizefactory;
//            resizez *= resizefactorz;
//
//        }
//
//        //resize realization grid to the coarsest scale
//        //real = Resample(real, real->length/resizex,
//        //                real->width/resizey, real->height/resizez, VarNum);
//        real = go_sim.CreateGrid(real->length/resizex,
//                        real->width/resizey, real->height/resizez, VarNum, low, high);
//
//        for (int scale = scales; scale > 0; --scale) {
//
//            resizex = 1;
//            resizey = 1;
//            resizez = 1;
//
//            for (int j = 0; j < scale-1; ++j) {
//
//                resizex *= resizefactorx;
//                resizey *= resizefactory;
//                resizez *= resizefactorz;
//
//            }
//
//            ti = Resample(go_sim.TI, go_sim.TI->length/resizex,
//                                go_sim.TI->width/resizey, go_sim.TI->height/resizez, VarNum);
//
//            go_sim.GOSIM_run(ti, real, scale, i);
//
//            if (scale > 1)
//
//                real = Resample(real, go_sim.Real->length/(resizex/resizefactorx), go_sim.Real->width/(resizey/resizefactory),
//                                go_sim.Real->height/(resizez/resizefactorz), VarNum);
//
//        }
//        finish = clock();
//        totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
//
//        stringstream ss;
//        ss<<go_sim.Reals.size();
//        string suffix = ".csv";
//
//        go_sim.SaveData(go_sim.Reals[i], output_prefix + ss.str() + suffix, variable_name);
////
//        cout <<"Totaltime of realization " << i + 1 << " is " << totaltime <<"\n";
//        ti = go_sim.TI;
//        //go_sim.Real = go_sim.CreateGrid(ti->length, ti->width, ti->height, VarNum, low, high);
//    }

    //Grid *etype = go_sim.Etype();

    //go_sim.SaveData(etype, "etype.csv", "average");

///------------------------------------------------------------------------------
//    //3. 2D binary channels conditional simulation
//    int VarNum = 1;
//
//    // set the pattern size
//    int patternSizex = 11;
//    int patternSizey = 11;
//    int patternSizez = 1;
//
//    int Search_iterations = 5; // the iteration number for pattern search
//
//    //the iteration number for each scale
//    vector<int> EM_iterations;
//
//    EM_iterations.push_back(20);
//    EM_iterations.push_back(15);
//    EM_iterations.push_back(4);
//    EM_iterations.push_back(3);
//    EM_iterations.push_back(2);
//
//    int scales = EM_iterations.size();
//
//    bool IsCategorical = true; // is it categorical data?
//    bool histmatch = false; // histogram matching?
//    bool Is3D = false;
//    bool IsConditional = true;
//
//    vector<float> cdweight;
//    cdweight.push_back(0);
//    cdweight.push_back(0.1);
//    cdweight.push_back(0.15);
//    cdweight.push_back(0.2);
//    cdweight.push_back(0.2);
//
//    string variable_name = "facies";
//    string output_prefix = "Output/stre_c10_sim";
//
//    int resizefactorx = 2;
//    int resizefactory = 2;
//    int resizefactorz = 1;
//
//    int simgridx = 250;
//    int simgridy = 250;
//    int simgridz = 1;
//
//    int CategoryNum = 2;
//
//    float low = 0.0;
//    float high = 1.0;
//
//    GOSIM go_sim(VarNum, patternSizex, patternSizey, patternSizez,
//                              Search_iterations, scales, EM_iterations,
//                              IsCategorical, Is3D, IsConditional,
//                              histmatch, CategoryNum, cdweight,
//                              variable_name, output_prefix,
//                              resizefactorx, resizefactory, resizefactorz,
//                              simgridx, simgridy, simgridz,
//                              "Data/Stre.csv","Data/StreConData10.csv",
//                              low, high);
//
//    //Grid TI = go_sim.LoadData("Stre.csv"); // load TI data
//
//    int real_num = 50;//how many realizations do you want?
//
//    Grid *real = go_sim.Real;
//    Grid *ti = go_sim.TI;
//
//    int resizex = 1;
//    int resizey = 1;
//    int resizez = 1;
//
//    for (int i = 0; i < real_num; ++i) {
//
//        std::cout << "real_num = " << i+1 << "\n";
//
//        for (int j = 0; j < scales-1; ++j) {
//
//            resizex *= resizefactorx;
//            resizey *= resizefactory;
//            resizez *= resizefactorz;
//
//        }
//
//        real = go_sim.CreateGrid(real->length/resizex,
//                        real->width/resizey, real->height/resizez, VarNum, low, high);
//
//        for (int scale = scales; scale > 0; --scale) {
//
//            resizex = 1;
//            resizey = 1;
//            resizez = 1;
//
//            for (int j = 0; j < scale-1; ++j) {
//
//                resizex *= resizefactorx;
//                resizey *= resizefactory;
//                resizez *= resizefactorz;
//
//            }
//
//            ti = Resample(go_sim.TI, go_sim.TI->length/resizex,
//                                go_sim.TI->width/resizey, go_sim.TI->height/resizez, VarNum);
//
//            go_sim.GOSIM_run(ti, real, scale, i);
//
//            if (scale > 1)
//
//                real = Resample(real, go_sim.Real->length/(resizex/resizefactorx), go_sim.Real->width/(resizey/resizefactory),
//                                go_sim.Real->height/(resizez/resizefactorz), VarNum);
//
//        }
//
//        stringstream ss;
//        ss<<go_sim.Reals.size();
//        string suffix = ".csv";
//        go_sim.SaveData(go_sim.Reals[i], output_prefix + ss.str() + suffix, variable_name);
//
//        ti = go_sim.TI;
//        //go_sim.Real = go_sim.CreateGrid(ti->length, ti->width, ti->height, VarNum, low, high);
//    }
//
//    Grid *etype = go_sim.Etype();
//
//    go_sim.SaveData(etype, "Output/etype_c10.csv", "average");

///-----------------------------------------------------------------------------

//    //4. 3D facies conditional simulation
//    int VarNum = 1;
//
//    // set the pattern size
//    int patternSizex = 7;
//    int patternSizey = 7;
//    int patternSizez = 5;
//
//    int Search_iterations = 5; // the iteration number for pattern search
//
//    //the iteration number for each scale
//    vector<int> EM_iterations;
//    EM_iterations.push_back(20);
//    EM_iterations.push_back(4);
//    EM_iterations.push_back(3);
//    EM_iterations.push_back(2);
//
//    int scales = EM_iterations.size();
//
//    bool IsCategorical = true; // is it categorical data?
//    bool histmatch = true; // does it need histogram matching?
//    bool Is3D = true;
//    bool IsConditional = true;
//
//    vector<float> cdweight;
//    cdweight.push_back(0.1);
//    cdweight.push_back(0.2);
//    cdweight.push_back(0.3);
//    cdweight.push_back(0.3);
//
//    int simgridx = 100;
//    int simgridy = 130;
//    int simgridz = 10;
//
//    string variable_name = "facies";
//    string output_prefix = "Output/facies3_c_sim";
//
//    int resizefactorx = 2;
//    int resizefactory = 2;
//    int resizefactorz = 1;
//
//    int CategoryNum = 3;
//
//    float low = 0.0;
//    float high = 2.0;
//
//    GOSIM go_sim(VarNum, patternSizex, patternSizey, patternSizez,
//                              Search_iterations, scales, EM_iterations,
//                              IsCategorical, Is3D, IsConditional,
//                              histmatch, CategoryNum, cdweight,
//                              variable_name, output_prefix,
//                              resizefactorx, resizefactory, resizefactorz,
//                              simgridx, simgridy, simgridz,
//                              "Data/facies3_ti.csv","Data/facies3_hd.csv",
//                              low, high);
////
//
//    int real_num = 50;//how many realizations do you want?
//
//    Grid *real = go_sim.Real;
//    Grid *ti = go_sim.TI;
//
//    int resizex = 1;
//    int resizey = 1;
//    int resizez = 1;
//
//
//    for (int i = 0; i < real_num; ++i) {
//
//        std::cout << "real_num = " << i+1 << "\n";
//
//        for (int j = 0; j < scales-1; ++j) {
//
//            resizex *= resizefactorx;
//            resizey *= resizefactory;
//            resizez *= resizefactorz;
//
//        }
//
//        real = go_sim.CreateGrid(real->length/resizex,
//                        real->width/resizey, real->height/resizez, VarNum, low, high);
//
//        for (int scale = scales; scale > 0; --scale) {
//
//            resizex = 1;
//            resizey = 1;
//            resizez = 1;
//
//            for (int j = 0; j < scale-1; ++j) {
//
//                resizex *= resizefactorx;
//                resizey *= resizefactory;
//                resizez *= resizefactorz;
//
//            }
//
//            ti = Resample(go_sim.TI, go_sim.TI->length/resizex,
//                                go_sim.TI->width/resizey, go_sim.TI->height/resizez, VarNum);
//
//            go_sim.GOSIM_run(ti, real, scale, i);
//
//            if (scale > 1)
//
//                real = Resample(real, go_sim.Real->length/(resizex/resizefactorx), go_sim.Real->width/(resizey/resizefactory),
//                                go_sim.Real->height/(resizez/resizefactorz), VarNum);
//
//        }
//
//        stringstream ss;
//        ss<<go_sim.Reals.size();
//        string suffix = ".csv";
//
//        go_sim.SaveData(go_sim.Reals[i], output_prefix + ss.str() + suffix, variable_name);
//
//        ti = go_sim.TI;
//    }
//
//    Grid *etype = go_sim.Etype();
//
//    go_sim.SaveData(etype, "Output/etype_3d.csv", "average");

///-----------------------------------------------------------------------------

///------------------------------------------------------------------------------
//    //5. 2D continuous data simulation
//    int VarNum = 1;
//
//    // set the pattern size
//    int patternSizex = 13;
//    int patternSizey = 13;
//    int patternSizez = 1;
//
//    int Search_iterations = 5; // the iteration number for pattern search
//
//    //the iteration number for each scale
//    vector<int> EM_iterations;
//    EM_iterations.push_back(15);
//    EM_iterations.push_back(10);
//    EM_iterations.push_back(4);
//    EM_iterations.push_back(3);
//
//    vector<float> w; // conditioning data weight
//    w.push_back(0.0); // zero for unconditional simulation is OK
//
//    int scales = EM_iterations.size();
//
//    bool IsCategorical = false; // is it categorical data?
//    bool histmatch = false; // does it need any highlight of some features?
//    bool Is3D = false;
//    bool IsConditional = false;
//
//    string variable_name = "cracks";
//    string output_prefix = "Output/crack_u_sim";
//
//    int resizefactorx = 2;
//    int resizefactory = 2;
//    int resizefactorz = 1;
//
//    int simgridx = 159;
//    int simgridy = 159;
//    int simgridz = 1;
//
//    int CategoryNum = 2;
//
//    float low = 0.0;
//    float high = 1.0;
//
//    GOSIM go_sim(VarNum, patternSizex, patternSizey, patternSizez,
//                              Search_iterations, scales, EM_iterations,
//                              IsCategorical, Is3D, IsConditional,
//                              histmatch, CategoryNum,w,
//                              variable_name, output_prefix,
//                              resizefactorx, resizefactory, resizefactorz,
//                              simgridx, simgridy, simgridz,
//                              "Data/crack.csv","Data/crack.csv",
//                              low, high);
//
//    int real_num = 3;//how many realizations do you want?
//
//    Grid *real = go_sim.Real;
//    Grid *ti = go_sim.TI;
//
//    int resizex = 1;
//    int resizey = 1;
//    int resizez = 1;
//
//    for (int i = 0; i < real_num; ++i) {
//
//        std::cout << "real_num = " << i+1 << "\n";
//        clock_t start, finish;
//        double totaltime;
//        start = clock();
//
//        for (int j = 0; j < scales-1; ++j) {
//
//            resizex *= resizefactorx;
//            resizey *= resizefactory;
//            resizez *= resizefactorz;
//
//        }
//
//        real = go_sim.CreateGrid(real->length/resizex,
//                        real->width/resizey, real->height/resizez, VarNum, low, high);
//
//        for (int scale = scales; scale > 0; --scale) {
//
//            resizex = 1;
//            resizey = 1;
//            resizez = 1;
//
//            for (int j = 0; j < scale-1; ++j) {
//
//                resizex *= resizefactorx;
//                resizey *= resizefactory;
//                resizez *= resizefactorz;
//
//            }
//
//            ti = Resample(go_sim.TI, go_sim.TI->length/resizex,
//                                go_sim.TI->width/resizey, go_sim.TI->height/resizez, VarNum);
//
//            go_sim.GOSIM_run(ti, real, scale, i);
//
//            if (scale > 1)
//
//                real = Resample(real, go_sim.Real->length/(resizex/resizefactorx), go_sim.Real->width/(resizey/resizefactory),
//                                go_sim.Real->height/(resizez/resizefactorz), VarNum);
//
//        }
//
//        finish = clock();
//        totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
//        cout <<"Totaltime of realization " << i << " is " << totaltime <<"\n";
//
//        stringstream ss;
//        //ss<<real_num;
//        ss<<go_sim.Reals.size();
//        string suffix = ".csv";
//        //    ss>>suffix;
//        go_sim.SaveData(go_sim.Reals[i], output_prefix + ss.str() + suffix, variable_name);
//
//        ti = go_sim.TI;
//    }

///------------------------------------------------------------------------------

    return 0;

}

