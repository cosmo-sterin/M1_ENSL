#include <iostream>
#include <string>
#include <vector>
#include <fstream> 
#include <bitset>

#include <algorithm>

#include "../../src/feep.h"

using namespace std;


int main(int argc, char** argv)
{
    feep feep_P1 = feep("../../data/tp1/images/00_small/feep_P1.pbm",true);

    feep antifeep = feep("../../data/tp1/images/00_small/antifeep_P2.pgm",true);
    feep bigfeep = feep("../../data/tp1/images/10_extra/Bigfeep_P5.pgm",true);
    feep clown = feep("../../data/tp1/images/10_extra/clown_P5.pgm",true);
    feep len_dark = feep("../../data/tp2/len_dark.pgm",true);
    feep obj_dark = feep("../../data/tp2/objects-dark.pgm",true);

    feep_P1.to_pbm(true).save("results/feep_P4.pbm");

    len_dark.to_pbm(false,PGM_PBM_MAX).save("results/len_dark_P1_max.pbm");
    len_dark.to_pbm(false,PGM_PBM_MEAN).save("results/len_dark_P1_mean.pbm");
    len_dark.to_pbm(false,PGM_PBM_MED).save("results/len_dark_P1_med.pbm");

    clown.to_pbm(false,PGM_PBM_MAX).save("results/clown_P1_max.pbm");
    clown.to_pbm(false,PGM_PBM_MEAN).save("results/clown_P1_mean.pbm");
    clown.to_pbm(false,PGM_PBM_MED).save("results/clown_P1_med.pbm");
}