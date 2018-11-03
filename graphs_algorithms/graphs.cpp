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
	mst_tree.clear();
	for (int i = 0; i < n; ++i) mst_tree.push_back(i);
	int tree_weight = 0;
	sort(edgesList.begin(), edgesList.end(), [](vector<int> v1, vector<int> v2) -> bool {return v1[2] < v2[2]; });
	int i = 0;
	int edges_count = 0;
	while (i < edgesList.size() && edges_count < n - 1) {
		int a = edgesList[i][0], b = edgesList[i][1];
		while (mst_tree[a] != a) a = mst_tree[a];
		while (mst_tree[b] != b) b = mst_tree[b];
		int w = edgesList[i][2];
		if (a != b) {
			mst_tree[a] = b;
			tree_weight += w;
			++edges_count;
		}
		++i;
	}
	return tree_weight;
}

int graphs::min_spanning_tree_Prim(t_adjList& adjList, int n, vector<int>& mst_tree) {
	mst_tree.clear();
	for (int i = 0; i < n; ++i) mst_tree.push_back(i);
	int mst_weight = 0;

	vector<int> included(n, false);
	auto cmp_edge = [](pair<int, int> i, pair<int, int> j) -> bool {
		return i.second > j.second;
	};
	priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp_edge)> q(cmp_edge);
	q.push(pair<int, int>(0, 0));
	while (q.size() != 0) {
		pair<int, int> p = q.top();
		int u = p.first;
		q.pop();
		mst_weight += p.second;
		for (vector<int> e : adjList[u]) {
			int v = e[0], w = e[1];
			if (included[v]) continue;
			q.push(pair<int, int>(v, w));
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
	path_tree.clear();
	for (int i = 0; i < n; ++i) path_tree.push_back(i);
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