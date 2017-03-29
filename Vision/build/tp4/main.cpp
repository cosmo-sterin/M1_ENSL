#include <iostream>
#include <string>
#include <vector>
#include <fstream> 
#include <bitset>
#include <cmath>

#include <algorithm>

#include "../../src/feep.h"

using namespace std;

int main(int argc, char** argv)
{
    feep hop = feep("hop.pgm",true).convert_to(PPM,true).convert_to(PGM,true);
    hop.save("results/hop.pgm");
    int nb_cc_0 = hop.find_cc(false);
    printf("There are %d connected components w.r.t 0-adj\n", nb_cc_0);
    int nb_cc_1 = hop.find_cc(true);
    printf("There are %d connected components w.r.t 1-adj\n", nb_cc_1);

    feep cc_0 = hop.cc_to_feep(PGM,false,nb_cc_0);
    cc_0.save("results/cc_0.pgm");
    feep cc_1 = hop.cc_to_feep(PGM,false,nb_cc_1);
    cc_1.save("results/cc_1.pgm");


    feep cc_0_rgb = hop.cc_to_feep(PPM,false,nb_cc_0);
    cc_0_rgb.save("results/cc_0.ppm");
    feep cc_1_rgb = hop.cc_to_feep(PPM,false,nb_cc_1);
    cc_1_rgb.save("results/cc_1.ppm");

    //hop_zeroAdj.conve

}