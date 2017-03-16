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
    auto binom_filter = make_filter({1,2,1,2,4,2,1,2,1},3,3,1/16.0);
    feep filtered_len_zp = len_dark.apply_filter(binom_filter);
    filtered_len_zp.convert_to(PGM,false).save("results/filtered_len_binom_zp.pgm");//filling sides with 0

    feep filtered_len_modulo = len_dark.apply_filter(binom_filter, FILT_MODULO);
    filtered_len_modulo.convert_to(PGM,false).save("results/filtered_len_binom_modulo.pgm");//tore it

    auto sobel_x = make_filter({-1,0,1,-2,0,2,-1,0,1},3,3,1/4.0);
    auto sobel_y = make_filter({-1,-2,-1,0,0,0,1,2,1},3,3,1/4.0);

    feep filtered_len_sobelx = len_dark.apply_filter(sobel_x, FILT_MODULO);
    filtered_len_sobelx.convert_to(PGM,false).save("results/filtered_len_sobelx.pgm");

    feep filtered_len_sobely = len_dark.apply_filter(sobel_y, FILT_MODULO);
    filtered_len_sobely.convert_to(PGM,false).save("results/filtered_len_sobely.pgm");

    feep len_sobel_norm_map = feep(filtered_len_sobelx);
    
    for(int iLig = 0 ; iLig < len_sobel_norm_map.h ; iLig++)
        for(int iCol = 0 ; iCol < len_sobel_norm_map.w ; iCol++)
            len_sobel_norm_map[iLig][iCol].w = (int)sqrt(pow(filtered_len_sobelx[iLig][iCol].w,2)+pow(filtered_len_sobely[iLig][iCol].w,2));
           

    len_sobel_norm_map.convert_to(PGM,false).save("results/len_sobel_norm_map.pgm");

    feep len_sobel_angle_map = feep(filtered_len_sobelx);
    vector<vector<float>> angle_map;
    float M = 0;

    for(int iLig = 0 ; iLig < len_sobel_norm_map.h ; iLig++)
    {
        angle_map.push_back(vector<float>());
        for(int iCol = 0 ; iCol < len_sobel_norm_map.w ; iCol++)
        {
            if(filtered_len_sobelx[iLig][iCol].w == 0)
                angle_map[iLig].push_back(255);
            else
            {
                angle_map[iLig].push_back(atan(filtered_len_sobely[iLig][iCol].w/filtered_len_sobelx[iLig][iCol].w));
                M = max(M,angle_map[iLig][iCol]);
            }
        }
    }

    for(int iLig = 0 ; iLig < len_sobel_norm_map.h ; iLig++)
        for(int iCol = 0 ; iCol < len_sobel_norm_map.w ; iCol++)
        {
            if(angle_map[iLig][iCol] != 255.0)
            {
                angle_map[iLig][iCol] /= M;
                angle_map[iLig][iCol] *= 255;
            }
            len_sobel_angle_map[iLig][iCol].w = (int)angle_map[iLig][iCol];
        }



    len_sobel_angle_map.convert_to(PGM,false).save("results/len_sobel_angle_map.pgm");
}