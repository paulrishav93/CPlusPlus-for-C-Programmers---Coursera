#include<iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Person 
{
public:
    int age;
};

bool cmp( const Person a, const Person b ) 
{
    return a.age < b.age ;
}

int main() 
{
    vector <Person> v;

    v.resize(10);
    
    for(int i=1;i<10;++i)
    {
        v[i].age=10-i;
    }
    
    sort( v.begin(), v.end(), cmp );

    for(int i=1;i<10;++i)
        cout<<v[i].age;
}

/*
#include<iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Person
{
public:
    int age;
};

class cmp
{
public:
    bool operator() (  Person const &a,  Person const &b )
    {
        return a.age < b.age ;
    }
};


//bool cmp( const Person &a, const Person &b )
//{
//        return a.age < b.age ;
//}

int main()
{
    vector <Person> v;

    v.resize(10);

    for(int i=1;i<10;++i)
    {
        v[i].age=10-i;
    }
    //cmp cmp; this can be initialized at first as well.
    //sort( v.begin(), v.end(), cmp); can be used
    sort( v.begin(), v.end(), cmp());

    for(int i=1;i<10;++i)
        cout<<v[i].age;
}
*/
