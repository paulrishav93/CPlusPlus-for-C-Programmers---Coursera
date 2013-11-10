#include<iostream>
#include <vector>
#include <algorithm>

using namespace std;

enum class PlayerColor{RED,BLUE};

ostream& operator<<(ostream &stream, PlayerColor &ob)
{
    if(ob==PlayerColor::BLUE)
        stream<<"BLUE";
    else if(ob==PlayerColor::RED)
        stream<<"RED";

    return stream;
}

PlayerColor operator+(PlayerColor &ob2)
{
    return static_cast<PlayerColor>((static_cast<int>(ob2)+1)%2);
}

int main()
{
    PlayerColor player1color=PlayerColor::RED;
    PlayerColor player2color=+player1color;
    cout<<player2color;
}
