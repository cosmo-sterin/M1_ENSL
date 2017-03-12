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

int threshold(int i, int t, int M)
{
    return (i >= t) ? M : 0;
}

using namespace std::placeholders;

void make_transformations()
{
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

float n_B(int t, const histo& h)
{
    float s = 0;
    for(int i = 0 ; i < t ; i ++)
        s += h[i];
    return s;
}

float n_O(int t, const histo& h, int M)
{
    float s = 0;
    for(int i = t ; i <= M ; i++)
        s += h[i];
    return s;
}

float mu_B(int t, const histo& h)
{
    float nb = 1/n_B(t,h);
    float s = 0;
    for(int i = 0 ; i < t ; i++)
        s += h[i]*i;
    return s / nb;
}

float mu_O(int t, const histo& h, int M)
{
    float no = 1/n_O(t,h,M);
    float s = 0;
    for(int i = t ; i <= M ; i++)
        s += h[i]*i;
    return s / no;
}

float sigmasq_B(int t, const histo& h)
{
    float nnb = n_B(t,h);
    float nb = 1/nnb;
    float s = 0;
    for(int i = 0 ; i < t ; i++)
        s += h[i]*(i-nnb)*(i-nnb);
    return s / nb;
}

float sigmasq_O(int t, const histo& h, int M)
{
    float nno = n_O(t,h,M);
    float no = 1/nno;
    float s = 0;
    for(int i = t ; i <= M ; i++)
        s += h[i]*(i-nno)*(i-nno);
    
    return s / no;
}

int optimal_fisher(const histo& h, int M)
{
    int t_opt = 0;
    float res = 1000*1000*1000;
    for(int t = 0 ; t < 256 ; t++)
    {
        float the_res = n_B(t,h)*sigmasq_B(t,h) + n_O(t,h,M)*sigmasq_O(t,h,M);
        //printf("%f\n",the_res);
        if(the_res < res)
        {
            res = the_res;
            t_opt = t;
        }
    }
    return t_opt;
}

int main(int argc, char** argv)
{
    //Image transformation
    make_transformations();

    //Image Segmentation
    feep objects = feep("../../data/tp2/objects-dark.pgm",true);
    auto naive_100 = bind(threshold, _1, 100, objects.max_intens);
    feep objects_100 = objects.apply_transformation(naive_100,true);
    objects_100.save("results/obj_100threshold.pgm");

    auto naive_50 = bind(threshold, _1, 50, objects.max_intens);
    feep objects_50 = objects.apply_transformation(naive_50,true);
    objects_50.save("results/obj_50threshold.pgm");

    int t = optimal_fisher(objects.histogram, objects.max_intens);
    printf("Fisher's method gives t=%d\n", t);
    auto opt_t = bind(threshold, _1, t, objects.max_intens);
    feep objects_opt = objects.apply_transformation(opt_t,true);
    objects_opt.save("results/obj_optthreshold.pgm");

}