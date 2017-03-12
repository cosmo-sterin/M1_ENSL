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
    feep len_dark = feep("../../data/tp2/len_dark.pgm",true);
    len_dark.make_histogram();
    len_dark.save_histogram("results/histo_lendark.csv");

}