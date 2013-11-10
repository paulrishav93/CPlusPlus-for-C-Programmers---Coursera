#include<iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Person
{
public:
    int age;
};

int main()
{
    vector <Person> v;

    v.resize(10);

    for(int i=1;i<10;++i)
    {
        v[i].age=10-i;
    }
    
    // [arg1, arg2, ... ] can be sent to the lambda code { ... } if necessary
    sort( v.begin(), v.end(), [](const Person &a, const Person &b){return a.age < b.age;});

    for(int i=1;i<10;++i)
        cout<<v[i].age;
}
