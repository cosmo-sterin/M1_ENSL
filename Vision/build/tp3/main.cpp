#include <iostream>
#include <string>
#include <vector>
#include <fstream> 
#include <bitset>
#include <cmath>

#include <algorithm>

#include "../../src/feep.h"

using namespace std;

vector<vector<float>> make_filter(vector<float> coeff, int h, int w, float mult=1.0)
{
    if(coeff.size() != h*w)
    {
        cerr << "sizes dont match" << endl;
        return vector<vector<float>>();
    }
    vector<vector<float>> filter;
    int k = 0;
    for(int i = 0 ; i < h ; i++)
    {
        filter.push_back(vector<float>());
        for(int j = 0 ; j < w ; j++)
        {
            filter[i].push_back(coeff[k]*mult);
            k+=1;
        }
    }
    return filter;
}

int main(int argc, char** argv)
{
    feep len_dark = feep("../../data/tp2/len_dark.pgm",true);
    auto filter = make_filter({1,2,1,2,4,2,1,2,1},3,3,1/16.0);
    feep filtered_len_zp = len_dark.apply_filter(filter);
    filtered_len_zp.convert_to(PGM,false).save("results/filtered_len_zp.pgm");//filling sides with 0

    feep filtered_len_modulo = len_dark.apply_filter(filter, FILT_MODULO);
    filtered_len_modulo.convert_to(PGM,false).save("results/filtered_len_modulo.pgm");//tore it

    printf("%d %d %d\n", len_dark[0][0].w, filtered_len_modulo[0][0].w, filtered_len_zp[0][0].w);
}