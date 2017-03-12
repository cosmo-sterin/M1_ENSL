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

int main(int argc, char** argv)
{
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
}