/* Kruskal's MST Algorithm*/

/*  A of edges is maintained. The edges are sorted according to their weights. The least cost edge is
    added to the MST if it does not create a cycle.

    Each vertex is initially in the set of its own. Thus, we have used a parent[] array for this where,
    each vertex is a parent of its own. i.e., parent[i]=i.

    Now we pick the least weighted edges one by one. If both end of the edges lie in the same set, then
    a cycle is formed and we reject that edge. Otherwise, the edge is selected.

    When we add an edge to the MST, we make sure that both vertices and the sets the initally belong to
    are not united*/


#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>

using namespace std;

//edge stores the names of the vertices only
#define edge pair< int, int>

class Graph
{
public:

    //double corresponds to the weight of the edge
    //graph shall store all the edges initally, whereas MST shall store the finall tree
    vector< pair < double, edge > > graph, MST;
    double totalCost;

    //V stores the total number of vertices and E stores the total number of edges
    int V, E;

    //the parent[] array which stores the value of the root vertex for each vertex
    vector<int> parent;

    Graph(int vertices)
    {
        V=vertices;
        parent.resize(V+1);
        E=0;
    //initially, the parent of a vertex is the vertex itself
        for(int i=0;i<=V;++i)
            parent[i]=i;
    }

    //inserts all the edges to the graph container
    void insertEdge(int u, int v, double w)
    {
    	graph.push_back(pair< double, edge >(w, edge(u, v)));
    	E++;    //increment number of edges
    }

    //return the paretn vertex of a node. Here, path compression is used
    //e.g. if 2 was the parent of 1 and 3 was the parent of 2
    //then if we findSet(1), then parent[1] will be updated to 3
    //and parent[2] will remain 3
    int findSet(int x)
    {
        if(x!=parent[x])
            parent[x]=findSet(parent[x]);

        return parent[x];
    }

    //finds the MST
    void kruskal()
    {
        int i, parentu, parentv, MSTsize;

        totalCost=0;

        //sorts vertices according to weight
        sort(graph.begin(),graph.end());

        //we need to process all the edges as some of them may form cycles in between
        for(i=0; i<E;++i)
        {
            //finds parent of each vertex
            parentu=findSet(graph[i].second.first);
            parentv=findSet(graph[i].second.second);

            //check if they are part of the same set or not
            if(parentu!=parentv)
            {
                MST.push_back(graph[i]);
                totalCost+=graph[i].first;

                //parent of one vertex is made equal to the parent of the other vertex,
                //thereby uniting both the sets
                parent[parentu]=parent[parentv];
            }
        }

        MSTsize=MST.size();

        cout<<"Cost of MST: "<<totalCost<<endl<<endl;

        cout<<"The edges and their weights are as follows:"<<endl;
        for(i=0; i<MSTsize; ++i)
        {
            cout<<MST[i].second.first<<"<---->"<<MST[i].second.second<<" == "<<MST[i].first<<endl;
        }


    }
};

int main()
{
    ifstream fin;
    fin.open("input.txt");      //open a file

    int V,E=0,u,v;              //number of edges are zero initially
    double w;

    fin>>V;                     //input number of nodes from file

    Graph G(V);

    while(!fin.eof())
    {
    	fin>>u>>v>>w;
    	E++;
    	G.insertEdge(u,v,w);
    }

    G.kruskal();
}
