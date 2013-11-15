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
    else if(ob==PlayerColor::RED)
        stream<<"RED";
    else stream<<"BLACK";

    return stream;
}

class HexBoard
{
private:
    PlayerColor player1color,player2color;
    vector< vector<char> > board;
    vector< vector<int> > edge;
    typedef pair<int, PlayerColor> ip;
    vector< vector< ip > > vertices;
    queue<ip> Q;
    const int EAST,WEST,NORTH,SOUTH;
    int firstPlayer,presentPlayer,SIZE;                        //1 for player1 and 2 for player2
    vector<int> markedbfs;

    void printBoard();
    void Game();
    void createBoard();
    inline void addEdge(int,int);
    void displayEdges();
    int mainGame();
    void inputPlayer(int &, int &, const int);
    char character(const int);
    void bfs(const ip);
    int row_val(int);
    int column_val(int);
    void set_virtual_nodes();
    void endgame();

public:
    HexBoard(int size):SIZE(size),
    board(vector< vector<char> > (size+1, vector<char>(size+1, '.'))),
    edge(vector< vector<int> > (size*size + 1 + 4)),EAST(size*size+1),WEST(size*size+2),
    NORTH(size*size+3),SOUTH(size*size+4),markedbfs(vector<int>(size*size+5,-1))
    {
        vertices=vector< vector< ip > > (size +1 + 1 ,vector< ip >(size + 1 ));
        auto c=1;
        for(auto i=vertices.begin()+1;i<vertices.end();++i)
            for(auto j=i->begin()+1;j<i->end();++j)
            {
                j->first=c++;
                j->second=PlayerColor::BLACK;
            }
        set_virtual_nodes();
        srand(time(0));
        presentPlayer=0;
        Game();
    }



};

void HexBoard::set_virtual_nodes()
{
    vertices[SIZE+1].resize(SIZE);
    vertices[row_val(EAST)][column_val(EAST)].second=PlayerColor::BLUE;
    vertices[row_val(EAST)][column_val(EAST)].first=EAST;
    vertices[row_val(WEST)][column_val(WEST)].second=PlayerColor::BLUE;
    vertices[row_val(WEST)][column_val(WEST)].first=WEST;
    vertices[row_val(NORTH)][column_val(NORTH)].second=PlayerColor::RED;
    vertices[row_val(NORTH)][column_val(NORTH)].first=NORTH;
    vertices[row_val(SOUTH)][column_val(SOUTH)].second=PlayerColor::RED;
    vertices[row_val(SOUTH)][column_val(SOUTH)].first=SOUTH;
}

void HexBoard::bfs(const ip vertex)
{
    Q.push(vertex);
    PlayerColor present_player_color;

    while(!Q.empty())
    {
        ip v=Q.front();
        int node=v.first;
        Q.pop();

        markedbfs[node]=presentPlayer;

        for(int i=0;i<edge[node].size();++i)
        {
            int j=edge[node][i];
            int row_j=row_val(j);
            int column_j=column_val(j);

            if(presentPlayer==1) present_player_color=player1color;
            else present_player_color=player2color;

            if(markedbfs[j]==-1 && vertices[row_j][column_j].second==present_player_color)
             {
                 Q.push(vertices[row_j][column_j]);
             }
        }
    }
    cout<<endl;

    if(markedbfs[WEST]==presentPlayer && markedbfs[EAST]==presentPlayer)  endgame();

    if(markedbfs[NORTH]==presentPlayer && markedbfs[SOUTH]==presentPlayer) endgame();

    markedbfs=(vector<int>(SIZE*SIZE+5,-1));
}

void HexBoard::endgame()
{
    cout<<"Player: "<<presentPlayer<<" wins\n";
    exit(0);
}

int HexBoard::column_val(int val)
{
    int rem=val%SIZE;
    if(!rem) rem=SIZE;
    return rem;
}

int HexBoard::row_val(int val)
{
    double fraction=static_cast<double>(val)/SIZE;
    val=val/SIZE;
    if(fraction - static_cast<int>(fraction)!=0)
        val++;
    return val;
}

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
    if(presentPlayer)
    {
        cout<<endl;
        cout<<"**************************************************************"<<endl;
        cout<<"Player: "<<presentPlayer<<" moves from";
        if(presentPlayer)
        {
            PlayerColor temp;
            if(presentPlayer==1) temp=player1color;
            else temp=player2color;

            if(temp==PlayerColor::BLUE)
            {
                cout<<" EAST ---- > WEST"<<endl;
            }
            else cout<<" NORTH ---- > SOUTH"<<endl;
        }
    }

    cout<<endl;
    cout<<"               N"<<endl;
    cout<<"               |"<<endl;
    cout<<"           E --+--> W"<<endl;
    cout<<"               |"<<endl;
    cout<<"               v"<<endl;
    cout<<"               S"<<endl;
    cout<<endl<<endl<<endl;
    auto space=1,num=1;
    char a;

    cout<<"  ";
    for(a=65;a<=SIZE+64;++a)
        cout<<a<<" ";
    cout<<endl;

    for(auto i=board.begin()+1;i<board.end();++i)
    {

        for(auto k=1;k<space;++k) cout<<" ";

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
    createBoard();
    cout<<endl<<endl<<"Board has been created\n";
    char choice;
    cout<<endl<<endl;

    cout<<"Player 1 - Enter 1 for Blue (EAST->WEST) and 2 for Red (NORTH->SOUTH)"<<endl;
    cin>>choice;
    int ch=choice-48;
    cout<<ch<<endl;
    cout<<endl;

    while(ch < 1 || ch > 2)
    {
        cout<<"Enter a valid choice between 1 and 2"<<endl;
        cin>>choice;
        ch=choice-48;
    }

    player1color=static_cast<PlayerColor>(ch);
    if(player1color==PlayerColor::BLUE) player2color=PlayerColor::RED;
    else player2color=PlayerColor::BLUE;

    cout<<"Player 1 Color is: "<<player1color<<endl;
    cout<<"Player 2 Color is: "<<player2color<<endl;

    cout<<"Which player will make the first move?"<<endl;
    cout<<"Press 1 for player 1 and 2 for player 2, or press any button for a toss"<<endl;
    cin>>ch;

    if(ch==1) firstPlayer=1;
    else if (ch==2) firstPlayer=2;
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
    int row, column;

    if(firstPlayer==1) presentPlayer=1;
    else presentPlayer=2;

    while(true)
    {
        inputPlayer(row,column,presentPlayer);
        if (presentPlayer==1) vertices[row][column].second=static_cast<PlayerColor>(player1color);
        else vertices[row][column].second=static_cast<PlayerColor>(player2color);
        board[row][column]=character(presentPlayer);
        printBoard();
        bfs(vertices[row][column]);
        if(presentPlayer==1) presentPlayer=2;
        else presentPlayer=1;
    }

}

int main()
{
    int size,ch=1;

    while(ch)
    {
        cout<<"Enter size of the HexBoard: ";
        cin>>size;
        if(size==0) cout<<"Wrong Size choice. Input Again\n";
        else HexBoard a(size);
    }

    return 0;
}
