template<typename T>
struct static_tree_query{
	struct node{
		int sum = 0;
		
		node(){
			sum = 0;
		}
		
		node(T x){
			sum += x;
		}
		
		void reverse(){
			
		}
	};
	
	node unite(const node& a, const node& b){
		node res;
		res.sum = a.sum + b.sum;
		return res;
	}
	
	int n, l;
	vector<vector<int>> adj;
 
	int timer;
	vector<int> tin, tout;
	vector<vector<int>> up;
	vector<int> A;
	vector<vector<node>> lft;
 
	void dfs(int v, int p){
		tin[v] = ++timer;
		up[v][0] = p;
		
		lft[v][0] = node(A[v]);
		
		for (int i = 1; i <= l; ++i){
			
			if(up[v][i-1] == -1)continue;
			
			up[v][i] = up[up[v][i-1]][i-1];
			lft[v][i] = unite(lft[v][i-1],lft[up[v][i-1]][i-1]);
		}
 
		for(int u : adj[v]){
			if(u != p){
				dfs(u, v);
			}
		}
 
		tout[v] = ++timer;
	}
 
	bool anc(int u, int v){
		return tin[u] <= tin[v] && tout[u] >= tout[v];
	}
 
	int lca(int u, int v){
		if(anc(u, v)) return u;
		if(anc(v, u)) return v;
		
		for (int i = l; i >= 0; --i){
			if(up[u][i] == -1)continue;
			
			if(!anc(up[u][i], v)){
				u = up[u][i];
			}
		}
		
		return up[u][0];
	}
 
	static_tree_query (const vector<vector<int>> &graph, const vector<T> &A_, int root = 0){
		A = A_;
		n = graph.size();
		adj = graph;
		tin = vector<int> (n);
		tout = vector<int> (n);
		timer = 0;
		l = ceil(log2(n));
		up.assign(n, vector<int>(l + 1,-1));
		lft.assign(n, vector<node>(l + 1));
		dfs(root, -1);
	}
	
	node find(int u,int v){
		int lc = lca(u,v);
		node u_to_lca;
		for (int i = l; i >= 0; --i){
			
			if(up[u][i] == -1)continue;
			
			if(!anc(up[u][i], lc) || up[u][i] == lc){
				u_to_lca = unite(u_to_lca,lft[u][i]);
				u = up[u][i];
			}
		}
		
		u_to_lca = unite(u_to_lca,lft[lc][0]);
		
		node v_to_lca;
		
		for (int i = l; i >= 0; --i){
			
			if(up[v][i] == -1)continue;
			
			if(!anc(up[v][i], lc) || up[v][i] == lc){
				v_to_lca = unite(v_to_lca,lft[v][i]);
				v = up[v][i];
			}
		}
		
		v_to_lca.reverse();
		
		return unite(u_to_lca,v_to_lca);
	}
};
