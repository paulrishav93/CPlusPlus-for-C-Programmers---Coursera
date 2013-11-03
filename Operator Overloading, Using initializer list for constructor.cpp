#include<iostream>
#include <vector>
#include <algorithm>
using namespace std;

class point
{
    const double x,y;

public:
    double getx(){ return x;}
    double gety(){ return y;}

    point(const int x=0.0, const int y=0.0):x(x),y(y){}
};

ostream& operator << ( ostream &out, point &p)
{
    out<<"("<<p.getx()<<","<<p.gety()<<")";
}


int main()
{
    point p(3,5);

    cout<<p;
}
