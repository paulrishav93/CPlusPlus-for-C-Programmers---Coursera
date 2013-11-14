#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cstdlib>
#include <ctime>

using namespace std;

enum class PlayerColor {BLACK, BLUE, RED};

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
    typedef pair<int, PlayerColor> ip;
    vector< vector< ip > > vertices;
    //vector< vector<PlayerColor> > vertexColor;
    const int EAST,WEST,NORTH,SOUTH;
    int firstPlayer;                        //1 for player1 and 2 for player2

public:
    HexBoard(int size):SIZE(size),
    board(vector< vector<char> > (size+1, vector<char>(size+1, '.'))),
    edge(vector< vector<int> > (size*size + 1 + 4)),EAST(size*size+1),WEST(size*size+2),
    NORTH(size*size+3),SOUTH(size*size+4)
    {
        vertices=vector< vector< ip > > (size +1,vector< ip >(size + 1));
        auto c=1;
        for(auto i=vertices.begin()+1;i<vertices.end();++i)
            for(auto j=i->begin()+1;j<i->end();++j)
            {
                (*j).first=c++;
                j->second=PlayerColor::BLACK;
            }
    }

    void printBoard();
    void Game();
    void createBoard();
    inline void addEdge(int,int);
    void displayEdges();
    int mainGame();
    void inputPlayer(int &, int &, const int);
    char character(const int);
    void bfs(int source);

};

char HexBoard:: character(const int presentPlayer)
    {
        if(presentPlayer==1)
            return 'X';
        else return '0';
    }
void HexBoard::inputPlayer(int &row, int &column, const int presentPlayer)
{
    char r,c;
    cout<<endl<<endl;
    row=column=0;

    while(row>SIZE||row<1||column>SIZE||column<1)
    {
        cout<<"Enter your move player "<<presentPlayer<<", e.g. 6A"<<endl;
        cin>>r>>c;
        row=static_cast<int>(r)-48;
        column=static_cast<int>(c)-64;
        if(row>SIZE||row<1||column>SIZE||column<1)
            cout<<"Invalid Move."<<endl;
        else if(board[row][column]!='.')
        {
            cout<<"Cell already occupied. Please try again."<<endl;
            row=0;
        }
    }
}

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
            if(i<SIZE)
            {
                if(j==1)
                {
                    addEdge(vertices[i][j+1].first,vertices[i][j].first);
                    addEdge(vertices[i+1][j].first,vertices[i][j].first);
                    addEdge(vertices[i+1][j+1].first,vertices[i][j].first);
                }

                else if(j<SIZE)
                {
                    addEdge(vertices[i][j+1].first,vertices[i][j].first);
                    addEdge(vertices[i+1][j].first,vertices[i][j].first);
                    addEdge(vertices[i+1][j+1].first,vertices[i][j].first);
                    addEdge(vertices[i+1][j-1].first,vertices[i][j].first);
                }

                else
                {
                    addEdge(vertices[i][j].first,vertices[i+1][j].first);
                    addEdge(vertices[i+1][j-1].first,vertices[i][j].first);
                }
            }

            else if(i==SIZE)
            {
                if(j<SIZE)
                addEdge(vertices[i][j].first,vertices[i][j+1].first);
            }
        }
    }

    //create 4 virtual nodes, one on each side of the hex board

    for(int i=1;i<=SIZE*SIZE-SIZE+1;i+=SIZE)
        addEdge(EAST,i);

    for(int i=SIZE;i<=SIZE*SIZE;i+=SIZE)
        addEdge(WEST,i);

    for(int i=1;i<=SIZE;++i)
        addEdge(NORTH,i);

    for(int i=SIZE*SIZE-SIZE+1;i<=SIZE*SIZE;++i)
        addEdge(SOUTH,i);
}

void HexBoard::printBoard()
{
    cout<<endl<<endl<<endl;
    auto space=1,num=1;
    char a;
    createBoard();

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


void HexBoard::displayEdges()
{
    auto c=1;
    for(auto i=edge.begin()+1;i<edge.end();++i)
    {
        cout<<c++<<": ";
        for(auto j=i->begin();j<i->end();++j)
            cout<<*j<<" ";
        cout<<endl;
    }
}
void HexBoard::Game()
{
    printBoard();
    int choice;
    cout<<endl<<endl;

    cout<<"Player 1 - Enter 1 for Blue (EAST->WEST) and 2 for Red (NORTH->SOUTH)"<<endl;
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

    cout<<"Which player will make the first move?"<<endl;
    cout<<"Press 1 for player 1 and 2 for player 2, or press any button for a toss"<<endl;
    cin>>choice;

    if(choice==1) firstPlayer=1;
    else if (choice==2) firstPlayer=2;
    else
    {
        double probability=static_cast<double>(rand())/RAND_MAX;
        if(probability>=0.5) firstPlayer=2;
        else firstPlayer=1;
    }

    cout<<"Player "<<firstPlayer<<" will start the game"<<endl;

    mainGame();
}

int HexBoard::mainGame()
{
    int vnodeStart, vnodeEnd, row, column, r,c, presentPlayer;

    if(firstPlayer==1)
    {
        presentPlayer=1;
        if(player1color==PlayerColor::BLUE)           //for starting with BLUE
        {
            vnodeStart=EAST;
            vnodeEnd=WEST;

            while(4)
            {
                inputPlayer(row,column,presentPlayer);
                board[row][column]=character(presentPlayer);
                printBoard();
                if(presentPlayer==1) presentPlayer=2;
                else presentPlayer=1;
            }
        }

        else if(player1color==PlayerColor::RED)      //for starting with RED
        {
            vnodeStart=NORTH;
            vnodeEnd=SOUTH;
        }
    }

    else if (firstPlayer==2)
    {

    }
}



int main()
{
    srand(time(0));
    HexBoard a(11);
    a.Game();
    return 0;
}
