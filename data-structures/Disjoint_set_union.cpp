struct dsu{
	int n;
	vector<int> parent;
	vector<int> siz;
	dsu(int sz) : n(sz){
		parent.resize(n);
		siz.resize(n,1);
		iota(parent.begin(),parent.end(), 0);
	}
	inline bool cmp(int a, int b){ // Give indices here
		return siz[a] < siz[b];
	}
	inline int find(int x){ // Give index here
		assert(x >= 0 && x < n);
		while (x != parent[x]) x = parent[x] = parent[parent[x]];
		return x;
	}
	bool combine(int a, int b){ // Give indices here, (returns true if a and b are in different sets)
		assert(min(a,b) >= 0 && max(a,b) < n);
		if((a = find(a)) != (b = find(b))){
			if(cmp(a,b))swap(a,b);
			parent[b] = a;
			siz[a] += siz[b];
			return true;
		}
		return false;
	}
	inline bool same(int a,int b){ // Returns true if a and b are in same set
		assert(min(a,b) >= 0 && max(a,b) < n);
		return find(a) == find(b);
	}
	inline int size(int x){ // Returns the size of the set containing x
		assert(x >= 0 && x < n);
		return siz[find(x)];
	}
};
