vector<vector<int>> scc;
vector<int> comp;

void SCC(vector<vector<int>> &adj){
	scc.clear();comp.clear();
	int V = adj.size();
	comp.resize(V);
	vector<int> disc(V);
	vector<int> low(V);
	vector<bool> stackMember(V);
	stack<int> st;
	for (int i = 0; i < V; i++){
		disc[i] = -1;
		low[i] = -1;
		stackMember[i] = false;
	}
	static int time = 0;
	function<void(int)> find_scc = [&](int u){
		disc[u] = low[u] = ++time;
		st.push(u);
		stackMember[u] = true;
		for (auto i = adj[u].begin(); i != adj[u].end(); ++i){
			int v = *i;
			if (disc[v] == -1){
				find_scc(v);
				low[u]  = min(low[u], low[v]);
			}
			else if (stackMember[v])
				low[u]  = min(low[u], disc[v]);
		}
		int w = 0;
		if (low[u] == disc[u]){
			vector<int> row;
			int ID = scc.size();
			while (st.top() != u){
				w = (int) st.top();
				row.push_back(w);
				comp[w] = ID;
				stackMember[w] = false;
				st.pop();
			}
			w = (int) st.top();
			row.push_back(w);
			comp[w] = ID;
			scc.push_back(row);
			stackMember[w] = false;
			st.pop();
		}
	};
	for (int i = 0; i < V; i++){
		if (disc[i] == -1)find_scc(i);
	}
}

// Call this immediately after SCC to get the compressed graph

vector<vector<int>> build_graph(const vector<vector<int>> &adj){ 
	int n = adj.size();
	vector<vector<int>> scc_graph(scc.size());
	for(int i = 0; i < n; i++){
		for(int x : adj[i]){
			if(comp[i] != comp[x]){
				scc_graph[comp[i]].push_back(comp[x]);
			}
		}
	}
	return scc_graph;
}
