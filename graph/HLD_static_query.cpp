
const int inf = 1e9 + 60;

class HLD_static_query{
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
	
	class sparse_table{
		public:
		
		struct node{
			int sml = inf;
			void apply(int x){
				sml = x;
			}
			void reverse(){
				
			}
		};
		node unite(const node &a,const node &b){
			node res{};
			res.sml = min(a.sml,b.sml);
			return res;
		}
		
		int N, LG;
		vector<vector<node>>spt;
		vector<int>lg;
		sparse_table(){}
		
		template<class T>
		sparse_table(vector<T> &a){
			N = a.size();
			LG = 32 - __builtin_clz(std::max(N - 1, 1));
			spt.resize(N+1, vector<node>(LG+1));
			lg.resize(N + 5,0);
			for(int i = 2;i <= N;i++)
				lg[i] = lg[i/2] + 1;
			build(a);
		}
		sparse_table(int _N){
			N = _N;
			LG = 32 - __builtin_clz(std::max(N - 1, 1));
			spt.resize(N+1, vector<node>(LG+1));
			lg.resize(N + 5,0);
			for(int i = 2;i <= N;i++)
				lg[i] = lg[i/2] + 1;
		}
		template<class T>
		void build(vector<T> &a){
			for(int i = 0;i < N;i++)
				spt[i][0].apply(a[i]);
			for(int j = 1;j <= LG;j++){
				int len = (1 << j), half = (1 << (j-1));
				for(int i = 0;i + len <= N;i++)
					spt[i][j] = unite(spt[i][j-1],spt[i+half][j-1]);
			}
		}
		node find(int l, int r){ // For idempotent operations in O(1)
			assert(l <= r && l >= 0 && r <= N-1);
			int lgg = lg[r-l+1], len = (1<<lgg);
			return unite(spt[l][lgg], spt[r-len+1][lgg]);
		}
		node query(int L, int R){ // For other operations in O(log n)
			node answer{};
			for (int j = LG; j >= 0; j--) {
				if (L + (1 << j) - 1 <= R) {
					answer = unite(answer,spt[L][j]);
					L += 1 << j;
				}
			}
			return answer;
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
	vector<int> help;
	int tim;
	bool leave_lca;
	LCA lca;
	sparse_table tr{};
	HLD_static_query(const vector<vector<int>> &G,const vector<int> &values,bool _leave_lca){
		adj = G;
		a = values;
		N = G.size();
		help.resize(N);
		label.resize(N);
		depth.resize(N);
		par.resize(N,-1);
		siz.resize(N);
		heavy.resize(N,-1);
		chain_head.resize(N,-1);
		iota(chain_head.begin(),chain_head.end(),0);
		tim = 0;
		leave_lca = _leave_lca;
		sub_dfs(0);
		label_dfs(0);
		chain_dfs(0);
		
		lca = LCA(adj);
		tr = sparse_table(help);
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
		help[tim] = a[i];
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
	sparse_table::node find(int u,int v,int flag = -1){
		if(flag == -1){
			flag = leave_lca;
		}
		if(u == v){
			return (flag ? sparse_table::node() : tr.find(label[u],label[u]));
		}
		int lc = lca.lca(u,v);
		
		
		sparse_table::node u_to_lca;
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
		
		sparse_table::node lca_to_v;
		
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
};

// Set third argument to FALSE for vertex queries and TRUE for edge queries
