// graphs_algorithms.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "graphs.h"

using namespace std;

void depth_and_breadth_searches_test(t_adjList& adj_list) {
	vector<int> dfs_tree, bfs_tree;
	int s, n = adj_list.size();
	cout << "source : ";
	cin >> s;
	graphs::breadth_first_search(adj_list, n, s, bfs_tree);
	graphs::depth_first_search(adj_list, n, s, dfs_tree);
	cout << "breadth first search tree : " << endl;
	for (int i = 0; i < n; ++i) cout << bfs_tree[i] << " ";
	cout << endl;
	cout << "depth first search tree : " << endl;
	for (int i = 0; i < n; ++i) cout << dfs_tree[i] << " ";
	cout << endl;
}

int main()
{
	t_adjList adj_list = graphs::input_graph();
	
    return 0;
}

