const int INF = 1e9;
class mcmf{
  public:
	struct Edge{
		int from, to, capacity, cost;
	};

	vector<vector<int>> adj, cost, capacity;
	vector<Edge> edges;
	int N;
	
	mcmf(int _n){
		N = _n;
	}
	
	void addEdge(int From,int To,int Capacity, int Cost){
		assert(From >= 0 && From < N && To >= 0 && To < N);
		edges.push_back({From,To,Capacity,Cost});
	}
	
	void shortest_paths(int n, int v0, vector<int>& d, vector<int>& p) {
		d.assign(n, INF);
		d[v0] = 0;
		vector<bool> inq(n, false);
		queue<int> q;
		q.push(v0);
		p.assign(n, -1);

		while (!q.empty()) {
			int u = q.front();
			q.pop();
			inq[u] = false;
			for (int v : adj[u]) {
				if (capacity[u][v] > 0 && d[v] > d[u] + cost[u][v]) {
					d[v] = d[u] + cost[u][v];
					p[v] = u;
					if (!inq[v]) {
						inq[v] = true;
						q.push(v);
					}
				}
			}
		}
	}

	int min_cost_flow(int s, int t,int K = INF) {
		adj.assign(N, vector<int>());
		cost.assign(N, vector<int>(N, 0));
		capacity.assign(N, vector<int>(N, 0));
		for (Edge e : edges) {
			adj[e.from].push_back(e.to);
			adj[e.to].push_back(e.from);
			cost[e.from][e.to] = e.cost;
			cost[e.to][e.from] = -e.cost;
			capacity[e.from][e.to] = e.capacity;
		}

		int flow = 0;
		int Cost = 0;
		vector<int> d, p;
		while (flow < K) {
			shortest_paths(N, s, d, p);
			if (d[t] == INF)
				break;

			// find max flow on that path
			int f = K - flow;
			int cur = t;
			while (cur != s) {
				f = min(f, capacity[p[cur]][cur]);
				cur = p[cur];
			}

			// apply flow
			flow += f;
			Cost += f * d[t];
			cur = t;
			while (cur != s) {
				capacity[p[cur]][cur] -= f;
				capacity[cur][p[cur]] += f;
				cur = p[cur];
			}
		}

		if (flow < K && K != INF)
			return -1;
		else
			return Cost;
	}
};
// Finds minimum cost for a flow of K units
// Set K == INF if minimum cost for maximum flow is required
