int n;
vector<vector<int>> graph;
vector<int> Enter,Exit,order;
void traverse(int root = 0,int par = -1){
	order.clear();
	Enter.clear();Enter.resize(n);
	Exit.clear();Exit.resize(n);
	int travel_time = 0;
	function<void(int,int)> traverse_tree = [&](int i, int parent){
		order.push_back(i);
		Enter[i] = travel_time++;
		for(int node : graph[i]){
			if(node != parent){
				traverse_tree(node,i);
			}
		}
		Exit[i] = travel_time++;
	};traverse_tree(root,par);
}

bool anc(int child, int ancestor){ // If ansector is an ancestor of child
	return Enter[ancestor] < Enter[child] && Exit[child] < Exit[ancestor];
}
