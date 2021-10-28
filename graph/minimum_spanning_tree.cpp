struct dsu{
	int n;
	vector<int> parent;
	vector<int> siz;
	dsu(int sz) : n(sz){
		parent.resize(n);
		siz.resize(n,1);
		iota(parent.begin(),parent.end(), 0);
	}
	bool cmp(int a, int b){ // Give indices here
		return siz[a] < siz[b];
	}
	inline int find(int x){ // Give index here
		return (x == parent[x] ? x : (parent[x] = find(parent[x])));
	}
	inline bool combine(int a, int b){ // Give indices here, (returns true if a and b are in different sets)
		a = find(a);
		b = find(b);
		if(a != b){
			if(cmp(a,b))swap(a,b);
			parent[b] = a;
			siz[a] += siz[b];
			return true;
		}
		return false;
	}
};

vector<vector<pair<int,int>>> mst(vector<vector<pair<int,int>>> &adj){ // Undirected minimum spanning tree of undirected graph
	int n = adj.size();
	vector<vector<int>> edg;
	for(int i = 0; i < n; i++){
		for(auto x : adj[i]){
			edg.push_back({x.second,i,x.first});
		}
	}
	sort(edg.begin(),edg.end());
	dsu uf(n);
	vector<vector<pair<int,int>>> ret(n);
	for(auto p : edg){
		if(uf.combine(p[1],p[2])){
			ret[p[1]].push_back({p[2],p[0]});
			ret[p[2]].push_back({p[1],p[0]});
		}
	}
	return ret;
}
