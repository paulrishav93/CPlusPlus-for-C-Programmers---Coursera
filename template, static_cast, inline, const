#include <iostream>
#include <vector>

using namespace std;

const int N=40;

template <class totalSum>

inline void sum(totalSum &p, const int n, vector<totalSum> d)
{
  p=0;

  for(int i=0; i<n; ++i)
     p+=d[i];
}

int main()
{
   double accum=0;

   vector<double> data(N);

   for(int i=0; i<N; ++i)
      data[i] = static_cast<double>(i)/2;

    sum(accum, N, data);

    cout<<accum<<endl;
	return 0;
}
