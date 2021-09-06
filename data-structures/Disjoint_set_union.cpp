struct dsu{
	int n;
	vector<int> parent;
	vector<int> siz;
	dsu(int sz) : n(sz){
		parent.resize(n);
		siz.resize(n,1);
		iota(parent.begin(),parent.end(), 0);
	}
	bool cmp(int a, int b){ // Give indices here
		return siz[a] < siz[b];
	}
	inline int find(int x){ // Give index here
		return (x == parent[x] ? x : (parent[x] = find(parent[x])));
	}
	inline bool combine(int a, int b){ // Give indices here, (returns true if a and b are in different sets)
		a = find(a);
		b = find(b);
		if(a != b){
			if(cmp(a,b))swap(a,b);
			parent[b] = a;
			siz[a] += siz[b];
			return true;
		}
		return false;
	}
};
