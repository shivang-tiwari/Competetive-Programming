struct LCA{
	int n, l;
	vector<vector<int>> adj;

	int timer;
	vector<int> tin, tout;
	vector<vector<int>> up;

	void dfs(int v, int p){
		tin[v] = ++timer;
		up[v][0] = p;
		for (int i = 1; i <= l; ++i){
			up[v][i] = up[up[v][i-1]][i-1];
		}

		for(int u : adj[v]){
			if(u != p){
				dfs(u, v);
			}
		}

		tout[v] = ++timer;
	}

	bool anc(int u, int v){
		return tin[u] <= tin[v] && tout[u] >= tout[v];
	}

	int lca(int u, int v){
		if(anc(u, v)) return u;
		if(anc(v, u)) return v;
		
		for (int i = l; i >= 0; --i){
			if(!anc(up[u][i], v)){
				u = up[u][i];
			}
		}
		
		return up[u][0];
	}

	LCA (const vector<vector<int>> &graph, int root = 0){
		n = graph.size();
		adj = graph;
		tin = vector<int> (n);
		tout = vector<int> (n);
		timer = 0;
		l = ceil(log2(n));
		up.assign(n, vector<int>(l + 1));
		dfs(root, root);
	}
};
