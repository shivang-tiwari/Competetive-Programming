// Articulation Point - A vertex whose removal increases the number of connected components in the graph
// Graph is undirected
vector<int> find_cutpoints(const vector<vector<int>> &adj) {
	int n = adj.size();
	vector<bool> visited;
	vector<int> tin, low;
	int timer;
	timer = 0;
	visited.assign(n, false);
	tin.assign(n, -1);
	low.assign(n, -1);
	vector<int> cutpoints;
	function<void(int,int)> dfs = [&](int v, int p){
		visited[v] = true;
		tin[v] = low[v] = timer++;
		int children=0;
		for (int to : adj[v]) {
			if (to == p) continue;
			if (visited[to]) {
				low[v] = min(low[v], tin[to]);
			} else {
				dfs(to, v);
				low[v] = min(low[v], low[to]);
				if (low[to] >= tin[v] && p!=-1){
					// v is an articulation point
					cutpoints.push_back(v);
					
				}
				++children;
			}
		}
		if(p == -1 && children > 1){
			// v is an articulation point
			cutpoints.push_back(v);
		}
	};
	for (int i = 0; i < n; ++i) {
		if (!visited[i])
			dfs(i,-1);
	}
	return cutpoints;
}
