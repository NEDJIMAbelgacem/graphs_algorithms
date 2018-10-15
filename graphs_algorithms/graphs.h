#pragma once
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <queue>
#include <stack>

#define MIN(x, y) ((x) > (y) ? (y) : (x))
#define MAX(x, y) ((x) < (y) ? (y) : (x))
#define INF 1 << 30

using namespace std;
using t_adjList = map<int, set<vector<int>>>;
using t_adjMatrix = vector<vector<int>>;
using t_edgesList = vector<vector<int>>;

class graphs
{
public:
	// graph input
	static t_adjList input_graph();
	// graph representations conversion
	static t_adjList from_adjMatrix_to_adjList(t_adjMatrix& adjMatrix, int n);
	static t_edgesList from_adjMatrix_to_edgesList(t_adjMatrix& adjMatrix, int n);
	static t_adjMatrix from_adjList_to_adjMatrix(t_adjList& adjList, int n);
	static t_edgesList from_adjList_to_edgesList(t_adjList& adjList, int n);
	static t_adjMatrix from_edgesList_to_adjMatrix(t_edgesList& edgesList, int n);
	static t_adjList from_edgesList_to_adjList(t_edgesList& edgesList, int n);
	// minimum spanning tree algorithms
	int min_spanning_tree_Kruskal(t_edgesList& edgesList, int n, vector<int>& mst_tree);
	int min_spanning_tree_Prim(t_adjList& adjList, int n, vector<int>& mst_tree);
	// shortest paths algorithms
	static void depth_first_search(t_adjList& adjList, int n, int source, vector<int>& path_tree);
	static void breadth_first_search(t_adjList& adjList, int n, int source, vector<int>& path_tree);
	static void shortest_path_djikstra(t_adjList& adjList, int n, int source, vector<int>& path_tree, vector<int>& distances);
	static void shortest_path_bellman_ford(t_adjList& adjList, int n, int source, vector<int>& path_tree, vector<int>& distances);
};

