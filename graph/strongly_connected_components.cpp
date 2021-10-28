vector<vector<int>> scc;
vector<int> comp;

void SCC(vector<vector<int>> &adj){
	int V = adj.size();
	comp.resize(V);
	int *disc = new int[V];
	int *low = new int[V];
	bool *stackMember = new bool[V];
	stack<int> *st = new stack<int>();
	for (int i = 0; i < V; i++){
		disc[i] = -1;
		low[i] = -1;
		stackMember[i] = false;
	}
	static int time = 0;
	function<void(int)> find_scc = [&](int u){
		disc[u] = low[u] = ++time;
		st->push(u);
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
			while (st->top() != u){
				w = (int) st->top();
				row.push_back(w);
				comp[w] = ID;
				stackMember[w] = false;
				st->pop();
			}
			w = (int) st->top();
			row.push_back(w);
			comp[w] = ID;
			scc.push_back(row);
			stackMember[w] = false;
			st->pop();
		}
	};
	for (int i = 0; i < V; i++){
		if (disc[i] == -1)find_scc(i);
	}
}
