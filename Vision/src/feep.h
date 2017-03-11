#ifndef DEF_FEEP_H
#define DEF_FEEP_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream> 
#include <bitset>

#include <algorithm>

using namespace std;

enum feep_type
{
    PBM = 0,
    PGM,
    PPM
};

enum conversion_method
{
    NONE = 0,
    PGM_PBM_MAX, // >= max_intens/2
    PGM_PBM_MEAN, // >= mean_itens
    PGM_PBM_MED, // >= med_intens
};

struct pixel
{
    unsigned char is_white;//PBM
    unsigned char w;//PGM, white intensity
    unsigned char r,g,b;//PPM

    pixel(unsigned char is_white=0, unsigned char w=0, unsigned char r=0, unsigned char g=0, unsigned char b=0);

    const unsigned char& operator [](unsigned int i) const;

    unsigned char& operator [](unsigned int i);
};

class feep
{
public:
    feep();
    feep(string file_name, int debug_mode=0);

    string type_to_str(feep_type t);

    char flush_white(ifstream& fs);

    void flush_comment(ifstream& fs, bool initial=false);

    int get_int(ifstream& fs);

    void load(string file_name, int debug_print=0);
    void save(string where);

    void print_infos();

    void pretty_print();

    const vector<pixel>& operator [](unsigned int i) const;

    vector<pixel>& operator [](unsigned int i);

    feep to_pbm(bool to_binary, conversion_method how=NONE);

private:
    string image_name;
    feep_type type;
    bool binary;
    int w,h;
    int max_intens;

    vector<vector<pixel>> pixel_map;
};

#endif