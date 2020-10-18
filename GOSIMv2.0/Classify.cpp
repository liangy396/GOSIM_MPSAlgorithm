// K-means is implemented according to Arthur and Vassilvitskii (2007)

#include "GOSIM.h"
#include "utilities.h"
#include <vector>

void GOSIM::Classify(Grid *grid, int category_num, int varnum) {

    vector< vector<float> > cluster, tempCluster;
    vector<int> tempClusterMembers(category_num);

    for (int v = 0; v < varnum; ++v) {
        cluster.push_back(vector<float>(category_num, 0));
        tempCluster.push_back(vector<float>(category_num, 0));
    }

    Grid subset(5000, 1, 1, varnum);
    for (int i = 0; i < subset.length; ++i) {
        int x = randomInt(0, grid->length-1);
        int y = randomInt(0, grid->width-1);
        int z = randomInt(0, grid->height-1);
        for (int v = 0; v < varnum; ++v) {
            subset(i, 0, 0, v) = (*grid)(x, y, z, v);
        }
    }

    for (int v = 0; v < varnum; ++v) {
        cluster[v][0] = subset(0, 0, 0, v);
    }

    Grid2D distance(subset.length, 1);

    for (int i = 1; i < category_num; ++i) {

        double sum = 0;
        for (int x = 0; x < subset.length; ++x) {
            float ClosestDistance = HUGE_VAL;
            for (int j = 0; j < i; ++j) {
                float dist = 0;
                for (int v = 0; v < varnum; ++v) {
                    float delta = subset(x, 0, 0, v) - cluster[v][j];
                    dist += delta*delta;
                }
                if (dist < ClosestDistance) ClosestDistance = dist;
            }
            distance(x, 0) = ClosestDistance;
            sum += ClosestDistance;
        }
        distance /= sum;

        for (int x = 1; x < subset.length; ++x) {
            distance(x, 0) += distance(x-1, 0);
        }

        int x;
        float choice = randomFloat(0, 1);
        for (x = 0; x < subset.length; ++x) {
            if (choice < distance(x, 0)) break;
        }
        for (int v = 0; v < varnum; ++v) {
            cluster[v][i] = subset(x, 0, 0, v);
        }
    }

    while(true) {

        for (int i = 0; i < category_num; ++i) {
            tempClusterMembers[i] = 0;
            for (int v = 0; v < varnum; ++v) {
                tempCluster[v][i] = 0;
            }
        }

        for (int z = 0; z < grid->height; ++z) {
            for (int y = 0; y < grid->width; ++y) {
                for (int x = 0; x < grid->length; ++x) {

                    int ClosestCluster = 0;
                    float ClosestDistance = HUGE_VAL;
                    for (int i = 0; i < category_num; ++i) {
                        float dist = 0;
                        for (int v = 0; v < varnum; ++v) {
                            float d = cluster[v][i] - (*grid)(x, y, z, v);
                            dist += d*d;
                        }
                        if (dist < ClosestDistance) {
                            ClosestCluster = i;
                            ClosestDistance = dist;
                        }
                    }

                    for (int v = 0; v < varnum; ++v) {
                        tempCluster[v][ClosestCluster] += (*grid)(x, y, z, v);
                    }
                    tempClusterMembers[ClosestCluster]++;
                }
            }
        }

        for (int i = 0; i < category_num; ++i) {
            if (tempClusterMembers[i] == 0) {
                int x = randomInt(0, grid->length-1);
                int y = randomInt(0, grid->width-1);
                int z = randomInt(0, grid->height-1);
                for (int v = 0; v < varnum; ++v) {
                    tempCluster[v][i] = (*grid)(x, y, z, v) + randomFloat(-0.1, 0.1);
                }
            } else {
                for (int v = 0; v < varnum; ++v) {
                    tempCluster[v][i] /= tempClusterMembers[i];
                }
            }
        }


        if (cluster == tempCluster) { break; }

        cluster.swap(tempCluster);
    }

    for (int z = 0; z < grid->height; ++z) {
        for (int y = 0; y < grid->width; ++y) {
            for (int x = 0; x < grid->length; ++x) {

                int ClosestCluster = 0;
                float ClosestDistance = HUGE_VAL;
                for (int i = 0; i < category_num; ++i) {
                    float dist = 0;
                    for (int v = 0; v < varnum; ++v) {
                        float delta = cluster[v][i] - (*grid)(x, y, z, v);
                        dist += delta*delta;
                    }
                    if (dist < ClosestDistance) {
                        ClosestCluster = i;
                        ClosestDistance = dist;
                    }
                }

                for (int v = 0; v < varnum; ++v) {
                    (*grid)(x, y, z, v) = cluster[v][ClosestCluster];
                }
            }
        }
    }


}
