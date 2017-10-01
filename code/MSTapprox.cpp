#include <cmath>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <cassert>
#include <list>
#include <limits>
#include <time.h>
#include <iomanip>
#include "common.h"

class Edge
{
public:
	int u, v;
	double weight;

	Edge(int u, int v, double weight) : weight(weight)
	{
		if (u < v) {
			this->u = u;
			this->v = v;
		} else
		if (u > v) {
			this->u = v;
			this->v = u;
		}
		else {
			// should never be reached here
			assert(false);
		}
	}

    bool operator < (const Edge & ref) const
    {
        return weight < ref.weight;
    }

    bool operator == (const Edge & ref) const
    {
		return u == ref.u && v == ref.v && weight == ref.weight;
    }

};

class Map
{
public:

	int N;

	std::vector<Edge> tovisit;
	std::set<int> V;
	std::multiset<Edge> E;

	Map(int N) : N(N) { }
	//constructor
};

//Function to parse edges from input file
Map parseEdges(std::vector<City> &graph)
{
 	int N;
	//N = number of nodes; E = number of edges

	N = graph.size();
	Map map(N);
	//define the object-graph of graph class
	//initiate it with N-number of nodes

	int u, v;
	double weight;

	for (int i = 0; i < N; ++i)
	  {
	    for (int j = 0; j < N; ++j)
	    {
	    	u = i;
	    	v = j;
	    	if(u!=v){
            weight = Dis_Cal(graph[i], graph[j]);
	    	map.tovisit.push_back(Edge(u, v, weight));
	    	}
	    }
	  }
	return map;
}

Map computeMST(Map & map)
{
	map.V.insert(0);

	sort(map.tovisit.begin(), map.tovisit.end());

	while (map.V.size() < map.N)
	{
		for (auto iter = map.tovisit.begin(); iter != map.tovisit.end();)
		{
			// iter is an 'Edge' varirable
			if (map.V.find(iter->u) != map.V.end() && map.V.find(iter->v) == map.V.end())
			{
				map.V.insert(iter->v);
				map.E.insert(* iter);
				iter = map.tovisit.erase(iter);

				iter = map.tovisit.end();
			}
			else if (map.V.find(iter->v) != map.V.end() && map.V.find(iter->u) == map.V.end())
			{
				map.V.insert(iter->u);
				map.E.insert(* iter);
				iter = map.tovisit.erase(iter);

				iter = map.tovisit.end();
			}
			else
			{
				iter ++;
			}
		}
	}
	return map;
}

// Graph class represents a directed graph using adjacency list representation
class Graph
{
    int V;    // No. of vertices
    std::list<int> *adj;    // Declare Pointer to an array containing adjacency lists
    void DFSUtil(int v, bool visited[], std::vector<int> &node);  // A function used by DFS
public:
    Graph(int V);   // Constructor
    void addEdge(int v, int w);   // function to add an edge to graph
    std::vector<int> DFS(int v);    // DFS traversal of the vertices reachable from v
};

Graph::Graph(int V)
{
    this->V = V;
    adj = new std::list<int>[V];
}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w); // Add w to v¡¯s list.
}

void Graph::DFSUtil(int v, bool visited[], std::vector<int> &node)
{
    // Mark the current node as visited and print it
    visited[v] = true;
    //std::cout << v << " ";
    node.push_back(v);

    // Recur for all the vertices adjacent to this vertex
    std::list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
        if (!visited[*i])
            DFSUtil(*i, visited, node);
}

// DFS traversal of the vertices reachable from v.
// It uses recursive DFSUtil()
std::vector<int> Graph::DFS(int v)
{
    // Mark all the vertices as not visited
    bool *visited = new bool[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;

    // Call the recursive helper function to print DFS traversal
    std::vector<int> node;
    DFSUtil(v, visited, node);
    return node;
}

void tourweight(Map &map, std::vector<City> &graph, clock_t &t_start, int &runtime, std::string output_file)
{
    std::string str1, str2;
    str1 = output_file + ".sol";
    str2 = output_file + ".trace";
    std::ofstream output1, output2;
	output1.open(str1);
	output2.open(str2);

    // Create a graph from MST output
    Graph g(map.N);

    std::multiset<Edge>::iterator i;

    for (auto i = map.E.begin(); i != map.E.end(); i++){
        g.addEdge(i->u, i->v);    //add v to node u's adj list
        g.addEdge(i->v, i->u);    //add u to node v's adj list
    }
    int a;
    std::vector <int> node;
    int weight = std::numeric_limits<int>::max();

    clock_t t_end;
    double duration = 0.0;
    int k=0;
    while(duration < runtime && k<map.N)
    {
        a=k;
        std::vector <int>().swap(node);
        node = g.DFS(a);
        node.push_back(a);

        int j;
        int sum=0;
        for(j=0; j< (node.size())-1; j++){
            sum = sum + Dis_Cal(graph[node[j]], graph[node[j+1]]);
        }

        if(weight>sum){
            weight = sum;
            t_end = clock();
            double time = double(t_end)/CLOCKS_PER_SEC;
            output2<< std::setprecision(2)<< std::fixed<< time<< ","<< weight<<"\n";
        }

        t_end = clock();
        duration = (double)(t_end - t_start)/CLOCKS_PER_SEC;
        k++;
    }
    output1<< weight<< "\n";
    std::vector<int>::iterator s ;
    for(s=node.begin(); s!=node.end()-1; s++){
        int w = Dis_Cal(graph[*s], graph[*(s+1)]);
        output1<< *s<< " "<< *(s+1)<< " "<< w<< "\n";
    }
    output1.close();
    output2.close();
}

void MSTapprox(std::string argv1, std::string argv2, std::string argv3)
{
    //argv1 = instance name; agrv2 = cutoff; argv3 = seed;
    clock_t t_start;
    t_start = clock();
    
    std::string input = argv1;
    srand(atoi(argv3.c_str()));
    int runtime;
    runtime = atoi(argv2.c_str());
    std::string str1, str2, str3, str4;
    str2 = argv2;
    str3 = "_MSTApprox_";
    str4 = argv3;
    
    std::size_t found = input.find_last_of("/\\");
    std::string strippedInstance = input.substr(found+1);
    strippedInstance = strippedInstance.substr(0,strippedInstance.length()-4);
    str1 = strippedInstance;

    std::string output_file = str1+str3+str2;
    
    std::vector<City> graph = createGraph(input);
    
    //Write this function to parse edges from graph file to create your graph object
    Map G = parseEdges(graph);
    
    //Run your MST function on graph G and collect as output the total weight of the MST
    G = computeMST(G);

    // Find tour based on doubled MST Graph
    tourweight(G, graph, t_start, runtime, output_file);
}
