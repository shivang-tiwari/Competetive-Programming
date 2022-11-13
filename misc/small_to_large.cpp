#include <bits/stdc++.h>
using namespace std;
 
 
int n;
vector<vector<int>> graph;
vector<int> color,distinct;
vector<set<int>*> subtree;
 
void dfs(int i,int parent = -1){
	int largest = -1;
	vector<int> children;
	for(int node : graph[i]){
		if(node != parent){
			dfs(node,i);
			children.push_back(node);
			if(largest == -1 || subtree[largest]->size() < subtree[node]->size()){
				largest = node;
			}
		}
	}
	
	if(largest == -1){
		subtree[i] = new set<int>; // new set for leaf node
	}
	else{
		subtree[i] = subtree[largest]; // largest sized child
	}
	
	for(int child : children){
		if(child == largest)continue;
		subtree[i]->insert(subtree[child]->begin(),subtree[child]->end());
	}
	subtree[i]->insert(color[i]);
	distinct[i] = subtree[i]->size();
}
 
 
int main(){
	ios::sync_with_stdio(false);cin.tie(0);
	
	cin >> n;
	graph.resize(n);
	color.resize(n);
	distinct.resize(n);
	subtree.resize(n,NULL);
	for(int i = 0; i < n; i++){
		cin >> color[i];
	}
	for(int i = 0; i < n-1; i++){
		int u,v;
		cin >> u >> v;
		u--;v--;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}
	
	dfs(0,-1);
	
	for(int i = 0; i < n; i++){
		cout << distinct[i] << ' ';
	}
	return (0-0);
}
