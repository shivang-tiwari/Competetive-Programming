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
	
	inline void push(int x, int l, int r) {
		int m = (l + r) >> 1;
		if(tree[x].add != neutral) {
			tree[2*x + 1].push(l, m, tree[x].add);
			tree[2*x + 2].push(m + 1, r, tree[x].add);
			tree[x].add = neutral;
		}
	}
	
	inline void pull(int x) {
		tree[x] = unite(tree[2*x+1], tree[2*x+2]);
	}

	int n;
	vector<node> tree;
	
	node find(int x, int l, int r, int lx, int rx){ // query is [lx,rx]
		if (lx <= l && r <= rx){
			return tree[x];
		}
		int m = (l + r) >> 1;
		push(x, l, r);
		node res{};
		if(rx <= m){
			res = find(2*x + 1, l, m, lx, rx);
		} 
		else{
			if(lx > m){
				res = find(2*x + 2, m + 1, r, lx, rx);
			}
			else{
				res = unite(find(2*x + 1, l, m, lx, rx), find(2*x + 2, m + 1, r, lx, rx));
			}
		}
		pull(x);
		return res;
	}
	
	template <typename M>
	void update(int x, int l, int r, int lx, int rx, const M& v) {
		if (lx <= l && r <= rx) {
			tree[x].apply(l, r, v);
			return;
		}
		int m = (l + r) >> 1;
		push(x, l, r);
		if (lx <= m) {
			update(2*x + 1, l, m, lx, rx, v);
		}
		if (rx > m){
			update(2*x + 2, m + 1, r, lx, rx, v);
		}
		pull(x);
	}
	
	template <typename M>
	void add(int x, int l, int r, int lx, int rx, const M& v){
		if (lx <= l && r <= rx) {
			tree[x].push(l, r, v);
			return;
		}
		int m = (l + r) >> 1;
		push(x, l, r);
		if(lx <= m){
			add(2*x + 1, l, m, lx, rx, v);
		}
		if(rx > m) {
			add(2*x + 2, m + 1, r, lx, rx, v);
		}
		pull(x);
	}
	
	segtree(int _n) {
		n = _n;
		tree = vector<node>(4*n);
	}
	
	// All functions below
	template <typename M>
	void update(int i, const M& v){ // Sets value at index i to v
		update(0, 0, n - 1, i, i, v);
	}
	template <typename M>
	void add(int lx, int rx, const M& v) { // adds v to a[lx to rx]
		add(0, 0, n - 1, lx, rx, v);
	}
	node find(int lx, int rx) { // value of lx to rx
		return find(0, 0, n - 1, lx, rx);
	}
};
