#include "feep.h"

pixel::pixel(unsigned char is_white, unsigned char w, unsigned char r, unsigned char g, unsigned char b) : is_white(is_white), w(w), r(r), g(g), b(b) {}

const unsigned char& pixel::operator [](unsigned int i) const
{
    if(not(i >= 0 and i <= 2))
    {
        cerr << "fatal, there are only 3 channels per pixel" << endl;
        exit(1);
    }
    switch(i)
    {
        case 0:
            return r;
        case 1:
            return g;
        case 2:
            return b;
    }
}

unsigned char& pixel::operator [](unsigned int i)
{
    if(not(i >= 0 and i <= 2))
    {
        cerr << "fatal, there are only 3 channels per pixel" << endl;
        exit(1);
    }
    switch(i)
    {
        case 0:
            return r;
        case 1:
            return g;
        case 2:
            return b;
    }
}


const vector<pixel>& feep::operator [](unsigned int i) const
{
    if(not(i >= 0 and i < pixel_map.size()))
    {
        cerr << "fatal, try to access unexistent pixel row" << endl;
        exit(1);
    }
    return pixel_map[i];
}

vector<pixel>& feep::operator [](unsigned int i)
{
    if(not(i >= 0 and i < pixel_map.size()))
    {
        cerr << "fatal, try to access unexistent pixel row" << endl;
        exit(1);
    }
    return pixel_map[i];
}


feep::feep()
{
    image_name="";
    type=PBM;
    binary=false;
    w=0;
    h=0;
    max_intens=0;
    histogram = {};
}

feep::feep(string file_name, int debug_mode)
{
    feep();
    load(file_name,debug_mode);
    make_histogram();
}

string feep::type_to_str(feep_type t)
{
    if(t == PBM)
        return "PBM";
    if(t == PGM)
        return "PGM";
    return "PPM";
}

char feep::flush_white(ifstream& fs)
{
    char c = fs.get();
    while(isspace(c))
        c = fs.get();
    
    return c;
}

void feep::flush_comment(ifstream& fs, bool initial)
{
    if(binary and !initial)
        return;
    char c = flush_white(fs);
    while(c == '#')
    {
        while(c != '\n' && c != '\r')
            c = fs.get();
        c = flush_white(fs);
    }
    fs.putback(c);
}

int feep::get_int(ifstream& fs)
{
    string s = "";
    char c = flush_white(fs);
    do
    {
        s += c;
        c = fs.get();
        if(!fs.good())
            return -1;
    } while(!isspace(c));
    return unsigned(stoi(s));
}

void feep::load(string file_name, int debug_print)
{
    image_name = file_name;
    ifstream fs;
    fs.open(file_name, fstream::binary);
    
    if(!fs)
    {
        cerr << "unable to open " << file_name << endl;
        exit(1);
    }

    flush_comment(fs,true);
    char c = fs.get();

    if(!fs.good() || c != 'P')
    {
        cerr << "ill formated file (1), abort." << endl;
        exit(1);
    }

    int c_int = get_int(fs);
    
    if(!(c_int >= 1 && c_int <= 6))
    {
        cerr << "ill formated file (2), abort." << endl;
        exit(1);
    }

    type = feep_type((c_int-1)%3);
    binary = c_int > 3;

    flush_comment(fs,true);
    w = get_int(fs);
    h = get_int(fs);

    if(type != PBM)
    {
        flush_comment(fs,true);
        max_intens = get_int(fs);
        if(max_intens > 255)
        {
            cerr << "max intens > 255 not implemented, abort" << endl;
            exit(1);
        }
    }

    flush_comment(fs,true);
    
    if(type != PPM)
    {
        for(int iLig = 0 ; iLig < h ; iLig++)
        {
            pixel_map.push_back(vector<pixel>());
            flush_comment(fs);
            for(int iCol = 0 ; iCol < w ; iCol++)
            {
                pixel p;

                if(binary)
                {
                    c = fs.get();

                    if(type == PBM)
                        p.is_white = c;
                    if(type == PGM)
                        p.w = c;
                }
                else
                {
                    if(type == PBM)
                        p.is_white = get_int(fs);
                    if(type == PGM)
                        p.w = get_int(fs);
                }

                if(!fs.good())
                    break;
                pixel_map[pixel_map.size()-1].push_back(p);
            }

            if(!fs.good())
            {
                cerr << "ill formated file (3): shape doesn't match" << endl;
                break;
            }
        }
    }
    else
    {
        for(int iLig = 0 ; iLig < h ; iLig++)
        {
            pixel_map.push_back(vector<pixel>());
            for(int iCol = 0 ; iCol < w ; iCol++)
            {
                pixel p;
                pixel_map[pixel_map.size()-1].push_back(p);
            }
        }

        for(int iLig = 0 ; iLig < h ; iLig++)
        {
            flush_comment(fs);
            for(int iCol = 0 ; iCol < w ; iCol++)
            {
                for(int chan = 0 ; chan < 3 ; chan++)
                {
                    unsigned char val;
                    if(binary)
                        val = fs.get();
                    else
                        val = get_int(fs);
                    
                    if(!fs.good())
                        break;

                    pixel_map[iLig][iCol][chan] = val;
                }
            }

            if(!fs.good())
            {
                cerr << "ill formated file (3): shape doesn't match" << endl;
                break;
            }
        }
    }

    if(debug_print == 2)
        pretty_print();
    
    if(debug_print)
        print_infos();

    fs.close();
}

void feep::save(string where)
{
    image_name = where;
    ofstream fs;
    fs.open(where, fstream::binary | fstream::out);
    fs << 'P';
    vector<char> type_s = {'1','2','3','4','5','6'};
    fs << type_s[type+3*binary] << endl; 
    fs << w << " " << h << endl;
    if(type == PGM || type == PPM)
        fs << max_intens << endl;

    if(type != PPM)
    {
        for(int iLig = 0 ; iLig < h ; iLig++)
        {
            for(int iCol = 0 ; iCol < w ; iCol++)
            {
                if(binary)
                {
                    if(type == PBM)
                        fs << pixel_map[iLig][iCol].is_white;
                    if(type == PGM)
                        fs << pixel_map[iLig][iCol].w;
                }
                else
                {
                    if(type == PBM)
                        fs << (int)(pixel_map[iLig][iCol].is_white) << " ";
                    if(type == PGM)
                        fs << (int)(pixel_map[iLig][iCol].w) << " ";   
                }
            }
            if(not binary)
                fs << endl;
        }
    }
    else
    {
        for(int iLig = 0 ; iLig < h ; iLig++)
        {
            for(int iCol = 0 ; iCol < w ; iCol++)
            {
                for(int chan = 0 ; chan < 3 ; chan++)
                {
                    if(binary)
                        fs << pixel_map[iLig][iCol][chan];
                    else
                        fs << (int)pixel_map[iLig][iCol][chan] << " ";
                }
            }
            if(not binary)
                fs << endl;
        }
    }

    fs.close();
}

void feep::print_infos()
{
    cout << "filename: " << image_name << endl;
    cout << "type: " << type_to_str(type) << "; binary? " << binary << endl;
    cout << "size: " << w << "x" << h << endl;
    if(type != PBM)
        cout << "max intensity: " << max_intens << endl;
}

void feep::pretty_print()
{
    vector<string> chan = {"R", "G", "B"};
    if(type == PPM)
    {
        for(int c = 0 ; c < 3 ; c += 1)
        {
            cout << "channel: " << chan[c] << endl;
            for(auto lig: pixel_map)
            {
                for(auto p: lig)
                    cout << (int) p[c] << " ";
                cout << endl;
            }
        }
        return;
    }


    for(auto lig: pixel_map)
    {
        for(auto p: lig)
        {
            if(type == PBM)
                cout << (int)(p.is_white) << " ";
            if(type == PGM)
            {
                cout << int(p.w) << " ";
            }
        }
        cout << endl;
    }
}

void feep::convert_to_pbm(feep& copy, conversion_method how)
{
    if(type == PPM)
    {
        cerr << "no direct conversions PPM -> PBM, use an intermediate PGM" << endl;
        return;
    }
    

    if(how == MAX)
    {
        for(int iLig = 0 ; iLig < copy.h ; iLig++)
            for(int iCol = 0; iCol < copy.w ; iCol++)
                copy[iLig][iCol].is_white = (copy[iLig][iCol].w > copy.max_intens / 2);
        
        copy.type = PBM;
        return;
    }

    if(how == MEAN)
    {
        float mean = 0.0;
        for(int iLig = 0 ; iLig < copy.h ; iLig++)
            for(int iCol = 0; iCol < copy.w ; iCol++)
                mean += copy[iLig][iCol].w;
                
        mean /= copy.h*copy.w;
        for(int iLig = 0 ; iLig < copy.h ; iLig++)
            for(int iCol = 0; iCol < copy.w ; iCol++)
                copy[iLig][iCol].is_white = (copy[iLig][iCol].w > mean);
        
        copy.type = PBM;
        return;
    }

    if(how == MED)
    {
        vector<unsigned char> vals;
        for(int iLig = 0 ; iLig < copy.h ; iLig++)
            for(int iCol = 0; iCol < copy.w ; iCol++)
                vals.push_back(copy[iLig][iCol].w);
        
        sort(vals.begin(),vals.end());

        float med = 0.0;
        auto s = vals.size();
        if(s%2 == 0)
            med = (vals[s/2]+vals[s/2-1])/2;
        else
            med = vals[s/2];

        for(int iLig = 0 ; iLig < copy.h ; iLig++)
            for(int iCol = 0; iCol < copy.w ; iCol++)
                copy[iLig][iCol].is_white = (copy[iLig][iCol].w > med);
        
        copy.type = PBM;
        return;
    }


    cerr << "nothing was done" << endl;
    return;
}

void feep::convert_to_pgm(feep& copy, conversion_method how)
{

    if(type == PBM)
    {
        copy.max_intens = 1;
        for(int iLig = 0 ; iLig < copy.h ; iLig++)
            for(int iCol = 0 ; iCol < copy.w ; iCol++)
                copy[iLig][iCol].w = copy[iLig][iCol].is_white;
        copy.type = PGM;
        return;
    }
    
    if(type == PPM)
    {
        if(how == MAX)
        {
            for(int iLig = 0 ; iLig < copy.h ; iLig++)
                for(int iCol = 0 ; iCol < copy.w ; iCol++)
                    copy[iLig][iCol].w = max(copy[iLig][iCol][0],max(copy[iLig][iCol][1],copy[iLig][iCol][2]));
            copy.type = PGM;
            return;
        }
        if(how == MEAN)
        {
            for(int iLig = 0 ; iLig < copy.h ; iLig++)
                for(int iCol = 0 ; iCol < copy.w ; iCol++)
                    copy[iLig][iCol].w = (copy[iLig][iCol][0]+copy[iLig][iCol][1]+copy[iLig][iCol][2])/3;
            copy.type = PGM;
            return;   
        }
        if(how == MED)
        {
            for(int iLig = 0 ; iLig < copy.h ; iLig++)
                for(int iCol = 0 ; iCol < copy.w ; iCol++)
                {
                    vector<unsigned char> v = {copy[iLig][iCol][0],copy[iLig][iCol][1],copy[iLig][iCol][2]};
                    sort(v.begin(),v.end());
                    auto med = v[1];
                    copy[iLig][iCol].w = med;
                }
            copy.type = PGM;
            return;   
        }
    }

    cerr << "nothing was done" << endl;
    return;
}

void feep::convert_to_ppm(feep& copy, conversion_method how, vector<array<int,3>> color_map)
{
    if(how == DEFAULT_COLOR_MAP)
    {
        //TODO
        cerr << "no default colormap implemented yet" << endl;
    }

    if(how != CUSTOM_COLOR_MAP)
    {
        cerr << "nothing was done" << endl;
        return;
    }

    if(color_map.size() != 256)
    {
        cerr << "wrong format for custom colormap" << endl;
        return;
    }

    for(int iLig = 0 ; iLig < copy.h ; iLig++)
        for(int iCol = 0 ; iCol < copy.w ; iCol++)
        {
            int w = copy[iLig][iCol].w;
            for(int chan = 0 ; chan < 3 ; chan++)
                copy[iLig][iCol][chan] = color_map[w][chan];
        }
    copy.type = PGM;
}

feep feep::convert_to(feep_type to, bool to_binary, conversion_method how, vector<array<int,3>> color_map)
{
    feep copy = feep(*this);
    copy.binary = to_binary;

    if(to == type)
        return copy;

    if(to == PBM)
        convert_to_pbm(copy, how);

    if(to == PGM)
        convert_to_pgm(copy, how);

    if(to == PPM)
        convert_to_ppm(copy, how, color_map);

    return copy;
}

//Normalized histo
void feep::make_histogram(bool normalized)
{
    if(type != PGM)
    {
        cerr << "histogram only defined for PGM at the moment" << endl;
        return;
    }

    for(int i = 0 ; i < 256 ; i++)
        histogram[i] = 0;
    for(int iLig = 0 ; iLig < h ; iLig++)
        for(int iCol = 0 ; iCol < w ; iCol++)
            histogram[pixel_map[iLig][iCol].w] += 1;

    if(normalized)
        for(int i = 0 ; i < 256 ; i++)
            histogram[i] /= (h*w);
}

void feep::save_histogram(string histo_name)
{
    FILE* histo = fopen(histo_name.c_str(),"w");
    fprintf(histo,"Int,NbPix\n");
    for(int i=0 ; i < 256 ; i++)
        fprintf(histo,"%d,%f\n",i,histogram[i]);
    fclose(histo);
}

feep feep::apply_transformation(function<int(int)> phi, bool copy_p)
{
    if(type != PGM)
    {
        cerr << "transformations implem for PGM only" << endl;
        return *this;
    }
    feep* copy;
    if(copy_p)
        copy = new feep(*this);
    else
        copy = this;

    for(int iLig = 0 ; iLig < h ; iLig++)
        for(int iCol = 0 ; iCol < w ; iCol++)
            (*copy)[iLig][iCol].w = phi((*copy)[iLig][iCol].w);
    copy->make_histogram();
    return *copy;
}

int feep::equalize_i(int i)
{
    float s1 = 0, s2 = 0;
    for(int j = 0 ; j <= i ; j++)
        s1 += histogram[j];
    for(int k = 0 ; k <= max_intens ; k++)
        s2 += histogram[k];

    return max_intens*s1/s2;
}

feep feep::equalize(bool copy_p)
{
    if(type != PGM)
    {
        cerr << "transformations implem for PGM only" << endl;
        return *this;
    }
    feep* copy;
    if(copy_p)
        copy = new feep(*this);
    else
        copy = this;

    for(int iLig = 0 ; iLig < h ; iLig++)
        for(int iCol = 0 ; iCol < w ; iCol++)
            (*copy)[iLig][iCol].w = equalize_i((*copy)[iLig][iCol].w);
    copy->make_histogram();
    return *copy;
}

int feep::apply_filter_side(int candidate, int bound, filtering_method how)
{
    int to_return = candidate;
    if(to_return >= 0 && to_return < bound)
        return to_return;

    switch(how)
    {
        case FILT_ZERO_PAD:
            return -1;
        break;

        case FILT_MODULO:
            if(to_return > 0)
                return to_return % bound;
            else
                return bound - (abs(to_return)%bound);
        break;

        default:
            cerr << "side effect filtering method not implem" << endl;
            return -1;
        break;
    }
}

int feep::apply_filter_on(int iLig, int iCol, const vector<vector<float>>& filter, filtering_method how)
{
    float new_w = 0.0;
    int h_filter = filter.size();
    int w_filter = filter[0].size();

    for(int i = 0 ; i < filter.size() ; i++)
        for(int j = 0 ; j < filter[i].size() ; j++)
        {
            int i_image = apply_filter_side(iLig + i - h_filter/2,h,how);
            int j_image = apply_filter_side(iCol + j - w_filter/2,w,how);

            if(i_image < 0 || j_image < 0)
                continue;
            new_w += filter[i][j]*pixel_map[i_image][j_image].w;
        }
    
    return (int)new_w;
}

feep feep::apply_filter(const vector<vector<float>>& filter, filtering_method how)
{
    if(type != PGM)
    {
        cerr << "filtering on PGM only" << endl;
        return *this;
    }

    feep copy = feep(*this);


    for(int iLig = 0 ; iLig < h ; iLig++)
        for(int iCol = 0 ; iCol < w ; iCol++)
        {
            copy[iLig][iCol].w = apply_filter_on(iLig,iCol,filter,how);
        }
        
    
    return copy;
}


bool feep::is_valid(int iLig, int iCol)
{
    return (iLig >= 0 && iLig < h) && (iCol >= 0 && iCol < w);
}

int feep::find_cc(bool one_adj)
{
    adj[0] = {{0,1},{0,-1},{1,0},{-1,0},{1,1},{-1,-1},{1,-1},{-1,1}};
    adj[1] = {{0,1},{1,0},{-1,0},{0,-1}};
    //Init to unseen
    for(int iLig = 0 ; iLig < h ; iLig++)
    {
        cc[one_adj].push_back(vector<int>());
        for(int iCol = 0 ; iCol < w ; iCol++)
            cc[one_adj][iLig].push_back(-1);
    }

    int current_cc = 0;

    for(int iLig = 0 ; iLig < h ; iLig++)
        for(int iCol = 0 ; iCol < w ; iCol++)
        {
            if(cc[one_adj][iLig][iCol] == -1)
            {

                queue<pair<int,int>> to_visit;
                to_visit.push(make_pair(iLig,iCol));
                while(!to_visit.empty())
                {
                    auto curr = to_visit.front();
                    to_visit.pop();

                    if(cc[one_adj][curr.first][curr.second] != -1)
                        continue;

                    cc[one_adj][curr.first][curr.second] = current_cc;

                    for(int iDir = 0 ; iDir < adj[one_adj].size() ; iDir++)
                    {
                        int newLig = curr.first+adj[one_adj][iDir][0];
                        int newCol = curr.second+adj[one_adj][iDir][1];

                        if(is_valid(newLig,newCol) && pixel_map[newLig][newCol].w == pixel_map[iLig][iCol].w)
                            to_visit.push(make_pair(newLig,newCol));
                        
                    }

                }

                current_cc += 1;
            }
        }
    return current_cc;
}

feep feep::cc_to_feep(feep_type to, bool one_adj, int nb_cc)
{
    feep copy = feep(*this);

    if(to == PGM)
    {
        for(int iLig = 0 ; iLig < h ; iLig++)
            for(int iCol = 0 ; iCol < w ; iCol++)
            {
                copy[iLig][iCol].w = (int)((float(cc[one_adj][iLig][iCol])/nb_cc)*255);
                copy.max_intens = 255;
            }
    }

    if(to == PPM)
    {
        vector<vector<int>> primes = {{2,3},{5,7},{11,13}};
        vector<function<float (float)>> f_chan;
        for(auto&& p: primes)
            f_chan.push_back([p](float x) { return 255*sin(p[0]+p[1]*x); });
        for(int iLig = 0 ; iLig < h ; iLig++)
            for(int iCol = 0 ; iCol < w ; iCol ++)
                for(int chan = 0 ; chan < 3 ; chan++)
                    copy[iLig][iCol][chan] = (int)f_chan[chan](cc[one_adj][iLig][iCol]);
        copy.max_intens = 255;
        copy.type = PPM;

    }

    return copy;
}