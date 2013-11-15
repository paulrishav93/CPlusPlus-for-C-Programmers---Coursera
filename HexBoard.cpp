/*  HEX BOARD GAME

All vertices on the edges of the board are connected to four nodes, EAST, WEST, NORTH and SOUTH respectively.
They have been called virtual nodes.

When a player chooses a location on the board, a BREADTH FIRST SEARCH is performed from that location. If two
virtual nodes on opposite ends are visited for the respective player, the game ends*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cstdlib>
#include <ctime>

using namespace std;

//each vertex can be BLACK,BLUE or RED

enum class PlayerColor {BLACK, BLUE, RED};

//overloading the left bitshift operator for printing
//objects of PlayerColor enum class
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

    PlayerColor player1color,player2color;                      //two players
    vector< vector<char> > board;                               //stores the printable version of the board 2D matrix
    vector< vector<int> > edge;                                 //stores the graph where each vertex is connected
    typedef pair<int, PlayerColor> ip;                          //an object which shall store the vertex number & its color
    vector< vector< ip > > vertices;                            //2D vector for pair<ip> objects
    queue<ip> Q;                                                //will be used in BFS
    const int EAST,WEST,NORTH,SOUTH;                            //stores the values assigned to the virtual nodes
    int firstPlayer,presentPlayer,SIZE;                         //SIZE stores size of the board
    vector<int> markedbfs;                                      //stores visited vertices in BFS

    void printBoard();                                          //prints the board
    void Game();                                                //begins the game
    void createBoard();                                         //creates a board, makes the adjacency list connecting all nodes
    inline void addEdge(int,int);                               //add an edge to the graph
    void displayEdges();                                        //not used here. used to check if all nodes are connected
    void mainGame();                                             //plays the game, players exchange turns
    void inputPlayer(int &, int &, const int);                  //takes an input from a specified player
    char character(const int);                                  //returns the character which shall represent the player on the board
    void bfs(const ip);                                         //BFS
    int row_val(int);                                           //for a given vertex value, calculates its equivalent row location
    int column_val(int);                                        //for a given vertex value, calculates its equivalent col location
    void set_virtual_nodes();                                   //sets the values of virtual nodes
    void endgame();                                             //ends the game

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
    //sets the sttributes for all virtual nodes
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

    //game ends only if same player is on both virtual nodes
    if(markedbfs[WEST]==presentPlayer && markedbfs[EAST]==presentPlayer && present_player_color==PlayerColor::BLUE)  endgame();

    if(markedbfs[NORTH]==presentPlayer && markedbfs[SOUTH]==presentPlayer && present_player_color==PlayerColor::RED) endgame();

    //reset the markedbfs array
    markedbfs=(vector<int>(SIZE*SIZE+5,-1));
}

void HexBoard::endgame()
{
    cout<<endl;
    cout<<"****************CONGRATULATIONS****************"<<endl<<endl;
    cout<<endl<<"Player: "<<presentPlayer<<" wins\n";
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

//the character representing the players may be set here
char HexBoard:: character(const int presentPlayer)
{
    if(presentPlayer==1)
        return 'X';
    else return 'O';
}

//takes the input of the row and column where a player wants to move next

void HexBoard::inputPlayer(int &row, int &column, const int presentPlayer)
{
    char c;
    cout<<endl<<endl;
    row=column=0;

    cout<<endl;
    cout<<"**************************************************************"<<endl;
    cout<<"Player: "<<presentPlayer<<" moves from";

    PlayerColor temp;
    if(presentPlayer==1) temp=player1color;
    else temp=player2color;

    if(temp==PlayerColor::BLUE) cout<<" WEST ---- > EAST and is represented by "<<character(presentPlayer)<<endl;

    else cout<<" NORTH ---- > SOUTH and is represented by "<<character(presentPlayer)<<endl;

    cout<<"**************************************************************"<<endl;

    while(row>SIZE||row<1||column>SIZE||column<1)
    {
        cout<<"Enter your move player "<<presentPlayer<<", e.g. 4C : ";
        cin>>row>>c;
        column=static_cast<int>(c)-64;
        if(row>SIZE||row<1||column>SIZE||column<1) cout<<"Invalid Move."<<endl;
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

    //connects the virtual nodes to the relevant vertices on the board
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


    cout<<endl;
    cout<<"             N"<<endl;
    cout<<"              \\"<<endl;
    cout<<"           E --+--> W"<<endl;
    cout<<"                \\"<<endl;
    cout<<"                 S"<<endl;
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

//function not used in program. may be removed
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

//initiates the game
void HexBoard::Game()
{
    printBoard();
    createBoard();

    cout<<endl<<endl<<"Board has been created\n";
    char choice;
    cout<<endl;
    cout<<"*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*";
    cout<<endl;

    cout<<"Player 1 - Enter 1 for Blue (WEST->EAST) and 2 for Red (NORTH->SOUTH) : ";
    cin>>choice;
    int ch=choice-48;
    cout<<endl;

    while(ch < 1 || ch > 2)
    {
        cout<<"*********************************************************************************"<<endl;
        cout<<"Enter a valid choice between 1 and 2 : ";
        cin>>choice;
        ch=choice-48;
    }

    player1color=static_cast<PlayerColor>(ch);
    if(player1color==PlayerColor::BLUE) player2color=PlayerColor::RED;
    else player2color=PlayerColor::BLUE;

    cout<<"Player 1 Color is: "<<player1color<<endl;
    cout<<"Player 2 Color is: "<<player2color<<endl;

    cout<<"Which player will make the first move?"<<endl;
    cout<<"Press 1 for player 1 and 2 for player 2, or press any other button for a toss : ";
    cin>>ch;

    if(ch==1) firstPlayer=1;
    else if (ch==2) firstPlayer=2;
    else
    {
        double probability=static_cast<double>(rand())/RAND_MAX;
        if(probability>=0.5) firstPlayer=2;
        else firstPlayer=1;
    }

    cout<<"*********************************************************************"<<endl;
    cout<<"Player "<<firstPlayer<<" will start the game"<<endl;
    mainGame();
}

//the game is played here, players change turns
void HexBoard::mainGame()
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
        cout<<"Enter size of the HexBoard (minimum 4): ";
        cin>>size;
        if(size<=3) cout<<"Wrong Size choice. Input Again\n";
        else HexBoard a(size);
    }

    return 0;
}
