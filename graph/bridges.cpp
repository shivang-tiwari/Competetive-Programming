// Bridge - An edge whose removal increases the number of connected components in the graph
// Graph is undirected
vector<pair<int,int>> find_bridges(vector<vector<int>> &adj){
	int n = adj.size();
	vector<bool> visited;
	vector<int> tin, low;
	int timer;
	timer = 0;
	visited.assign(n, false);
	tin.assign(n, -1);
	low.assign(n, -1);
	vector<pair<int,int>> bridges;
	function<void(int,int)> dfs = [&](int v, int p){
		visited[v] = true;
		tin[v] = low[v] = timer++;
		for (int to : adj[v]) {
			if (to == p) continue;
			if (visited[to]){
				low[v] = min(low[v], tin[to]);
			} 
			else{
				dfs(to, v);
				low[v] = min(low[v], low[to]);
				if (low[to] > tin[v]){
					// {v - to} is a bridge
					bridges.push_back({v,to});
				}
			}
		}
	};
	for(int i = 0; i < n; ++i){
		if (!visited[i])
			dfs(i,-1);
	}
	return bridges;
}
