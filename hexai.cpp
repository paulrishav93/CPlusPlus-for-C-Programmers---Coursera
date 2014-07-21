/*

 HEX BOARD GAME


    All vertices on the edges of the board are connected to four nodes, EAST, WEST, NORTH and SOUTH respectively.
    They have been called virtual nodes.

    When a player chooses a location on the board, a BREADTH FIRST SEARCH is performed from that location. If two
    virtual nodes on opposite ends are visited for the respective player, the game ends


*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

enum class player_color {BLACK, BLUE, RED};
enum class player_type{HUMAN, COMPUTER};

//useful for testing the color of each position on the board

ostream& operator << (ostream &stream, player_color &ob)
{
    if(ob==player_color::BLUE)
        stream<<"BLUE";
    else if(ob==player_color::RED)
        stream<<"RED";
    else stream<<"BLACK";

    return stream;
}

class HexBoard
{

public:

    HexBoard(int size);

private:

    player_type current_player;                                 //HUMAN or COMPUTER
    player_color human_color, computer_color;                   //decides color associated with each player
    const int EAST,WEST,NORTH,SOUTH,SIZE;                       //the virtual nodes and SIZE of the board
    vector< vector<char> > board;                               //stores the printable version of the board 2D matrix
    vector< vector<int> > edge;                                 //stores the graph where each vertex is connected
    typedef pair<int, player_color> ip;                         //an object which shall store the vertex number & its color
    typedef pair<int, int> ii;
    typedef pair< int, ii> i_ii;
    vector< vector< ip > > vertices;                            //2D vector for pair<ip> objects
    queue<ip> Q;                                                //will be used in BFS
    int spaces_to_fill,x_pos, y_pos;
    vector<int> marked_bfs;                                      //stores visited vertices in BFS
    string player_name;
    vector< ii > empty_points, empty_points_shuffle, points_to_be_removed;  //deals with empty points on board in simulation
    vector< ii > points_filled_by_simulation;                   //stores the points which will be chosen by the computer in the sim
    priority_queue< i_ii > max_priority_queue;                  //stores the list of possible moves with their winner chance
    int first_player,present_player;                            //keeps info about the present and the first player

    void print_board();
    void start_game();
    void continue_game();
    void create_board();
    inline void add_edge(int, int);

    int bfs(const ip);
    void human_player_move(int, int);
    void computer_player_move();
    int row_val(int);
    int column_val(int);
    void set_virtual_nodes();
    void end_game();
    char character_of_player(player_type);
    void input_player(int &, int &, player_type );
    void input_from_human(int &,int &);
    void input_from_computer(int &,int &);
    void input_from_computerA(int &,int &);
    void find_empty_points();
    int do_simulation();

};

//constructor for HexBoard
HexBoard::HexBoard(int size):
    SIZE(size),board(vector< vector<char> > (size+1, vector<char>(size+1, '.'))),
    edge(vector< vector<int> > (size*size + 1 + 4)),EAST(size*size+1),WEST(size*size+2),
    NORTH(size*size+3),SOUTH(size*size+4),marked_bfs(vector<int>(size*size+5,-1))
    {
        vertices=vector< vector< ip > > (size +1 + 1 ,vector< ip >(size + 1 ));
        auto c=1;
        for(auto i=vertices.begin()+1;i<vertices.end();++i)
            for(auto j=i->begin()+1;j<i->end();++j)
            {
                j->first=c++;
                j->second=player_color::BLACK;
            }
        set_virtual_nodes();
        srand(time(0));
        present_player=0;
        human_color=player_color::BLUE;
        computer_color=player_color::RED;
        start_game();
    }

void HexBoard::set_virtual_nodes()
{
    //sets the sttributes for all virtual nodes
    vertices[SIZE+1].resize(SIZE);
    vertices[row_val(EAST)][column_val(EAST)].second=player_color::BLUE;
    vertices[row_val(EAST)][column_val(EAST)].first=EAST;
    vertices[row_val(WEST)][column_val(WEST)].second=player_color::BLUE;
    vertices[row_val(WEST)][column_val(WEST)].first=WEST;
    vertices[row_val(NORTH)][column_val(NORTH)].second=player_color::RED;
    vertices[row_val(NORTH)][column_val(NORTH)].first=NORTH;
    vertices[row_val(SOUTH)][column_val(SOUTH)].second=player_color::RED;
    vertices[row_val(SOUTH)][column_val(SOUTH)].first=SOUTH;
}

//calculate the equivalent column address for 'val'
//e.g. val=25 for position 5/5 on board of SIZE 5

int HexBoard::column_val(int val)
{
    int rem=val%SIZE;
    if(!rem) rem=SIZE;
    return rem;
}

//calculate equivalent row address
int HexBoard::row_val(int val)
{
    double fraction=static_cast<double>(val)/SIZE;
    val=val/SIZE;
    if(fraction - static_cast<int>(fraction)!=0)
        val++;
    return val;
}

//print the board
void HexBoard::print_board()
{


    cout<<endl;
    cout<<"             N"<<endl;
    cout<<"              \\"<<endl;
    cout<<"           W --+--> E"<<endl;
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

//starts the game
void HexBoard::start_game()
{
    print_board();
    create_board();

    cout<<endl<<endl<<"Board has been created\n";
    char choice;
    cout<<endl;
    cout<<"*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*";
    cout<<endl;

    cin.clear();
    cout<<"Enter your name: ";
    getline(cin,player_name);


    cout<<player_name<<" has color Blue (WEST->EAST) and computer has color Red (NORTH->SOUTH) : ";

    cout<<"Will you make the first move, "<<player_name<<" ?"<<endl;
    cout<<"Press Y for Yes, and N for No, or T for a Toss!"<<endl;

    cin>>choice;

    while(choice!='Y' && choice!='y' && choice!= 'N' && choice!= 'n' && choice!= 'T' && choice!= 't')
    {
        cout<<"Enter valid choice: Y for Yes, N for No, T for a Toss!"<<endl;
        cin>>choice;
    }

    if(choice=='Y'||choice=='y') first_player=static_cast<int>(player_type::HUMAN);
    else if (choice=='N' || choice=='n') first_player=static_cast<int>(player_type::COMPUTER);
    else
    {
        double probability=static_cast<double>(rand())/RAND_MAX;
        if(probability>=0.5) first_player=static_cast<int>(player_type::COMPUTER);
        else first_player=static_cast<int>(player_type::HUMAN);
    }

    cout<<"*********************************************************************"<<endl;

    if(first_player==static_cast<int>(player_type::HUMAN))
    {
        cout<<player_name<<" will start the game!"<<endl;
        current_player=player_type::HUMAN;
    }

    else
    {
        cout<<" The Computer will start the game!"<<endl;
        current_player=player_type::COMPUTER;
    }

    continue_game();
}

//adjacency list (graph) created for the board

void HexBoard::create_board()
{
    for(int i=1;i<=SIZE;++i)
    {
        for(int j=1;j<=SIZE;++j)
        {
            if(i<SIZE)
            {
                if(j==1)
                {
                    add_edge(vertices[i][j+1].first,vertices[i][j].first);
                    add_edge(vertices[i+1][j].first,vertices[i][j].first);
                }

                else if(j<SIZE)
                {
                    add_edge(vertices[i][j+1].first,vertices[i][j].first);
                    add_edge(vertices[i+1][j].first,vertices[i][j].first);
                    add_edge(vertices[i+1][j-1].first,vertices[i][j].first);
                }

                else
                {
                    add_edge(vertices[i][j].first,vertices[i+1][j].first);
                    add_edge(vertices[i+1][j-1].first,vertices[i][j].first);
                }
            }

            else if(i==SIZE)
            {
                if(j<SIZE)
                add_edge(vertices[i][j].first,vertices[i][j+1].first);
            }
        }
    }

    //connects the virtual nodes to the relevant vertices on the board
    for(int i=1;i<=SIZE*SIZE-SIZE+1;i+=SIZE)
        add_edge(EAST,i);

    for(int i=SIZE;i<=SIZE*SIZE;i+=SIZE)
        add_edge(WEST,i);

    for(int i=1;i<=SIZE;++i)
        add_edge(NORTH,i);

    for(int i=SIZE*SIZE-SIZE+1;i<=SIZE*SIZE;++i)
        add_edge(SOUTH,i);
}

void HexBoard::add_edge(int u, int v)
{
    edge[u].push_back(v);
    edge[v].push_back(u);
}

//ends the game
void HexBoard::end_game()
{
    cout<<endl;
    cout<<"****************CONGRATULATIONS****************"<<endl<<endl;
    if(current_player==player_type::HUMAN)
        cout<<endl<<player_name<<" wins"<<endl;

    else cout<<"The Computer Wins!"<<endl;

    exit(0);
}

int HexBoard::bfs(const ip vertex)
{
    Q.push(vertex);
    player_color present_player_color;

    while(!Q.empty())
    {
        ip v=Q.front();
        int node=v.first;
        Q.pop();

        marked_bfs[node]=static_cast<int>(current_player);

        for(unsigned int i=0;i<edge[node].size();++i)
        {
            int j=edge[node][i];
            int row_j=row_val(j);
            int column_j=column_val(j);

            if(current_player==player_type::HUMAN) present_player_color=player_color::BLUE;
            else present_player_color=player_color::RED;

            if(marked_bfs[j]==-1 && vertices[row_j][column_j].second==present_player_color)
             {
                 Q.push(vertices[row_j][column_j]);
             }
        }
    }

    int winner = 2;
    //game ends only if same player is on both virtual nodes
    if(marked_bfs[WEST]==static_cast<int>(current_player) && marked_bfs[EAST]==static_cast<int>(current_player) && present_player_color==player_color::BLUE)
    {
        winner=static_cast<int>(current_player);
    }

    if(marked_bfs[NORTH]==static_cast<int>(current_player) && marked_bfs[SOUTH]==static_cast<int>(current_player) && present_player_color==player_color::RED)
    {
        winner=static_cast<int>(current_player);
    }

    //reset the markedbfs array
    marked_bfs=(vector<int>(SIZE*SIZE+5,-1));

    return winner;
}


//the character representing the players may be set here
char HexBoard:: character_of_player(player_type current_player)
{
    if(current_player==player_type::HUMAN)
        return 'X';
    else return 'O';
}

//main game is played here
void HexBoard::continue_game()
{
    int row, column, winner;

    while(true)
    {
        input_player(row,column,current_player);
        if (current_player==player_type::HUMAN) vertices[row][column].second=player_color::BLUE;
        else vertices[row][column].second=player_color::RED;
        board[row][column]=character_of_player(current_player);
        print_board();
        winner=bfs(vertices[row][column]);
        if(winner!=2) end_game();
        else
        {
            if(current_player==player_type::HUMAN)
                current_player=player_type::COMPUTER;

            else current_player=player_type::HUMAN;
        }
    }
}

//decides whether input is from computer or human
void HexBoard::input_player(int &row, int &column, player_type current_player)
{
    if(current_player==player_type::HUMAN)
        input_from_human(row,column);
    else input_from_computer(row,column);
}

//takes the input from the human
void HexBoard::input_from_human(int &row, int &column)
{
    char c;
    cout<<endl<<endl;
    row=column=0;

    cout<<endl;
    cout<<"**************************************************************"<<endl;
    cout<<player_name<<" moves from WEST ---- > EAST and is represented by "<<character_of_player(current_player)<<endl;


    cout<<"**************************************************************"<<endl;

    while(row>SIZE||row<1||column>SIZE||column<1)
    {
        cout<<"Enter your move player "<<player_name<<", e.g. 4C : ";
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

//for the input from the computer we do the following
// 1. store all the empty vertices in two arrays (empty_points and empty_points_shuffle)
// 2. Pick each empty point from empty_points and for each point do 1000 times
//          3. Shuffle empty_points shuffle
//          4. Place computer stones on first (no_of_empty_points-1)/2 stones
//              from empty_point_shuffle except the empty_point being evaluated
//              points_filled_by_simulation is used as a container for this
//          5. Do a BFS
//          6. If the Computer wins, then add 1 to the no_of_wins
//          7. Add pair<position_value, pair < x_pos, y_pos> > into a max priority queue max_pq
//          8. We need to reset everything before the next empty point is evaluated.
//                  8(a). run through empty_points array
//                          reset each empty point on the original board
// 9. Pop the first element of the priority queue and make that as the final choice for the computer

void HexBoard::input_from_computerA(int &row, int &column)
{

    row=(rand()%(SIZE))+1;
    column=(rand()%(SIZE))+1;

    while(board[row][column] != '.')
    {
        row=(rand()%(SIZE))+1;
        column=(rand()%(SIZE))+1;
    }
}

void HexBoard::input_from_computer(int &row, int &column)
{
    cout<<endl<<endl;

    cout<<endl;
    cout<<"**************************************************************"<<endl;
    cout<<"Computer moves from NORTH->SOUTH and is represented by "<<character_of_player(current_player)<<endl;

    cout<<"**************************************************************"<<endl;

    cout<<"Please wait while the computer decides on a move."<<endl;

    //stores all empty points into
    //empty_points and empty_points_shuffle
    find_empty_points();

    int no_of_empty_spaces=empty_points.size();

    //calculates no of spaces that should be filled by the computer
    spaces_to_fill=static_cast<int>(no_of_empty_spaces-1)/2;

    //do simulation from each point and push total number of wins it creates
    //into a max priority queue
    for(auto i=empty_points.begin(); i<empty_points.end(); ++i)
    {
        x_pos=i->first;
        y_pos=i->second;

        int no_of_wins=do_simulation();

        max_priority_queue.push(i_ii(no_of_wins, ii(x_pos,y_pos)));
    }

    //get the best position
    i_ii best_play=max_priority_queue.top();

    //get the row and column value of best position
    row=best_play.second.first;
    column=best_play.second.second;

    //make sure that the position is not already occupied
    while(board[row][column] != '.')
    {
        max_priority_queue.pop();
        best_play=max_priority_queue.top();
        row=best_play.second.first;
        column=best_play.second.second;
    }

    //empty all containers
    while(!max_priority_queue.empty()) max_priority_queue.pop();

    empty_points.clear();
    empty_points_shuffle.clear();
}

//does the simulation from each point
int HexBoard::do_simulation()
{
    int count_sim=1, no_of_wins=0, x1_pos, y1_pos;
    int c=1;
    const int no_of_simulations=1000;

    //the point w.r.t which the simulation is to be performed
    //is colored RED. Should be reset after simulation is over
    vertices[x_pos][y_pos].second=player_color::RED;

    while(count_sim<=no_of_simulations)
    {
        random_shuffle(empty_points_shuffle.begin(),empty_points_shuffle.end());

        auto eps_itr=empty_points_shuffle.begin();

        int count_space=1;

        //decides which points should be actually occupied by the computer
        //in the simulation
        while(count_space<=spaces_to_fill && eps_itr!= empty_points_shuffle.end())
        {
            x1_pos=eps_itr->first;
            y1_pos=eps_itr->second;

            if(x1_pos!=x_pos || y1_pos != y_pos && board[x1_pos][y1_pos] == '.')
            {
                points_filled_by_simulation.push_back(ii (*eps_itr));
                count_space++;
            }

            eps_itr++;
        }

        //assign the selected points the color RED and then perform a BFS to check for winner
        for(auto pfbs_itr=points_filled_by_simulation.begin();pfbs_itr<points_filled_by_simulation.end();++pfbs_itr)
        {
            x1_pos=pfbs_itr->first;
            y1_pos=pfbs_itr->second;
            vertices[x1_pos][y1_pos].second=player_color::RED;
        }

        int winner=bfs(vertices[x1_pos][y1_pos]);

        //if the computer wins, then add 1 to the no_of_wins that point makes
        if(winner==static_cast<int>(player_type::COMPUTER))
        {
            no_of_wins += 1;
        }

        count_sim++;

        //reset the board by making the selected points' color as BLACK again
        for(auto pfbs_itr=points_filled_by_simulation.begin();pfbs_itr<points_filled_by_simulation.end();++pfbs_itr)
        {
            x1_pos=pfbs_itr->first;
            y1_pos=pfbs_itr->second;
            vertices[x1_pos][y1_pos].second=player_color::BLACK;
        }

        //clear the container
        points_filled_by_simulation.clear();
    }

    //make BLACK the position w.r.t which the simulation was being performed
    vertices[x_pos][y_pos].second=player_color::BLACK;

    return no_of_wins;
}

//uses the board[][] to check for empty points
//on the HexBoard
void HexBoard::find_empty_points()
{
    for(auto i=1;i<=SIZE;++i)
        for(auto j=1;j<=SIZE;++j)
            if(board[i][j]=='.')
            {
                empty_points.push_back(ii(i,j));
                empty_points_shuffle.push_back(ii(i,j));
            }
}

int main()
{
    int size,ch=1;

    while(ch)
    {
        cout<<"Enter size of the HexBoard (minimum 4): ";
        cin>>size;
        cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
        if(size<=3) cout<<"Wrong Size choice. Input Again\n";
        else HexBoard a(size);
    }

    return 0;
}
