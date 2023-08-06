class segtree {
 public:
	int neutral = 0;
	struct node {
		int sum = 0;
		int add = 0;
		void apply(int l, int r, int v){
			sum = v; 
		}
		void push(int l, int r, int v) {
			sum += v*(r - l + 1); 
			add += v;
		}
	};
	
	node unite(const node &a, const node &b) const {
		node res;
		res.sum = a.sum + b.sum; 
		return res;
	}
	
	inline pair<int,int> get_ch(int x,int l,int r){
		int m = (l + r) >> 1;
		int z = x + ((m - l + 1) << 1);
		return {m,z};
	}
	
	inline void push(int x, int l, int r) {
		auto [m,z] = get_ch(x,l,r);
		if(tree[x].add != neutral) {
			tree[x + 1].push(l, m, tree[x].add);
			tree[z].push(m + 1, r, tree[x].add);
			tree[x].add = neutral;
		}
	}
	
	inline void pull(int x, int z) {
		tree[x] = unite(tree[x+1], tree[z]);
	}

	int n;
	vector<node> tree;
	
	node find(int x, int l, int r, int lx, int rx){ // query is [lx,rx]
		if (lx <= l && r <= rx){
			return tree[x];
		}
		auto [m,z] = get_ch(x,l,r);
		push(x, l, r);
		node res{};
		if(rx <= m){
			res = find(x + 1, l, m, lx, rx);
		} 
		else{
			if(lx > m){
				res = find(z, m + 1, r, lx, rx);
			}
			else{
				res = unite(find(x + 1, l, m, lx, rx), find(z, m + 1, r, lx, rx));
			}
		}
		pull(x, z);
		return res;
	}
	
	template <typename M>
	void update(int x, int l, int r, int lx, int rx, const M& v) {
		if (lx <= l && r <= rx) {
			tree[x].apply(l, r, v);
			return;
		}
		auto [m,z] = get_ch(x,l,r);
		push(x, l, r);
		if (lx <= m) {
			update(x + 1, l, m, lx, rx, v);
		}
		if (rx > m){
			update(z, m + 1, r, lx, rx, v);
		}
		pull(x, z);
	}
	
	template <typename M>
	void add(int x, int l, int r, int lx, int rx, const M& v){
		if (lx <= l && r <= rx) {
			tree[x].push(l, r, v);
			return;
		}
		auto [m,z] = get_ch(x,l,r);
		push(x, l, r);
		if (lx <= m) {
			add(x + 1, l, m, lx, rx, v);
		}
		if (rx > m) {
			add(z, m + 1, r, lx, rx, v);
		}
		pull(x, z);
	}
	
	template <typename M>
	segtree(const vector<M> &v) {
		n = v.size();
		assert(n > 0);
		tree = vector<node>(2*n - 1);
	}
	
	// All functions below
	template <typename M>
	void update(int i, const M& v){ // Sets value at index i to v
		assert(i >= 0 && i < n);
		update(0, 0, n - 1, i, i, v);
	}
	template <typename M>
	void add(int lx, int rx, const M& v) { // adds v to a[lx to rx]
		assert(0 <= lx && lx <= rx && rx <= n - 1);
		add(0, 0, n - 1, lx, rx, v);
	}
	node find(int lx, int rx) { // value of lx to rx
		assert(0 <= lx && lx <= rx && rx <= n - 1);
		return find(0, 0, n - 1, lx, rx);
	}
};
