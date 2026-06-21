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


struct amogus {
	amogus (int n_) : ds(2*n_), n(n_) { }
	
	void same(int x, int y){
		ds.combine(x,y);
	}
	
	void diff(int x, int y){
		ds.combine(x,y+n);
		ds.combine(x+n,y);
	}
	
	bool is_valid(){
		for(int i = 0; i < n; i++){
			if(!is_valid(i)) return false;
		}
		return true;
	}
	
	bool is_valid(int i){
		return !ds.same(i,i+n);
	}
	
	protected:
	
	dsu ds;
	const int n;
};
