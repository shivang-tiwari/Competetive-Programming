template <typename T = int>
class blossom {
  public:
	struct edge {
		int from;
		int to;
		T cost;
	};
	vector<edge> edges;
	vector< vector<int> > g;
	int n;
	blossom(int _n) : n(_n) {
		g.resize(n);
	}
	int add_edge(int from, int to, T cost = 1) {
		assert(0 <= from && from < n && 0 <= to && to < n);
		int id = (int) edges.size();
		g[from].push_back(id);
		g[to].push_back(id);
		edges.push_back({from, to, cost});
		return id;
	}
	vector<int> max_matching(){
		vector<int> mate(n, -1);
		vector<int> label(n);
		vector<int> parent(n);
		vector<int> orig(n);
		queue<int> q;
		vector<int> aux(n, -1);
		int aux_time = -1;
		auto lca = [&](int x, int y) {
			aux_time++;
			while (true) {
				if(x != -1){
					if(aux[x] == aux_time){
						return x;
					}
					aux[x] = aux_time;
					if(mate[x] == -1){
						x = -1;
					}
					else{
						x = orig[parent[mate[x]]];
					}
				}
				swap(x, y);
			}
		};
		auto go_blossom = [&](int v, int w, int a){
			while (orig[v] != a) {
				parent[v] = w;
				w = mate[v];
				if(label[w] == 1){
					label[w] = 0;
					q.push(w);
				}
				orig[v] = orig[w] = a;
				v = parent[w];
			}
		};
		auto augment = [&](int v) {
			while(v != -1){
				int pv = parent[v];
				int nv = mate[pv];
				mate[v] = pv;
				mate[pv] = v;
				v = nv;
			}
		};
		auto bfs = [&](int root){
			fill(label.begin(), label.end(), -1);
			iota(orig.begin(), orig.end(), 0);
			while(!q.empty()){
				q.pop();
			}
			q.push(root);
			label[root] = 0;
			while (!q.empty()){
				int v = q.front();
				q.pop();
				for(int id : g[v]){
					auto &e = edges[id];
					int x = e.from ^ e.to ^ v;
					if(label[x] == -1){
						label[x] = 1;
						parent[x] = v;
						if(mate[x] == -1){
							augment(x);
							return true;
						}
						label[mate[x]] = 0;
						q.push(mate[x]);
						continue;
					}
					if(label[x] == 0 && orig[v] != orig[x]){
						int a = lca(orig[v], orig[x]);
						go_blossom(x, v, a);
						go_blossom(v, x, a);
					}
				}
			}
			return false;
		};
		auto greedy = [&]() {
			vector<int> order(n);
			iota(order.begin(), order.end(), 0);
			shuffle(order.begin(), order.end(), mt19937(787788));
			for(int i : order){
				if(mate[i] == -1){
					for(int id : g[i]){
						auto &e = edges[id];
						int to = e.from ^ e.to ^ i;
						if(mate[to] == -1){
							mate[i] = to;
							mate[to] = i;
							break;
						}
					}
				}
			}
		};
		greedy();
		for(int i = 0; i < n; i++){
			if(mate[i] == -1){
				bfs(i);
			}
		}
		return mate;
	}
};
