#pragma once
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <queue>
#include <stack>
#include <limits>

#define MIN(x, y) ((x) > (y) ? (y) : (x))
#define MAX(x, y) ((x) < (y) ? (y) : (x))
#define INF (numeric_limits<int>::max() >> 2)

using namespace std;
using t_adjList = map<int, set<vector<int>>>;
using t_adjMatrix = vector<vector<int>>;
using t_edgesList = vector<vector<int>>;

namespace graphs
{
	// graph representations conversion
	t_adjList from_adjMatrix_to_adjList(t_adjMatrix& adjMatrix, int n);
	t_edgesList from_adjMatrix_to_edgesList(t_adjMatrix& adjMatrix, int n);
	t_adjMatrix from_adjList_to_adjMatrix(t_adjList& adjList, int n);
	t_edgesList from_adjList_to_edgesList(t_adjList& adjList, int n);
	t_adjMatrix from_edgesList_to_adjMatrix(t_edgesList& edgesList, int n);
	t_adjList from_edgesList_to_adjList(t_edgesList& edgesList, int n);
	t_adjList transpose_graph(t_adjList& edgesList, int n);
	// minimum spanning tree algorithms
	int min_spanning_tree_Kruskal(t_edgesList& edgesList, int n, vector<int>& mst_tree);
	int min_spanning_tree_Prim(t_adjMatrix& adjMatrix, int n, vector<int>& mst_tree);
	// graph traversal algorithms
	void depth_first_search(t_adjList& adjList, int n, int source, vector<int>& path_tree);
	void breadth_first_search(t_adjList& adjList, int n, int source, vector<int>& path_tree);
	// shortest paths algorithms
	void shortest_path_Djikstra(t_adjList& adjList, int n, int source, vector<int>& path_tree, vector<int>& distances);
	bool shortest_path_Bellman_Ford(t_edgesList& edgesList, int n, int source, vector<int>& path_tree, vector<int>& distances);
	void shortest_path_Floyd_Warshal(t_edgesList& edgesList, int n, vector<vector<int>>& distances);
	// strongly connected components algorithms
	void strongly_connected_components_Kusaraju(t_adjList& adjList, int n, vector<int>& graph_partition);
	void __tarjan_depth_first_search(t_adjList& adjList, int n, int u, vector<bool>& in_stack, stack<int>& dfs_stack, vector<int>& index, vector<int>& low_index, int& visit_t, vector<int>& graph_partition);
	void strongly_connected_components_Tarjan(t_adjList& adjList, int n, vector<int>& graph_partition);
	// graph coloring
	int coloring_welsh_powell(t_adjList& adjList, int nb_vertices, vector<int>& graph_coloring);
	int coloring_dsatur(t_adjList& adjList, int nb_vertices, vector<int>& graph_coloring);
};

