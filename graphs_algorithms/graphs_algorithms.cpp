// graphs_algorithms.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "graphs.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Important notes:
// -> this function doest garantee that ill formated files will be detected, follow file syntax carefully
// -> there is no mecanism to avoid duplicate edges that can be included in adj_list
bool read_graph_from_file(ifstream& f, int& nb_vertices, int& nb_edges, t_adjList& adj_list, t_adjMatrix& adj_matrix, t_edgesList& edges_list, graphs::GType& type) {
	const char* graph_types[] = { "undirected_unweighted", "undirected_weighted", "directed_unweighted", "directed_weighted" };
	string graph_type;
	f >> graph_type;
	bool valid_type = false;
	for (int i = 0; i < 4; ++i) {
		if (graph_type == graph_types[i]) {
			valid_type = true;
			break;
		}
	}
	if (!valid_type) {
		cerr << "ERROR : undefined graph type *" << graph_type << "*" << endl;
		return false;
	}
	if (graph_type == "undirected_unweighted") type = graphs::Graph;
	else if (graph_type == "undirected_weighted") type = graphs::WGraph;
	else if (graph_type == "directed_unweighted") type = graphs::DiGraph;
	else type = graphs::WDiGraph;
	f >> nb_vertices;
	f >> nb_edges;
	adj_list.clear();
	edges_list.clear();
	adj_matrix.resize(nb_vertices, vector<int>(nb_vertices, -1));
	for (int i = 0; i < nb_vertices; ++i) {
		adj_list[i] = set<vector<int>>();
	}
	for (int i = 0; i < nb_edges; ++i) {
		int u, v, w;
		if (graph_type == "undirected_unweighted") {
			f >> u >> v;
			edges_list.push_back(vector<int>{u, v, 1});
			// I needed to do this to make edges bidirectional for algorithms that uses adjacency lists
			edges_list.push_back(vector<int>{v, u, 1});
			adj_matrix[u][v] = adj_matrix[v][u] = 1;
			adj_list[u].insert(vector<int>{ v, 1 });
			adj_list[v].insert(vector<int>{ u, 1 });
		}
		else if (graph_type == "undirected_weighted") {
			f >> u >> v >> w;
			edges_list.push_back(vector<int>{u, v, w});
			if (adj_matrix[u][v] == -1) adj_matrix[u][v] = adj_matrix[v][u] = w;
			// We take the minimum parallel edge though it could be the maximum if we were
			// iterrested in the maximum paths instead of minimums
			else adj_matrix[u][v] = adj_matrix[v][u] = MIN(adj_matrix[v][u], w);
			adj_list[u].insert(vector<int>{ v, w });
			adj_list[v].insert(vector<int>{ u, w });
		}
		else if (graph_type == "directed_unweighted") {
			f >> u >> v;
			edges_list.push_back(vector<int>{u, v, 1});
			if (adj_matrix[u][v] == -1) adj_matrix[u][v] = 1;
			adj_list[u].insert(vector<int>{ v, 1 });
		}
		else {
			f >> u >> v >> w;
			edges_list.push_back(vector<int>{u, v, w});
			if (adj_matrix[u][v] == -1) adj_matrix[u][v] = w;
			// We take the minimum parallel edge though it could be the maximum if we were
			// iterrested in the maximum paths instead of minimums
			else adj_matrix[u][v] = MIN(adj_matrix[u][v], w);
			adj_list[u].insert(vector<int>{ v, w });
		}
	}
	return true;
}

void depth_first_searches_test(int source, int nb_vertices, t_adjList& adj_list) {
	vector<int> dfs_tree;
	graphs::depth_first_search(adj_list, nb_vertices, source, dfs_tree);
	cout << " --- Depth first search algorithm result : ---" << endl;
	cout << "Depth first search tree in vector representation : " << endl;
	cout << "Vertice : ";
	for (int i = 0; i < nb_vertices; ++i) cout << i << " ";
	cout << endl;
	cout << "Parent  : ";
	for (int i = 0; i < nb_vertices; ++i) cout << dfs_tree[i] << " ";
	cout << endl;
	cout << " ---------------------------------------------" << endl;
}

void breadth_first_searches_test(int source, int nb_vertices, t_adjList& adj_list) {
	vector<int> bfs_tree;
	graphs::breadth_first_search(adj_list, nb_vertices, source, bfs_tree);
	cout << " --- Breadth first search algorithm result : ---" << endl;
	cout << "Breadth first search tree in vector representation : " << endl;
	cout << "Vertice : ";
	for (int i = 0; i < nb_vertices; ++i) cout << i << " ";
	cout << endl;
	cout << "Parent : ";
	for (int i = 0; i < nb_vertices; ++i) cout << bfs_tree[i] << " ";
	cout << endl;
	cout << " ---------------------------------------------" << endl;
}

// Warning :
// -> this function will loop infinitely if the graph contains negative weight edges
void djikstra_shortest_paths_test(int source, int nb_vertices, t_adjList& adj_list) {
	vector<int> distances, path_tree;
	graphs::shortest_path_Djikstra(adj_list, nb_vertices, source, path_tree, distances);
	cout << " --- Djikstra algorithm result : ---" << endl;
	cout << "shortest distances from vertice " << source << " to other edges : " << endl;
	cout << "Vertice  : ";
	for (int i = 0; i < nb_vertices; ++i) cout << i << " ";
	cout << endl;
	cout << "Distance : ";
	for (int d : distances) {
		if (d < INF) cout << d << " ";
		else cout << "x ";
	}
	cout << endl;
	/*cout << "paths tree : " << endl;
	for (int p : path_tree) cout << p << " ";
	cout << endl;*/
	cout << " ---------------------------------------------" << endl;
}

void bellman_ford_paths_test(int source, int nb_vertices, t_edgesList edgesList) {
	vector<int> path_tree;
	vector<int> distance;
	bool has_negative_cycle = graphs::shortest_path_Bellman_Ford(edgesList, nb_vertices, source, path_tree, distance);
	cout << " --- Bellman-Ford algorithm result : ---" << endl;
	cout << "has negative cycle ? : " << (has_negative_cycle ? "true" : "false") << endl;
	cout << "shortest distances from vertice " << source << " to other edges : " << endl;
	cout << "Vertice  : ";
	for (int i = 0; i < nb_vertices; ++i) cout << i << " ";
	cout << endl;
	cout << "Distance : ";
	for (int i : distance) {
		if (i < INF) cout << i << " ";
		else cout << "x ";
	}
	cout << endl;
	/*cout << "Path tree : " << endl;
	for (int i : path_tree) cout << i << " ";
	cout << endl;*/
	cout << " ---------------------------------------------" << endl;
}

void floyd_warshal_paths_test(int nb_vertices, t_edgesList edgesList) {
	vector<vector<int>> distances;
	graphs::shortest_path_Floyd_Warshal(edgesList, nb_vertices, distances);
	cout << " --- Floyd-Warshal algorithm result : ---" << endl;
	cout << "Shortest paths matrix : " << endl;
	cout << "    ";
	for (int i = 0; i < nb_vertices; ++i) cout << i << " ";
	cout << endl;
	for (int u = 0; u < nb_vertices; ++u) {
		cout << u << " : ";
		for (int v = 0; v < nb_vertices; ++v) {
			if (distances[u][v] != INF) cout << distances[u][v] << " ";
			else cout << "x ";
		}
		cout << endl;
	}
	cout << " ---------------------------------------------" << endl;
}

void kruskal_MST_test(int nb_vertices, t_edgesList edgesList) {
	vector<int> mst;
	int tree_weight = graphs::min_spanning_tree_Kruskal(edgesList, nb_vertices, mst);
	cout << " --- Kruskal algorithm result : ---" << endl;
	cout << "Minimum spanning tree weight : " << tree_weight << endl;
	cout << "Minimum spanning tree in vector representation : " << endl;
	cout << "Vertice : ";
	for (int i = 0; i < nb_vertices; ++i) cout << i << " ";
	cout << endl;
	cout << "Parent  : ";
	for (int i : mst) cout << i << " ";
	cout << endl;
	cout << " ---------------------------------------------" << endl;
}

void prim_MST_test(t_adjMatrix& adjMatrix, int nb_vertices) {
	vector<int> mst;
	int tree_weight = graphs::min_spanning_tree_Prim(adjMatrix, nb_vertices, mst);
	cout << " --- Prim algorithm result : ---" << endl;
	cout << "Minimum spanning tree weight : " << tree_weight << endl;
	cout << "Minimum spanning tree in vector representation : " << endl;
	cout << "Vertice : ";
	for (int i = 0; i < nb_vertices; ++i) cout << i << " ";
	cout << endl;
	cout << "Parent  : ";
	for (int i : mst) cout << i << " ";
	cout << endl;
	cout << " ---------------------------------------------" << endl;
}

void kusaraju_SCC_test(t_adjList& adj_list, int nb_vertices) {
	vector<int> graph_partition;
	graphs::strongly_connected_components_Kusaraju(adj_list, nb_vertices, graph_partition);
	cout << " --- Kusaraju algorithm result : ---" << endl;
	cout << "Graph partition indexing bijection : " << endl;
	cout << "Vertice   : ";
	for (int i = 0; i < nb_vertices; ++i) cout << i << " ";
	cout << endl;
	cout << "Component : ";
	for (int i = 0; i < nb_vertices; ++i) cout << graph_partition[i] << " ";
	cout << endl;
	cout << " ---------------------------------------------" << endl;
}

void tarjan_SCC_test(t_adjList& adj_list, int nb_vertices) {
	vector<int> graph_partition;
	graphs::strongly_connected_components_Tarjan(adj_list, nb_vertices, graph_partition);
	cout << " --- Tarjan algorithm result : ---" << endl;
	cout << "Graph partition indexing bijection : " << endl;
	cout << "Vertice   : ";
	for (int i = 0; i < nb_vertices; ++i) cout << i << " ";
	cout << endl;
	cout << "Component : ";
	for (int i = 0; i < nb_vertices; ++i) cout << graph_partition[i] << " ";
	cout << endl;
	cout << " ---------------------------------------------" << endl;
}

void welsh_powel_coloring_test(t_adjList& adj_list, int nb_vertices) {
	vector<int> graph_coloring;
	int nb_colors = graphs::coloring_welsh_powell(adj_list, nb_vertices, graph_coloring);
	cout << " --- Welsh-Powell algorithm result : ---" << endl;
	cout << "Maximum number of color : " << nb_colors << endl;
	cout << "Graph colors indexing bijection : " << endl;
	cout << "Vertice : ";
	for (int i = 0; i < nb_vertices; ++i) cout << i << " ";
	cout << endl;
	cout << "Color   : ";
	for (int i = 0; i < nb_vertices; ++i) cout << graph_coloring[i] << " ";
	cout << endl;
	cout << " ---------------------------------------------" << endl;
}

void dsatur_coloring_test(t_adjList& adj_list, int nb_vertices) {
	vector<int> graph_coloring;
	int nb_colors = graphs::coloring_dsatur(adj_list, nb_vertices, graph_coloring);
	cout << " --- DSATUR algorithm result : ---" << endl;
	cout << "Maximum number of color : " << nb_colors << endl;
	cout << "Graph colors indexing bijection : " << endl;
	cout << "Vertice : ";
	for (int i = 0; i < nb_vertices; ++i) cout << i << " ";
	cout << endl;
	cout << "Color   : ";
	for (int i = 0; i < nb_vertices; ++i) cout << graph_coloring[i] << " ";
	cout << endl;
	cout << " ---------------------------------------------" << endl;
}

int main(int argc, char *argv[])
{
	const char* graph_file_name;
	if (argc <= 1) {
		cerr << "ERROR : No graph file provided" << endl;
		return 1;
	}
	else graph_file_name = argv[1];
	ifstream f(graph_file_name, ios_base::in);
	int nb_vertices;
	int nb_edges;
	t_adjList adj_list;
	t_adjMatrix adj_matrix;
	t_edgesList edges_list;
	graphs::GType type;
	bool valid_graph = read_graph_from_file(f, nb_vertices, nb_edges, adj_list, adj_matrix, edges_list, type);
	if (!valid_graph) {
		cerr << "ERROR : Ill formatted graph data" << endl;
		return 1;
	}
	cout << " --------------------------------------------" << endl;
	cout << " --- collection of some graphs algorithms ---" << endl;
	cout << " --------------------------------------------" << endl;
	while (true) {
		string choice;
		cout << "Choose one from the following algorithms to perform : " << endl;
		cout << "1 -> Depth first search" << endl;
		cout << "2 -> Breadth first search" << endl;
		cout << "3 -> Djikstra single source shortest paths" << endl;
		cout << "4 -> Bellman-Ford single source shortest paths" << endl;
		cout << "5 -> Floyd-Warshal all pairs shortest paths" << endl;
		cout << "6 -> Prim's minimum spanning tree" << endl;
		cout << "7 -> Kruskal's minimus spanning tree" << endl;
		cout << "8 -> Kusaraju's strongly connected components" << endl;
		cout << "9 -> Tarjan's strongly connected components" << endl;
		cout << "10 -> Welsh-Powell graph coloring" << endl;
		cout << "11 -> DSATUR graph coloring" << endl;
		cout << "12 -> Test if the graph is flow network (has source & sink)" << endl;
		cout << "13 -> Test if the graph is compatible flow network (Kirchhoff law compliant)" << endl;
		cout << "14 -> convert the graph to dot language (graphviz)" << endl;
		cout << "15 -> render the graph in .png format" << endl;
		cout << "x -> exit" << endl;
		cout << "  >>> ";
		cin >> choice;
		cout << endl;
		if (choice == "x") break;
		int source;
		// Depth first search
		if (choice == "1") {
			cout << "Input source vertice : ";
			cin >> source;
			depth_first_searches_test(source, nb_vertices, adj_list);
		}
		// Breadth first search
		else if (choice == "2") {
			cout << "Input source vertice : ";
			cin >> source;
			breadth_first_searches_test(source, nb_vertices, adj_list);
		}
		// Djikstra single source shortest paths
		else if (choice == "3") {
			cout << "Input source vertice : ";
			cin >> source;
			djikstra_shortest_paths_test(source, nb_vertices, adj_list);
		}
		// Bellman-Ford single source shortest paths
		else if (choice == "4") {
			cout << "Input source vertice : ";
			cin >> source;
			bellman_ford_paths_test(source, nb_vertices, edges_list);
		}
		// Floyd-Warshal all pairs shortest paths
		else if (choice == "5") {
			floyd_warshal_paths_test(nb_vertices, edges_list);
		}
		// Prim's minimum spanning tree
		else if (choice == "6") {
			prim_MST_test(adj_matrix, nb_vertices);
		}
		// Kruskal's minimus spanning tree
		else if (choice == "7") {
			kruskal_MST_test(nb_vertices, edges_list);
		}
		// Kusaraju's strongly connected components
		else if (choice == "8") {
			kusaraju_SCC_test(adj_list, nb_vertices);
		}
		// Tarjan's strongly connected components
		else if (choice == "9") {
			tarjan_SCC_test(adj_list, nb_vertices);
		}
		// Welsh-Powell graph coloring
		else if (choice == "10") {
			welsh_powel_coloring_test(adj_list, nb_vertices);
		}
		// DSATUR graph coloring 
		else if (choice == "11") {
			dsatur_coloring_test(adj_list, nb_vertices);
		}
		else if (choice == "12") {
			cout << "is flow network ? : " << (graphs::is_flow_network(adj_list, nb_vertices) ? "TRUE" : "FALSE") << endl;
		}
		else if (choice == "13") {
			cout << "is compatible flow network ? : " << (graphs::is_compapatible_flow_network(adj_list, nb_vertices) ? "TRUE" : "FALSE") << endl;
		}
		else if (choice == "14") {
			string dot = graphs::convert_to_dot_language(adj_matrix, nb_vertices, type);
			cout << "-> dot language translation : " << endl;
			cout << dot << endl;
		}
		else if (choice == "15") {
			string graph_name;
			cout << "input graph name : ";
			cin >> graph_name;
			string dot = graphs::convert_to_dot_language(adj_matrix, nb_vertices, type);
			graphs::render_dot_formatted_graph(dot, graph_name);
			cout << "the graph is rendered to .\\" + graph_name + ".png" << endl;
		}
		else break;
	}
	
	return 0;
}
