int n,m;
vector<vector<pair<int,int>>> graph; // id,weight
vector<int> dist,pre;
void dijkstra(int start){
	dist.clear();dist.resize(n,1e18);
	pre.clear();pre.resize(n,-1);
	priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> que;
	que.push({dist[start] = 0, start});
	while (!que.empty()) {
		auto top = que.top();
		que.pop();
		if (top.first != dist[top.second]) continue;
		for(auto node : graph[top.second])
			if(dist[node.first] > top.first + node.second) 
				que.push({dist[node.first] = top.first + node.second, node.first}), pre[node.first] = top.second;
	}
}
