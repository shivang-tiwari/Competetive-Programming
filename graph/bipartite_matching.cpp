vector<int> col;
bool bpcheck(vector<vector<int>> &adj){
	int V = adj.size();
	col.clear();col.resize(V,-1);
	queue<pair<int,int>> q;
	for(int i = 0; i < V; i++){
		if(col[i] == -1) {
			q.push({i,0});
			col[i] = 0;
			while(!q.empty()){
				pair<int,int> p = q.front();
				q.pop();
				int v = p.first;
				int c = p.second;
				for(int j : adj[v]){
					if(col[j] == c)
						return false;
					if(col[j] == -1){
						col[j] = c ^ 0 ^ 1;
						q.push({j, col[j]});
					}
				}
			}
		}
	}
	return true;
}

vector<int> bpm(vector<vector<int>> &G,int n,int m){
	const int MAX = G.size() + 1,NIL = 0,INF = (1 << 28);
	vector<int> match(MAX); // Return this if matching is required
	vector<int> dist(MAX);
	function<bool()> bfs = [&]() {
		queue<int> Q;
		for(int i = 1; i <= n; i++) {
			if(match[i] == NIL) {
				dist[i] = 0;
				Q.push(i);
			}
			else dist[i] = INF;
		}
		dist[NIL] = INF;
		while(!Q.empty()) {
			int u = Q.front();Q.pop();
			if(u != NIL) {
				for(int v : G[u]){
					if(dist[match[v]] == INF) {
						dist[match[v]] = dist[u] + 1;
						Q.push(match[v]);
					}
				}
			}
		}
		return (dist[NIL]!=INF);
	};
	function<bool(int)> dfs = [&](int u) {
		if(u != NIL) {
			for(int v : G[u]){
				if(dist[match[v]] == dist[u]+1) {
					if(dfs(match[v])){
						match[v] = u;
						match[u] = v;
						return true;
					}
				}
			}
			dist[u] = INF;
			return false;
		}
		return true;
	};
	int matching = 0;
	// match[i] is assumed 0 for all vertex in G
	while(bfs())
		for(int i = 1; i <= n; i++){
			if(match[i] == NIL && dfs(i)){
				matching++;
			}
		}
	return match;
}

// n = number of applicants 
// m = number of jobs
// Applicants are enumerated as [1,2,3,...,n]
// Jobs are enumerated as [n+1,n+2,n+3,...,n+m]
// Make a double sided edge between applicant and job
// Hopcroft Karp algorithm, O(sqrt(V) * E)
// !!! REMEMBER 1-BASED INDEXING !!!
