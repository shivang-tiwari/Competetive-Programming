vector<int> col;
bool bpcheck(vector<vector<int>> &adj){
	int V = adj.size();
	col.clear();col.resize(V,-1);
	queue<pair<int,int>> q;
	for(int i = 0; i < V; i++){
		if(col[i] == -1) {
			q.push({i,0});
			col[i] = 0;
			while(!q.empty()){
				pair<int,int> p = q.front();
				q.pop();
				int v = p.first;
				int c = p.second;
				for(int j : adj[v]){
					if(col[j] == c)
						return false;
					if(col[j] == -1){
						col[j] = c ^ 0 ^ 1;
						q.push({j, col[j]});
					}
				}
			}
		}
	}
	return true;
}

class bpm {
	public:
	vector< vector<int> > g;
	vector<int> pa;
	vector<int> pb;
	vector<int> was;
	int n, m;
	int res;
	int iter;
	bpm(int _n, int _m) : n(_n), m(_m){
		assert(0 <= n && 0 <= m);
		pa = vector<int>(n, -1);
		pb = vector<int>(m, -1);
		was = vector<int>(n, 0);
		g.resize(n);
		res = 0;
		iter = 0;
	}
	void add(int from, int to){
		assert(0 <= from && from < n && 0 <= to && to < m);
		g[from].push_back(to);
	}
	bool dfs(int v) {
		was[v] = iter;
		for(int u : g[v]) {
			if (pb[u] == -1) {
				pa[v] = u;
				pb[u] = v;
				return true;
			}
		}
		for(int u : g[v]){
			if(was[pb[u]] != iter && dfs(pb[u])){
				pa[v] = u;
				pb[u] = v;
				return true;
			}
		}
		return false;
	}
	int solve() {
		while(true){
			iter++;
			int add = 0;
			for(int i = 0; i < n; i++){
				if (pa[i] == -1 && dfs(i)){
					add++;
				}
			}
			if(add == 0){
				break;
			}
			res += add;
		}
		return res;
	}
	int run_one(int v){
		if(pa[v] != -1){
			return 0;
		}
		iter++;
		return (int) dfs(v);
	}
};

// n = number of applicants 
// m = number of jobs
// Applicants are enumerated as [0,1,2,...,n-1]
// Jobs are enumerated as [0,1,2,...,m-1]

// pa[i] = Job alotted to ith Applicant
// pb[i] = Applicant alotted to ith Job

// !!! REMEMBER 0-BASED INDEXING !!!
