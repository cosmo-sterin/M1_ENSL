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

    feep_P1.convert_to(PBM,true).save("results/feep_P4.pbm");
    feep_P1.convert_to(PGM,false).save("results/feep_P2.pgm");
    feep_P1.convert_to(PGM,true).save("results/feep_P5.pgm");

    antifeep.convert_to(PGM,true).save("results/antifeep_P5.pgm");

    len_dark.convert_to(PBM,false,MAX).save("results/len_dark_P1_max.pbm");
    len_dark.convert_to(PBM,false,MEAN).save("results/len_dark_P1_mean.pbm");
    len_dark.convert_to(PBM,false,MED).save("results/len_dark_P1_med.pbm");

    clown.convert_to(PBM,false,MAX).save("results/clown_P1_max.pbm");
    clown.convert_to(PBM,false,MEAN).save("results/clown_P1_mean.pbm");
    clown.convert_to(PBM,false,MED).save("results/clown_P1_med.pbm");

    feep clown_color = feep("../../data/tp1/images/10_extra/clown_P6.ppm",true);
    clown_color.convert_to(PPM,false).save("results/clown_P3.ppm");

    clown_color.convert_to(PGM,false,MAX).save("results/clown_P2_max.pgm");
    clown_color.convert_to(PGM,false,MEAN).save("results/clown_P2_mean.pgm");
    clown_color.convert_to(PGM,false,MED).save("results/clown_P2_med.pgm");
}