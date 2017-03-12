#include <iostream>
#include <string>
#include <vector>
#include <fstream> 
#include <bitset>
#include <cmath>

#include <algorithm>

#include "../../src/feep.h"

using namespace std;


int inversion(int i)
{
    return 255-i;
}

int gamma_correction(int i, float gamma)
{
    return int(pow(i,1/gamma));
}

int naive_threshold(int i, int t, int M)
{
    return (i >= t) ? M : 0;
}

int main(int argc, char** argv)
{
    //Image transformation

    using namespace std::placeholders;
    feep len_dark = feep("../../data/tp2/len_dark.pgm",true);
    len_dark.save_histogram("results/histo_lendark.csv");

    feep len_white = len_dark.apply_transformation(inversion,true);
    len_white.save_histogram("results/histo_lenwhite.csv");
    len_white.save("results/len_white.pgm");

    auto gamma22 = bind(gamma_correction, _1, 2.2);
    feep len_gamma22 = len_dark.apply_transformation(gamma22,true);
    len_gamma22.save_histogram("results/histo_leng22.csv");
    len_gamma22.save("results/len_g22.pgm");

    auto gamma1o22 = bind(gamma_correction, _1, 1/(2.2));
    feep len_gamma1o22 = len_dark.apply_transformation(gamma1o22,true);
    len_gamma1o22.save_histogram("results/histo_leng1o22.csv");
    len_gamma1o22.save("results/len_g1o22.pgm");

    feep len_equa = len_dark.equalize(true);
    len_equa.save_histogram("results/histo_lenequa.csv");
    len_equa.save("results/len_equa.pgm");

    //Image Segmentation
    feep objects = feep("../../data/tp2/objects-dark.pgm",true);
    auto naive_100 = bind(naive_threshold, _1, 100, objects.max_intens);
    feep objects_100 = objects.apply_transformation(naive_100,true);
    objects_100.save("results/obj_100threshold.pgm");

    auto naive_50 = bind(naive_threshold, _1, 50, objects.max_intens);
    feep objects_50 = objects.apply_transformation(naive_50,true);
    objects_50.save("results/obj_50threshold.pgm");

}