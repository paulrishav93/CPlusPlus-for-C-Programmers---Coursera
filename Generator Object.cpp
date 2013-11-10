#include<iostream>
#include <vector>
#include <algorithm>
using namespace std;

class gen
{
public:
    gen(double x_zero, double increment):x(x_zero),incr(increment){};
    double operator()(){x+=incr; return x;}

private:
    double x,incr;
};

double integrate(gen g, int n)
{
    vector<double> fx(n);

    generate(fx.begin(), fx.end(),g);

    for(auto i:fx)
        cout<<i<<" ";

    return accumulate(fx.begin(), fx.end(),0.0);
}

int main()
{
    const int n=10;

    gen g(0.0, 1);


    cout<<endl<<integrate(g,n);
}

/*

1 2 3 4 5 6 7 8 9 10
55*/
