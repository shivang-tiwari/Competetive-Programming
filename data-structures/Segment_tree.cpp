class segtree {
 public:
	int neutral = 0;
	struct node {
		// don't forget to set default value (used for leaves)
		// not necessarily neutral element!
		
		long long int sum = 0;
		
		int add = 0;
		
		template <typename M>
		void apply(int l, int r, const M &v) { // Value of a single node (used by build and update)
			sum = v; 
		}
		void push(int l, int r, int v) { // Lazy propogation (used by add)
			//sum += v*(r - l + 1); 
			//add += v;
		}
	};
	node unite(const node &a, const node &b) const { // Set combination operation
		node res;
		res.sum = a.sum + b.sum; 
		return res;
	}
	inline void push(int x, int l, int r) {
		//int y = (l + r) >> 1;
		//int z = x + ((y - l + 1) << 1);
		//if (tree[x].add != neutral) {
			//tree[x + 1].push(l, y, tree[x].add);
			//tree[z].push(y + 1, r, tree[x].add);
			//tree[x].add = neutral;
		//}
		
		// Don't forget to uncomment node.push
	}
	inline void pull(int x, int z) {
		tree[x] = unite(tree[x + 1], tree[z]);
	}

	int n;
	vector<node> tree;
	void build(int x, int l, int r) {
		if (l == r) {
			return;
		}
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		build(x + 1, l, y);
		build(z, y + 1, r);
		pull(x, z);
	}
	template <typename M>
	void build(int x, int l, int r, const vector<M> &v) {
		if (l == r) {
			tree[x].apply(l, r, v[l]);
			return;
		}
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		build(x + 1, l, y, v);
		build(z, y + 1, r, v);
		pull(x, z);
	}
	node find(int x, int l, int r, int lx, int rx) {
		if (lx <= l && r <= rx) {
			return tree[x];
		}
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		push(x, l, r);
		node res{};
		if(rx <= y){
			res = find(x + 1, l, y, lx, rx);
		} 
		else{
			if(lx > y){
				res = find(z, y + 1, r, lx, rx);
			} 
			else{
				res = unite(find(x + 1, l, y, lx, rx), find(z, y + 1, r, lx, rx));
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
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		push(x, l, r);
		if (lx <= y) {
			update(x + 1, l, y, lx, rx, v);
		}
		if (rx > y) {
			update(z, y + 1, r, lx, rx, v);
		}
		pull(x, z);
	}
	template <typename M>
	void add(int x, int l, int r, int lx, int rx, const M& v) {
		if (lx <= l && r <= rx) {
			tree[x].push(l, r, v);
			return;
		}
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		push(x, l, r);
		if (lx <= y) {
			add(x + 1, l, y, lx, rx, v);
		}
		if (rx > y) {
			add(z, y + 1, r, lx, rx, v);
		}
		pull(x, z);
	}
	segtree(int _n) : n(_n) {
		assert(n > 0);
		tree.resize(2 * n - 1);
		build(0, 0, n - 1);
	}
	template <typename M>
	segtree(const vector<M> &v) {
		n = v.size();
		assert(n > 0);
		tree.resize(2 * n - 1);
		build(0, 0, n - 1, v);
	}
	node find(int p) { // value at index p
		assert(0 <= p && p <= n - 1);
		return find(0, 0, n - 1, p, p);
	}
	template <typename M>
	void update(int lx, int rx, const M& v) {
		assert(0 <= lx && lx <= rx && rx <= n - 1);
		update(0, 0, n - 1, lx, rx, v);
	}
	// All functions below
	template <typename M>
	void update(int i, const M& v){ // Sets value at index i to v
		assert(i >= 0 && i < n);
		update(i,i,v);
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
