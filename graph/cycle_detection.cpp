//This detects cycle in a ***directed graph***
bool isCyclic(vector<vector<int>> &adj){
	int V = adj.size();
	vector<bool> visited(V);
	vector<bool> recStack(V);
	for(int i = 0; i < V; i++){
		visited[i] = false;
		recStack[i] = false;
	}
	function<bool(int)> cycle = [&](int v){
			if(visited[v] == false){
				visited[v] = true;
				recStack[v] = true;
				for(int i : adj[v]){
					if (!visited[i] && cycle(i))return true;
					else if (recStack[i])return true;
				}
			}
			recStack[v] = false;
			return false;
	};
	for(int i = 0; i < V; i++){
		if(cycle(i))return true;
	}
	return false;
}


//This detects cycle in an ***undirected graph***
vector<int> cyc;
bool isCyclic(vector<vector<int>> &adj){
	int V = adj.size();
	vector<bool> visited(V);
	for(int i = 0; i < V; i++){
		visited[i] = false;
	}
	function<bool(int,int)> cycle = [&](int v,int parent) -> bool {
		cyc.push_back(v);
		visited[v] = true;
		for(int i : adj[v]){
			if (!visited[i]){
				if(cycle(i,v)){
					return true;
				}
			}
			else if(i != parent){
				reverse(cyc.begin(),cyc.end());
				while(!cyc.empty() && cyc.back() != i)cyc.pop_back();
				reverse(cyc.begin(),cyc.end());
				return true;
			}
		}
		cyc.pop_back();
		return false;
	};
	for(int i = 0; i < V; i++){
		if(!visited[i] && cycle(i,-1)){
			return true;
		}
	}
	return false;
}
