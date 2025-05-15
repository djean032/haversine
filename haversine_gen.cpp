#include <cfloat>
#include <cstdio>
#define _CRT_SECURE_NO_WARNINGS

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "haversine_calc.cpp"

typedef uint64_t u64;
#define u64max UINT64_MAX;
typedef double f64;

f64 rand_center(f64 min, f64 max)
{
    f64 div = (f64)rand() / (f64)RAND_MAX;
    f64 center = (1.0 - div) * min + div * max;
    return center;
}

f64 rand_offset(f64 center, f64 radius, f64 max_allowed)
{

    f64 min = center - radius;
    if (min < -max_allowed)
    {
        min = -max_allowed;
    }

    f64 max = center + radius;
    if (max > max_allowed)
    {
        max = max_allowed;
    }

    f64 result = rand_center(min, max);
    return result;
}

int main(int argc, char **argv)
{
    u64 num_pts = atoll(argv[1]);
    u64 max_pts = (1ULL << 34);
    if (num_pts > max_pts) {
        num_pts = max_pts;
    }

    srand(time(NULL));
    f64 max_x = 180;
    f64 max_y = 90;
    u64 max_cluster_count = num_pts / 64 + 1;
    u64 cluster_pt_left = max_cluster_count;
    f64 x_center = 0;
    f64 y_center = 0;
    f64 x_radius = max_x;
    f64 y_radius = max_y;
    f64 earth_radius = 6372.8;
    f64 sum = 0;
    f64 sum_coefficient = 1.0 / (f64)num_pts;
    bool json_start = true;

    FILE *json_out;
    json_out = fopen("hav_output.json", "w");
    if (json_out == NULL)
    {
        printf("Error! Could not open file.\n");
    }
    fputs("{\n", json_out);
    fputs("\"pairs\":[", json_out);

    for (u64 i = 0; i < num_pts; i++)
    {
        if (cluster_pt_left-- == 0)
        {
            cluster_pt_left = max_cluster_count;
            x_center = rand_center(-max_x, max_x);
            y_center = rand_center(-max_y, max_y);
            x_radius = rand_center(0, max_x);
            y_radius = rand_center(0, max_y);
        }

        f64 x0 = rand_offset(x_center, x_radius, max_x);
        f64 y0 = rand_offset(y_center, y_radius, max_y);
        f64 x1 = rand_offset(x_center, x_radius, max_x);
        f64 y1 = rand_offset(y_center, y_radius, max_y);

        f64 hav_dist = RefHaversine(x0, y0, x1, y1, earth_radius);
        sum += hav_dist * sum_coefficient;
        if (i == num_pts - 1)
        {
            fprintf(json_out, "{\"x0\":%.*f,", DBL_DIG, x0);
            fprintf(json_out, "\"y0\":%.*f,", DBL_DIG, y0);
            fprintf(json_out, "\"x1\":%.*f,", DBL_DIG, x1);
            fprintf(json_out, "\"y1\":%.*f}\n", DBL_DIG, y1);
        }
        else
        {
            fprintf(json_out, "{\"x0\":%.*f,", DBL_DIG, x0);
            fprintf(json_out, "\"y0\":%.*f,", DBL_DIG, y0);
            fprintf(json_out, "\"x1\":%.*f,", DBL_DIG, x1);
            fprintf(json_out, "\"y1\":%.*f},\n", DBL_DIG, y1);
        }
    }
    fputs("]}", json_out);
    fclose(json_out);
    printf("%f\n", sum);

    return 0;
}
