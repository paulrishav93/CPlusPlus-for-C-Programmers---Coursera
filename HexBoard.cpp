#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

enum class PlayerColor {BLUE=1, RED};

ostream& operator << (ostream &stream, PlayerColor &ob)
{
    if(ob==PlayerColor::BLUE)
        stream<<"BLUE";
    else stream<<"RED";

    return stream;
}

class HexBoard
{
private:
    int SIZE;
    PlayerColor player1color,player2color;
    vector< vector<char> > board;
    vector< vector<int> > edge;
    vector< vector<int> > vertices;
    
public:
    HexBoard(int size):SIZE(size),
        board(vector< vector<char> > (size+1, vector<char>(size+1, '.'))),
        edge(vector< vector<int> > (size*size + 1))
        {
            vertices=vector< vector<int> > (size +1,vector<int>(size + 1));
            auto c=1;
            for(auto i=vertices.begin()+1;i<vertices.end();++i)
                for(auto j=i->begin()+1;j<i->end();++j)
                    *j=c++;
        }

    void printBoard();
    void Game();
    void createBoard();
    inline void addEdge(int u, int v);
};

void HexBoard::addEdge(int u, int v)
{
    edge[u].push_back(v);
    edge[v].push_back(u);
}
void HexBoard::createBoard()
{
    for(int i=1;i<=SIZE;++i)
    {
        for(int j=1;j<=SIZE;++j)
        {
            if(i==1)
            {
                if(j<SIZE)
                {
                    addEdge(vertices[i][j+1],vertices[i][j]);
                    addEdge(vertices[i+1][j],vertices[i][j]);
                    addEdge(vertices[i+1][j+1],vertices[i][j]);
                }
                
                else addEdge(vertices[i][j],vertices[i+1][j]);
            }
            
            else if(i==SIZE){}
            else if (j==1){}
            else if (j==SIZE){}
            else{}
        }
    }
}
void HexBoard::printBoard()
{
    auto space=1,num=1;
    char a;

    cout<<"  ";
    for(a=65;a<=SIZE+64;++a)
        cout<<a<<" ";
    cout<<endl;

    for(auto i=board.begin()+1;i<board.end();++i)
    {
        for(auto k=1;k<space;++k)
            cout<<" ";
        if(num<=9) cout<<" ";
        space++;
        cout<<num<<" ";
        for(auto j=i->begin()+1;j<i->end();++j)
            cout<<*j<<" ";
        cout<<num++<<endl;
    }

    for(auto k=1;k<space;++k)
            cout<<" ";
    cout<<"  ";
    for(char a=65;a<=SIZE+64;++a)
        cout<<a<<" ";
}

void HexBoard::Game()
    {
        printBoard();
        int choice;
        cout<<endl<<endl;

        cout<<"Player 1 - Enter 1 for Blue and 2 for Red"<<endl;
        cin>>choice;

        while(choice < 1 || choice > 2)
        {
            cout<<"Enter a valid choice between 1 and 2"<<endl;
            cin>>choice;
        }

        player1color=static_cast<PlayerColor>(choice);
        player2color=static_cast<PlayerColor>(static_cast<int>((choice+1)%2));

        cout<<"Player 1 Color is: "<<player1color<<endl;
        cout<<"Player 2 Color is: "<<player2color<<endl;
    }

int main()
{
    HexBoard a(11);
    a.Game();
}
