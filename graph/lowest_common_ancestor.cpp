class LCA{
	template<typename T>
	struct SparseTable{
		vector<vector<T>> table;
		vector<int> logtable;
		function<T(T, T)> merge_func;
		SparseTable(){}
		SparseTable(const vector<T> &vec, const function<T(T, T)> &f): merge_func(f){
			int maxlength = 0;
			while((1 << (maxlength+1)) <= (int)vec.size()) maxlength++;
			table.resize(maxlength+1, vector<T>(vec.size()));
			logtable.resize(vec.size()+1);
			for(int i = 0;i < maxlength + 1;i++){
				for(int j = 0;j < (int)vec.size() - (1 << i) + 1;j++){
					if(i)table[i][j] = merge_func(table[i-1][j], table[i-1][j + (1 << (i-1))]);
					else table[i][j] = vec[j];
				}
			}
			for(int i = 2;i <= (int)vec.size();i++)logtable[i] = logtable[i >> 1]+1;
		}
		T query(int l, int r){
			assert(l < r);
			int length = r - l;
			return merge_func(table[logtable[length]][l], table[logtable[length]][r - (1 << logtable[length])]);
		}
	};
	private:
	int a = 0, b = 0;
	vector<int> begin, number, tour, dep, rev;
	SparseTable<int> table;
	void init_dfs(int v, int p, const vector<vector<int>> &g){
		number[v] = b;rev[b] = v;b++;
		for(auto t : g[v]){
			if(t == p) continue;
			dep[t] = dep[v] + 1;
			init_dfs(t, v, g);
			tour.push_back(number[v]);
			if(begin[v] == -1)begin[v] = a;
			a++;
		}
		if(begin[v] == -1)begin[v] = a;
	}
	public:
	//initialization O(NlogN)
	LCA(const vector<vector<int>> &g, int root = 0):begin(g.size(), -1), number(g.size()), dep(g.size(), -1), rev(g.size()){
		dep[root] = 0;
		init_dfs(root, -1, g);
		table = SparseTable<int>(tour, [](int x, int y){return min(x, y);});
	}
	//O(1) per query
	int lca(int u, int v){
		if(begin[u] == begin[v]) return (dep[u] > dep[v] ? v : u);
		if(begin[u] > begin[v]) swap(u, v);
			return rev[table.query(begin[u], begin[v]+1)];
	}
	int depth(int v){
		return dep[v];
	}
	int dist(int u, int v){
		return dep[u] + dep[v] - 2*dep[lca(u, v)];
	}
};
