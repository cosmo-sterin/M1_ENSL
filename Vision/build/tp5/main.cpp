#include <iostream>
#include <DGtal/base/Common.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/helpers/StdDefs.h>
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i;

Color black(0, 0, 0);
Color white(255, 255, 255);
Color red(255,0,0);
Color green(0,255,0);

int x_0, y_0;

bool in_disc(int x, int y)
{
    return x*x + y*y <= x_0*x_0 + y_0*y_0;
}


vector<vector<int>> dir[2] = {{{0,1},{1,0},{0,-1},{-1,0},{1,1},{-1,-1},{1,-1},{-1,1}},{{0,1},{1,0},{0,-1},{-1,0}}};

bool in_border(const DigitalSet& disc, int x, int y, bool un_adj)
{
    for(auto the_dir: dir[un_adj])
    {
        int new_x = x + the_dir[0];
        int new_y = y + the_dir[1];

        if(not disc(Point(new_x,new_y)))
            return true;
    }
    return false;
}


int main(int argc, char** argv)
{
    int dim = 30;

    x_0 = atoi(argv[1]);
    y_0 = atoi(argv[2]);


    trace.beginBlock("Line");
    

    Point O(-dim,-dim), P(dim-1,dim-1), M(0,0);

    Domain domain(O, P);
    Board2D board;
    board << domain << M;
    
    DigitalSet the_disc(domain);
    
    for(Domain::ConstIterator it = domain.begin(); it != domain.end(); ++it)
    {
        Point p = *it; 
        if (in_disc(p[0],p[1]))
            the_disc.insert(p);
    }

    board << CustomStyle(the_disc.className(), new CustomColors(black, black)) << the_disc;

    DigitalSet border0(domain),border1(domain);

    for(Domain::ConstIterator it = domain.begin(); it != domain.end(); ++it)
    {
        Point p = *it; 
        if (not the_disc(p))
            continue;
        if (in_border(the_disc,p[0],p[1],false))
            border0.insert(p);
        if (in_border(the_disc,p[0],p[1],true))
            border1.insert(p);
    }

    board << CustomStyle(border0.className(), new CustomColors(red, red)) << border0;
    board << CustomStyle(border1.className(), new CustomColors(green, green)) << border1;
    board.saveSVG("disck.svg");
    return 0;
}

