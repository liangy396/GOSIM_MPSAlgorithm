#include "GOSIM.h"

Grid *GOSIM::HistCalc(Grid *grid, int buckets, float minVal, float maxVal) {

    float BucketWidth = buckets / (maxVal - minVal);

    int length = grid->length;
    int width = grid->width;
    int height = grid->height;

    vector<long> count(buckets, 0);
    for (int z = 0; z < height; ++z) {
        for (int y = 0; y < width; ++y) {
            for (int x = 0; x < length; ++x) {

                float value = (*grid)(x, y, z, 0);
                int bucket;

                bucket = (int)((value - minVal) * BucketWidth);
                if (bucket >= buckets) { bucket = buckets-1; }
                if (bucket < 0) { bucket = 0; }

                count[bucket]++;

            }
        }
    }

    double scale = 1.0 / (length * width * height);

    Grid *histogram = new Grid(buckets, 2, 1, 1);

    for (int i = 0; i < buckets; ++i) {

        (*histogram)(i, 0, 0, 0) = count[i] * scale;
        (*histogram)(i, 1, 0, 0) = count[i];
    }

    return histogram;
}
