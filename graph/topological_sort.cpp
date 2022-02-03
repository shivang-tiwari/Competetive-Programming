vector<int> toposort(vector<vector<int>> &adj){
	int V = adj.size();
	stack<int> Stack;
	vector<bool> visited(V,false);
	function<void(int)> toposort = [&](int v) -> void {
		visited[v] = true;
		for(auto i : adj[v])
			if(!visited[i])
				toposort(i);
		Stack.push(v);
	};
	for(int i = 0; i < V; i++)
		if (!visited[i])
			toposort(i);
	vector<int> ans;
	while(!Stack.empty()) {
		ans.push_back(Stack.top());
		Stack.pop();
	}
	return ans;
}
