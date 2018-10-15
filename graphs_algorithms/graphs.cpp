#include "stdafx.h"
#include "graphs.h"

t_adjList graphs::input_graph() {
	int nv, ne;
	cout << "vertice count : ";
	cin >> nv;
	cout << "edges count : ";
	cin >> ne;
	cout << "is the graph undirected ? [Y / n] : ";
	char c;
	cin >> c;
	t_adjList adj_lst;
	for (int i = 0; i < ne; ++i) adj_lst[i];
	cout << "input the edges with their respective weight : " << endl;
	for (int i = 0; i < ne; ++i) {
		int a, b, w;
		cin >> a >> b >> w;
		adj_lst[a].insert(vector<int>{ b, w });
		if (c == 'y' || c == 'Y') adj_lst[b].insert(vector<int>{ a, w });
	}
	return adj_lst;
}

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
		w = edgesList[i][2];
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

void graphs::shortest_path_djikstra(t_adjList& adjList, int n, int source, vector<int>& path_tree, vector<int>& distance) {
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

void graphs::shortest_path_bellman_ford(t_adjList& adjList, int n, int source, vector<int>& path_tree, vector<int>& distance) {
	distance.resize(n, INF);
	path_tree.clear();
	for (int i = 0; i < n; ++i) path_tree.push_back(i);
	for (int u = 0; u < n; ++u) {
		for (vector<int> e : adjList[u]) {
			if (distance[u] + e[1] < distance[e[0]]) {
				distance[e[0]] = distance[u] + e[1];
				path_tree[e[0]] = u;
			}
		}
	}
}
