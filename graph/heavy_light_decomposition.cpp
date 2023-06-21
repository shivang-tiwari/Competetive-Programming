class HLD{
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
		LCA(){}
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
	
	class segtree {
	 public:
		int neutral = 0;
		struct node {
			// don't forget to set default value (used for leaves)
			// not necessarily neutral element!
			int sum = 0; // Set identity element
		
			void apply(int l, int r, int v) { // Value of a single node (used by build and update)
				sum = v;
			}
			void push(int l, int r, int v) { // Lazy propogation (used by add)
				//sum += v*(r - l + 1); 
				//add += v;
			}
			void reverse(){
			}
		};
		node unite(const node &a, const node &b) const { // Set combination operation
			node res;
			 res.sum = a.sum + b.sum;
			return res;
		}
		inline void push(int x, int l, int r) {
			//int y = (l + r) >> 1;
			//int z = x + ((y - l + 1) << 1);
			//if (tree[x].add != neutral) {
				//tree[x + 1].push(l, y, tree[x].add);
				//tree[z].push(y + 1, r, tree[x].add);
				//tree[x].add = neutral;
			//}
			
			// Don't forget to uncomment node.push
		}
		inline void pull(int x, int z) {
			tree[x] = unite(tree[x + 1], tree[z]);
		}
	
		int n;
		vector<node> tree;
		void build(int x, int l, int r) {
			if (l == r) {
				return;
			}
			int y = (l + r) >> 1;
			int z = x + ((y - l + 1) << 1);
			build(x + 1, l, y);
			build(z, y + 1, r);
			pull(x, z);
		}
		template <typename M>
		void build(int x, int l, int r, const vector<M> &v) {
			if (l == r) {
				tree[x].apply(l, r, v[l]);
				return;
			}
			int y = (l + r) >> 1;
			int z = x + ((y - l + 1) << 1);
			build(x + 1, l, y, v);
			build(z, y + 1, r, v);
			pull(x, z);
		}
		node find(int x, int l, int r, int lx, int rx) {
			if (lx <= l && r <= rx) {
				return tree[x];
			}
			int y = (l + r) >> 1;
			int z = x + ((y - l + 1) << 1);
			push(x, l, r);
			node res{};
			if (rx <= y) {
				res = find(x + 1, l, y, lx, rx);
			} else {
				if (lx > y) {
				res = find(z, y + 1, r, lx, rx);
				} else {
				res = unite(find(x + 1, l, y, lx, rx), find(z, y + 1, r, lx, rx));
				}
			}
			pull(x, z);
			return res;
		}
		template <typename... M>
		void update(int x, int l, int r, int lx, int rx, const M&... v) {
			if (lx <= l && r <= rx) {
				tree[x].apply(l, r, v...);
				return;
			}
			int y = (l + r) >> 1;
			int z = x + ((y - l + 1) << 1);
			push(x, l, r);
			if (lx <= y) {
				update(x + 1, l, y, lx, rx, v...);
			}
			if (rx > y) {
				update(z, y + 1, r, lx, rx, v...);
			}
			pull(x, z);
		}
		template <typename... M>
		void add(int x, int l, int r, int lx, int rx, const M&... v) {
			if (lx <= l && r <= rx) {
				tree[x].push(l, r, v...);
				return;
			}
			int y = (l + r) >> 1;
			int z = x + ((y - l + 1) << 1);
			push(x, l, r);
			if (lx <= y) {
				add(x + 1, l, y, lx, rx, v...);
			}
			if (rx > y) {
				add(z, y + 1, r, lx, rx, v...);
			}
			pull(x, z);
		}
		segtree(){}
		segtree(int _n) : n(_n) {
			assert(n > 0);
			tree.resize(2 * n - 1);
			build(0, 0, n - 1);
		}
		template <typename M>
		segtree(const vector<M> &v) {
			n = v.size();
			assert(n > 0);
			tree.resize(2 * n - 1);
			build(0, 0, n - 1, v);
		}
		node find(int p) { // value at index p
			assert(0 <= p && p <= n - 1);
			return find(0, 0, n - 1, p, p);
		}
		template <typename... M>
		void update(int lx, int rx, const M&... v) {
			assert(0 <= lx && lx <= rx && rx <= n - 1);
			update(0, 0, n - 1, lx, rx, v...);
		}
		void add(int i, int v){
			assert(i >= 0 && i < n);
			add(i,i,v);
		}
		// All functions below
		void update(int i, int v){ // Sets value at index i to v
			assert(i >= 0 && i < n);
			update(i,i,v);
		}
		template <typename... M>
		void add(int lx, int rx, const M&... v) { // adds v to a[lx to rx]
			assert(0 <= lx && lx <= rx && rx <= n - 1);
			add(0, 0, n - 1, lx, rx, v...);
		}
		node find(int lx, int rx) { // value of lx to rx
			assert(0 <= lx && lx <= rx && rx <= n - 1);
			return find(0, 0, n - 1, lx, rx);
		}
	};
	
	public:
	int N;
	vector<vector<int>> adj;
	vector<int> a;
	vector<int> label;
	vector<int> par;
	vector<int> depth;
	vector<int> siz;
	vector<int> heavy;
	vector<int> chain_head;
	int tim;
	bool leave_lca;
	LCA lca;
	segtree tr;
	HLD(const vector<vector<int>> &G,const vector<int> &values,bool _leave_lca){
		adj = G;
		a = values;
		N = G.size();
		label.resize(N);
		depth.resize(N);
		par.resize(N,-1);
		siz.resize(N);
		heavy.resize(N,-1);
		chain_head.resize(N,-1);
		iota(chain_head.begin(),chain_head.end(),0);
		tim = 0;
		leave_lca = _leave_lca;
		lca = LCA(adj);
		tr = segtree(N);
		sub_dfs(0);
		label_dfs(0);
		chain_dfs(0);
	}
	void sub_dfs(int i,int parent = -1){
		par[i] = parent;
		siz[i] = 1;
		for(int v : adj[i]){
			if(v != parent){
				depth[v] = 1 + depth[i];
				sub_dfs(v,i);
				siz[i] += siz[v];
				if(heavy[i] == -1 || siz[heavy[i]] < siz[v]){
					heavy[i] = v;
				}
			}
		}
	}
	void label_dfs(int i,int parent = -1){
		tr.update(tim,a[i]);
		label[i] = tim++;
		if(heavy[i] != -1)label_dfs(heavy[i],i);
		for(int v : adj[i]){
			if(v != parent && v != heavy[i]){
				label_dfs(v,i);
			}
		}
	}
	void chain_dfs(int i,int parent = -1){
		for(int v : adj[i]){
			if(v != parent){
				if(v == heavy[i]){
					chain_head[v] = chain_head[i];
				}
				chain_dfs(v,i);
			}
		}
	}
	// All functions below
	segtree::node find(int u,int v,int flag = -1){
		if(flag == -1){
			flag = leave_lca;
		}
		if(u == v){
			return (flag ? segtree::node() : tr.find(label[u],label[u]));
		}
		int lc = lca.lca(u,v);
		
		
		segtree::node u_to_lca;
		if(flag){
			while(depth[u] > depth[lc]){
				int where = (depth[chain_head[u]] <= depth[lc] ? lc : chain_head[u]);
				if(where == lc){
					u_to_lca = tr.unite(tr.find(label[where]+1,label[u]),u_to_lca);
					u = lc;
				}
				else{
					u_to_lca = tr.unite(tr.find(label[where],label[u]),u_to_lca);
					u = par[where];
				}
			}
		}
		else{
			while(u != -1 && depth[u] >= depth[lc]){
				int where = (depth[chain_head[u]] <= depth[lc] ? lc : chain_head[u]);
				u_to_lca = tr.unite(tr.find(label[where],label[u]),u_to_lca);
				u = par[where];
			}
		}
		
		u_to_lca.reverse();
		
		segtree::node lca_to_v;
		
		u = v;
		// [v,lc)
		while(depth[u] > depth[lc]){
			int where = (depth[chain_head[u]] <= depth[lc] ? lc : chain_head[u]);
			if(where == lc){
				lca_to_v = tr.unite(tr.find(label[where]+1,label[u]),lca_to_v);
				u = lc;
			}
			else{
				lca_to_v = tr.unite(tr.find(label[where],label[u]),lca_to_v);
				u = par[where];
			}
		}
		
		
		return tr.unite(u_to_lca,lca_to_v);
	}
	void update(int i,int v){
		tr.update(label[i],v);
	}
	void add(int u,int v,int x, int flag = -1){
		if(flag == -1)flag = leave_lca;
		if(u == v){
			if(flag)return;
			tr.add(label[u],label[u],x);
			return;
		}
		int lc = lca.lca(u,v);
		if(flag){
			while(depth[u] > depth[lc]){
				int where = (depth[chain_head[u]] <= depth[lc] ? lc : chain_head[u]);
				if(where == lc){
					tr.add(label[where]+1,label[u],x);
					u = lc;
				}
				else{
					tr.add(label[where],label[u],x);
					u = par[where];
				}
			}
		}
		else{
			while(u != -1 && depth[u] >= depth[lc]){
				int where = (depth[chain_head[u]] <= depth[lc] ? lc : chain_head[u]);
				tr.add(label[where],label[u],x);
				u = par[where];
			}
		}
		u = v;
		while(depth[u] > depth[lc]){
			int where = (depth[chain_head[u]] <= depth[lc] ? lc : chain_head[u]);
			if(where == lc){
				tr.add(label[where]+1,label[u],x);
				u = lc;
			}
			else{
				tr.add(label[where],label[u],x);
				u = par[where];
			}
		}
	}
};

// Set third argument to FALSE for vertex queries and TRUE for edge queries
