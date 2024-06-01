vector<int> euler_path(const vector<vector<int>> &adj){ 
	int n = adj.size();
	vector<int> deg(n);
	for (int i = 0; i < n; ++i) {
		deg[i] = adj[i].size();
	}
	
	int first = 0;
	while (first < n && !deg[first]){
		++first;
	}
	if (first == n){ // no edges
		return {};
	}
	
	int v1 = -1, v2 = -1;
	
	for (int i = 0; i < n; ++i) {
		if (deg[i] & 1) {
			if (v1 == -1)
				v1 = i;
			else if (v2 == -1)
				v2 = i;
			else
				return {-1};
		}
	}
	
	vector<multiset<int>> g(n);
	
	for(int i = 0; i < n; i++){
		g[i] = multiset<int>(adj[i].begin(),adj[i].end());
	}
	
	if(v1 != -1){
		g[v1].insert(v2);
		g[v2].insert(v1);
	}
	
	stack<int> st;
	st.push(first);
	vector<int> res;
	while (!st.empty()) {
		int v = st.top();
		
		if(g[v].empty()){
			res.push_back(v);
			st.pop();
		}
		else{
			int i = *g[v].begin();
			
			g[v].erase(g[v].find(i));
			g[i].erase(g[i].find(v));
			st.push(i);
		}
	}
	
	if (v1 != -1) {
		for (size_t i = 0; i + 1 < res.size(); ++i) {
			if ((res[i] == v1 && res[i + 1] == v2) ||
				(res[i] == v2 && res[i + 1] == v1)) {
				vector<int> res2;
				for (size_t j = i + 1; j < res.size(); ++j)
					res2.push_back(res[j]);
				for (size_t j = 1; j <= i; ++j)
					res2.push_back(res[j]);
				res = res2;
				break;
			}
		}
	}
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if(!g[i].empty()){ // disconnected
				return {-1};
			}
		}
	}
	return res;
}
// for undirected graphs
// returns {-1} when euler path is not possible
