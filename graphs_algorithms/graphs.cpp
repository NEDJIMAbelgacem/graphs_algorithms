#include "stdafx.h"
#include "graphs.h"

//using namespace graphs;

t_adjList graphs::from_adjMatrix_to_adjList(t_adjMatrix& adjMatrix, int n) {
	map<int, set<vector<int>>> m;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (adjMatrix[i][j] != -1) {
				m[i].insert(vector<int>{ j, adjMatrix[i][j] });
			}
		}
	}
	return m;
}

t_edgesList graphs::from_adjMatrix_to_edgesList(t_adjMatrix& adjMatrix, int n) {
	vector<vector<int>> v;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (adjMatrix[i][j] != -1) {
				v.push_back(vector<int>{ i, j, adjMatrix[i][j] });
			}
		}
	}
	return v;
}

t_adjMatrix graphs::from_adjList_to_adjMatrix(t_adjList& adjList, int n) {
	vector<vector<int>> m(n, vector<int>(n, -1));
	for (int i = 0; i < n; ++i) {
		for (vector<int> v : adjList[i]) {
			int j = v[0];
			int w = v[1];
			if (m[i][j]) m[i][j] = w;
			else m[i][j] = MIN(m[i][j], w);
		}
	}
	return m;
}

t_edgesList graphs::from_adjList_to_edgesList(t_adjList& adjList, int n) {
	vector<vector<int>> v;
	for (int i = 0; i < n; ++i) {
		for (vector<int> v2 : adjList[i]) {
			int j = v2[0];
			int w = v2[1];
			v.push_back(vector<int>{i, j, w});
		}
	}
	return v;
}

t_adjMatrix graphs::from_edgesList_to_adjMatrix(t_edgesList& edgesList, int n) {
	vector<vector<int>> m(n, vector<int>(n, -1));
	for (vector<int> e : edgesList) {
		int i = e[0], j = e[1], w = e[2];
		if (m[i][j] == -1) m[i][j] = w;
		else m[i][j] = MIN(m[i][j], w);
	}
	return m;
}

t_adjList graphs::from_edgesList_to_adjList(t_edgesList& edgesList, int n) {
	map<int, set<vector<int>>> m;
	for (vector<int> e : edgesList) {
		int i = e[0], j = e[1], w = e[2];
		m[i].insert(vector<int>{ j, w });
	}
	return m;
}

t_adjList graphs::transpose_graph(t_adjList& adj_list, int n) {
	t_adjList transpose_adj_list;
	for (int u = 0; u < n; ++u) transpose_adj_list[u].clear();
	for (int u = 0; u < n; ++u) {
		for (vector<int> e : adj_list[u]) {
			transpose_adj_list[e[0]].insert(vector<int>{u, e[1]});
		}
	}
	return transpose_adj_list;
}

int graphs::min_spanning_tree_Kruskal(t_edgesList& edgesList, int n, vector<int>& mst_tree) {
	vector<int> parent(n);
	mst_tree.resize(n);
	for (int i = 0; i < n; ++i) {
		mst_tree[i] = parent[i] = i;
	}
	int tree_weight = 0;
	sort(edgesList.begin(), edgesList.end(), [](vector<int> v1, vector<int> v2) -> bool { return v1[2] < v2[2]; });
	int i = 0;
	int edges_count = 0;
	while (i < edgesList.size() && edges_count < n - 1) {
		int u = edgesList[i][0], v = edgesList[i][1];
		int a = u, b = v;
		int w = edgesList[i][2];
		while (parent[a] != a) a = parent[a];
		while (parent[b] != b) b = parent[b];
		if (a != b) {
			if (mst_tree[u] != u) mst_tree[v] = u;
			else mst_tree[u] = v;
			parent[a] = b;
			tree_weight += w;
			++edges_count;
		}
		++i;
	}
	return tree_weight;
}

int graphs::min_spanning_tree_Prim(t_adjMatrix& adjMatrix, int n, vector<int>& mst_tree) {
	mst_tree.clear();
	for (int i = 0; i < n; ++i) mst_tree.push_back(i);
	int mst_weight = 0;

	vector<int> weight(n, INF);
	set<int> not_included_v;
	for (int i = 0; i < n; ++i) not_included_v.insert(i);

	weight[0] = 0;
	auto cmp = [&weight](int i, int j) -> bool {
		return weight[i] < weight[j];
	};

	for (int _ = 0; _ < n - 1; ++_) {
		int u = *min_element(not_included_v.begin(), not_included_v.end(), cmp);
		not_included_v.erase(u);
		for (int v = 0; v < n; ++v) {
			if (adjMatrix[u][v] > 0 && not_included_v.find(v) != not_included_v.end() && weight[v] > weight[u] + adjMatrix[u][v]) {
				weight[v] = weight[u] + adjMatrix[u][v];
				mst_tree[v] = u;
				mst_weight += adjMatrix[u][v];
			}
		}
	}
	return mst_weight;
}

void graphs::breadth_first_search(t_adjList& adjList, int n, int source, vector<int>& path_tree) {
	vector<bool> visited(n, false);
	path_tree.clear();
	for (int i = 0; i < n; ++i) path_tree.push_back(i);

	deque<int> q;
	q.push_back(source);
	while (q.size() != 0) {
		for (int k : q) visited[k] = true;
		int qs = q.size();
		for (int i = 0; i < qs; ++i) {
			int k = q.front();
			q.pop_front();
			for (vector<int> e : adjList[k]) {
				if (!visited[e[0]]) {
					q.push_back(e[0]);
					path_tree[e[0]] = k;
				}
			}
		}
	}
}

void graphs::depth_first_search(t_adjList& adjList, int n, int source, vector<int>& path_tree) {
	vector<bool> visited(n, false);
	path_tree.clear();
	for (int i = 0; i < n; ++i) path_tree.push_back(i);
	stack<int> st;
	st.push(source);
	while (st.size() != 0) {
		int top = st.top();
		visited[top] = true;
		bool has_neighbour = false;
		for (vector<int> e : adjList[top]) {
			if (!visited[e[0]]) {
				has_neighbour = true;
				path_tree[e[0]] = top;
				st.push(e[0]);
				break;
			}
		}
		if (!has_neighbour) st.pop();
	}
}

void graphs::shortest_path_Djikstra(t_adjList& adjList, int n, int source, vector<int>& path_tree, vector<int>& distance) {
	distance.resize(n, INF);
	distance[source] = 0;
	path_tree.clear();
	for (int i = 0; i < n; ++i) path_tree.push_back(i);

	auto cmp_distance = [](pair<int, int> i, pair<int, int> j) -> bool {
		return i.second > j.second;
	};

	priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp_distance)> pq(cmp_distance);
	distance[source] = 0;
	pq.push(pair<int, int>(source, 0));

	while (pq.size() != 0) {
		pair<int, int> p = pq.top();
		pq.pop();
		int u = p.first;
		for (vector<int> e : adjList[u]) {
			int v = e[0], w = e[1];
			if (distance[v] > distance[u] + w) {
				distance[v] = distance[u] + w;
				path_tree[v] = u;
				pq.push(pair<int, int>(v, distance[v]));
			}
		}
	}
}

bool graphs::shortest_path_Bellman_Ford(t_edgesList& edgesList, int n, int source, vector<int>& path_tree, vector<int>& distance) {
	path_tree.resize(n);
	for (int i = 0; i < n; ++i) path_tree[i] = i;
	distance.resize(n, INF);
	distance[source] = 0;
	for (int i = 0; i < n - 1; ++i) {
		for (vector<int> e : edgesList) {
			// relax edges and avoid overflows
			if (distance[e[0]] != INF && distance[e[0]] + e[2] < distance[e[1]]) {
				distance[e[1]] = distance[e[0]] + e[2];
				path_tree[e[1]] = e[0];
			}
		}
	}
	bool has_negative_cycle = false;
	for (vector<int> e : edgesList) {
		if (distance[e[0]] != INF && distance[e[0]] + e[2] < distance[e[1]]) {
			has_negative_cycle = true;
			break;
		}
	}
	return has_negative_cycle;
}

void graphs::shortest_path_Floyd_Warshal(t_edgesList& edgesList, int n, vector<vector<int>>& distances) {
	distances.resize(n, vector<int>(n, INF));
	for (vector<int> e : edgesList) distances[e[0]][e[1]] = e[2];
	for (int u = 0; u < n; ++u) distances[u][u] = 0;
	for (int u = 0; u < n; ++u) {
		for (int v = 0; v < n; ++v) {
			for (int i = 0; i < n; ++i) {
				if (distances[u][i] + distances[i][v] < distances[u][v]) distances[u][v] = distances[u][i] + distances[i][v];
			}
		}
	}
}

void graphs::strongly_connected_components_Kusaraju(t_adjList& adjList, int n, vector<int>& graph_partition) {
	graph_partition.resize(n);
	t_adjList transpose_adjList = graphs::transpose_graph(adjList, n);
	vector<bool> assigned(n, false);
	int nb_visited = 0;
	int min_subset = 0;
	int r = 0;
	while (r != n) {
		vector<int> out_visited(n, false);
		vector<int> in_visited(n, false);
		deque<int> visit_q;
		visit_q.push_back(r);
		while (!visit_q.empty()) {
			int q_len = visit_q.size();
			for (int i = 0; i < q_len; ++i) {
				int u = visit_q.front();
				visit_q.pop_front();
				visit_q.push_back(u);
				out_visited[u] = true;
			}
			while (q_len--) {
				int u = visit_q.front();
				visit_q.pop_front();
				for (vector<int> e : adjList[u]) {
					if (!out_visited[e[0]]) visit_q.push_back(e[0]);
				}
			}
		}
		visit_q.push_back(r);
		while (!visit_q.empty()) {
			int q_len = visit_q.size();
			for (int i = 0; i < q_len; ++i) {
				int u = visit_q.front();
				visit_q.pop_front();
				visit_q.push_back(u);
				in_visited[u] = true;
			}
			while (q_len--) {
				int u = visit_q.front();
				visit_q.pop_front();
				for (vector<int> e : transpose_adjList[u]) {
					if (!in_visited[e[0]]) visit_q.push_back(e[0]);
				}
			}
		}
		r = n;
		for (int i = 0; i < n; ++i) {
			if (out_visited[i] && in_visited[i]) {
				graph_partition[i] = min_subset;
				assigned[i] = true;
			}
			else if (!assigned[i] && r == n) r = i;
		}
		min_subset++;
	}
}

void graphs::__tarjan_depth_first_search(t_adjList& adjList, int n, int u, vector<bool>& in_stack, stack<int>& dfs_stack, vector<int>& index, vector<int>& low_index, int& visit_t, vector<int>& graph_partition) {
	dfs_stack.push(u);
	in_stack[u] = true;
	low_index[u] = index[u] = visit_t;
	visit_t++;
	for (vector<int> e : adjList[u]) {
		int v = e[0];
		if (index[v] == -1) {
			__tarjan_depth_first_search(adjList, n, v, in_stack, dfs_stack, index, low_index, visit_t, graph_partition);
			low_index[u] = MIN(low_index[u], low_index[v]);
		}
		else if (in_stack[v]) {
			low_index[u] = MIN(low_index[u], index[v]);
		}
	}

	int w;
	if (low_index[u] == index[u])
	{
		while ((w = dfs_stack.top()) != u)
		{
			graph_partition[w] = u;
			dfs_stack.pop();
			in_stack[w] = false;
		}
		dfs_stack.pop();
		in_stack[w] = false;
	}
}

void graphs::strongly_connected_components_Tarjan(t_adjList& adjList, int n, vector<int>& graph_partition) {
	graph_partition.resize(n);
	for (int i = 0; i < n; ++i) graph_partition[i] = i;
	vector<bool> in_stack(n, false);
	stack<int> dfs_stack;
	vector<int> index(n, -1);
	vector<int> low_index(n, -1);
	int visit_t = 0;
	for (int u = 0; u < n; ++u) {
		if (index[u] == -1) {
			__tarjan_depth_first_search(adjList, n, u, in_stack, dfs_stack, index, low_index, visit_t, graph_partition);
		}
	}
}

int graphs::coloring_welsh_powell(t_adjList& adjList, int nb_vertices, vector<int>& graph_coloring) {
	graph_coloring.resize(nb_vertices, -1);
	vector<int> vertices(nb_vertices);
	vector<int> degree(nb_vertices);
	for (int i = 0; i < nb_vertices; ++i) {
		vertices[i] = i;
		degree[i] = adjList[i].size();
	}
	sort(vertices.begin(), vertices.end(), [degree](int i, int j) -> bool {
		return degree[i] > degree[j];
	});
	int max_color = -1;
	for (int v : vertices) {
		// find the minimum unused color
		set<int> color_set;
		for (int i = max_color + 1; i >= 0; --i) color_set.insert(i);
		for (vector<int> e : adjList[v]) {
			if (color_set.find(graph_coloring[e[0]]) != color_set.end()) color_set.erase(graph_coloring[e[0]]);
		}
		int color = *min_element(color_set.begin(), color_set.end());
		if (color > max_color) max_color = color;
		graph_coloring[v] = color;
	}
	return max_color + 1;
}

int graphs::coloring_dsatur(t_adjList& adjList, int nb_vertices, vector<int>& graph_coloring) {
	graph_coloring.resize(nb_vertices, -1);
	vector<int> vertice_degree(nb_vertices, -1);
	for (int i = 0; i < nb_vertices; ++i) vertice_degree[i] = adjList[i].size();
	int current_vertice = 0;
	for (int i = 1; i < nb_vertices; ++i) {
		if (vertice_degree[current_vertice] < vertice_degree[i]) current_vertice = i;
	}

	auto calculate_chromatic_degree = [&adjList, &graph_coloring](int u) -> int {
		set<int> colors;
		for (vector<int> e : adjList[u]) colors.insert(graph_coloring[e[0]]);
		return colors.size();
	};
	graph_coloring[current_vertice] = 0;
	int max_color = 0;
	for (int _ = 0; _ < nb_vertices - 1; ++_) {
		current_vertice = -1;
		int vertice_chromatic_degree = -1;
		for (int i = 0; i < nb_vertices; ++i) {
			// already colored
			if (graph_coloring[i] != -1) continue;
			// calculate the number of neighbouring colors
			int chromatic_degree = calculate_chromatic_degree(i);
			if (vertice_chromatic_degree < chromatic_degree || (vertice_chromatic_degree == chromatic_degree && vertice_degree[i] > vertice_degree[current_vertice])) {
				current_vertice = i;
				vertice_chromatic_degree = chromatic_degree;
			}
		}

		// find the minimum unused color
		set<int> color_set;
		for (int i = max_color + 1; i >= 0; --i) color_set.insert(i);
		for (vector<int> e : adjList[current_vertice]) {
			if (color_set.find(graph_coloring[e[0]]) != color_set.end()) color_set.erase(graph_coloring[e[0]]);
		}
		int color = *min_element(color_set.begin(), color_set.end());
		graph_coloring[current_vertice] = color;
		if (color > max_color) max_color = color;
	}
	return max_color + 1;
}

bool graphs::is_flow_network(t_adjList& adjList, int nb_vertices) {
	// pair<int, int>::first : outdegree
	// pair<int, int>::second : indegree
	map<int, pair<int, int>> degrees;
	for (int i = 0; i < nb_vertices; ++i) degrees[i] = make_pair(0, 0);
	for (const pair<int, set<vector<int>>>& p : adjList) {
		int u = p.first;
		for (vector<int> vect : p.second) {
			int v = vect[0];
			degrees[u].first += 1;
			degrees[v].second += 1;
		}
	}
	int nb_sinks = 0, nb_sources = 0;
	for (pair<int, pair<int, int>> p : degrees) {
		pair<int, int>& p2 = p.second;
		if (p2.first > 0 && p2.second > 0) continue;
		if (p2.first == 0) nb_sources++;
		if (p2.second == 0) nb_sinks++;
	}
	return nb_sinks == 1 && nb_sources == 1;
}

bool graphs::is_compapatible_flow_network(t_adjList& adjList, int nb_vertices) {
	// pair<int, int>::first : outdegree
	// pair<int, int>::second : indegree
	map<int, pair<int, int>> degrees;
	map<int, int> node_value;
	for (int i = 0; i < nb_vertices; ++i) {
		degrees[i] = make_pair(0, 0);
		node_value[i] = 0;
	}
	for (const pair<int, set<vector<int>>>& p : adjList) {
		int u = p.first;
		for (vector<int> vect : p.second) {
			int v = vect[0];
			int w = vect[1];
			degrees[u].first += 1;
			degrees[v].second += 1;
			node_value[u] -= w;
			node_value[v] += w;
		}
	}
	int nb_sinks = 0, nb_sources = 0;
	int source = -1, sink = -1;
	for (pair<int, pair<int, int>> p : degrees) {
		pair<int, int>& p2 = p.second;
		if (p2.first > 0 && p2.second > 0) continue;
		if (p2.first == 0) {
			nb_sources++;
			source = p.first;
		}
		if (p2.second == 0) {
			nb_sinks++;
			sink = p.first;
		}
	}
	if (nb_sinks != 1 || nb_sources != 1) return false;
	for (int i = 0; i < nb_vertices; ++i) {
		if (i == sink || i == source) continue;
		if (node_value[i] != 0) return false;
	}
	return true;
}

string graphs::convert_to_dot_language(t_adjMatrix& adjMatrix, int nb_vertices, GType type) {
	string s = "";
	switch (type)
	{
	case Graph:
		s += "graph {\n";
		for (int i = 0; i < nb_vertices; ++i) {
			for (int j = i + 1; j < nb_vertices; ++j) {
				int w = adjMatrix[i][j];
				if (w == -1) continue;
				s += "\t" + to_string(i) + " -- " + to_string(j) + "; \n";
			}
		}
		s += "}";
		break;
	case DiGraph:
		s += "digraph {\n";
		for (int i = 0; i < nb_vertices; ++i) {
			for (int j = 0; j < nb_vertices; ++j) {
				int w = adjMatrix[i][j];
				if (w == -1) continue;
				s += to_string(i) + " -> " + to_string(j) + "; \n";
			}
		}
		s += "}";
		break;
	case WGraph:
		s += "graph {\n";
		for (int i = 0; i < nb_vertices; ++i) {
			for (int j = i + 1; j < nb_vertices; ++j) {
				int w = adjMatrix[i][j];
				if (w == -1) continue;
				s += to_string(i) + " -- " + to_string(j) + "[label=" + to_string(w) + "]" + "; \n";
			}
		}
		s += "}";
		break;
	case WDiGraph:
		s += "digraph {\n";
		for (int i = 0; i < nb_vertices; ++i) {
			for (int j = i + 1; j < nb_vertices; ++j) {
				int w = adjMatrix[i][j];
				if (w == -1) continue;
				s += to_string(i) + " -> " + to_string(j) + "[label=" + to_string(w) + "]" + "; \n";
			}
		}
		s += "}";
		break;
	}
	return s;
}

void graphs::render_dot_formatted_graph(string dot, string file_name) {
	filebuf fb;
	fb.open(file_name + ".txt", ios::out);
	ostream os(&fb);
	os << dot << endl;
	fb.close();
	string cmd = "dot -Tpng " + file_name + ".txt" + " -o " + file_name + ".png";
	system(cmd.data());
}