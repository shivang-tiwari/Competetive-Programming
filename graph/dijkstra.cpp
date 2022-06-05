vector<long long int> dist;
vector<int> pre;
void dijkstra(int start,vector<vector<pair<int,int>>> &adj){
	dist.clear();dist.resize(adj.size(),1e18);
	pre.clear();pre.resize(adj.size(),-1);
	priority_queue<pair<long long int,int>,vector<pair<long long int,int>>,greater<pair<long long int,int>>> que;
	que.push({dist[start] = 0, start});
	while (!que.empty()) {
		auto top = que.top();
		que.pop();
		if (top.first != dist[top.second]) continue;
		for(auto node : adj[top.second])
			if(dist[node.first] > top.first + node.second) 
				que.push({dist[node.first] = top.first + node.second, node.first}), pre[node.first] = top.second;
	}
}

/*  No weights
vector<int> pre;
vector<long long int>dist;
void dijkstra(int start,vector<vector<int>> &adj){
	dist.clear();dist.resize(adj.size(),1e18);
	pre.clear();pre.resize(adj.size(),-1);
	queue<pair<int,int>> que;
	que.push({dist[start] = 0, start});
	while (!que.empty()) {
		auto top = que.front();
		que.pop();
		if (top.first != dist[top.second]) continue;
		for(auto node : adj[top.second])
			if(dist[node] > top.first + 1) 
				que.push({dist[node] = top.first + 1, node}), pre[node] = top.second;
	}
}
*/
