class segtree {
 public:
	struct node {
		// don't forget to set default value (used for leaves)
		// not necessarily neutral element!
		int sum = 0; // Set identity element
		int add = 0; 

		void apply(int l, int r, int v) { // Value of a single node (used by build and update)
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
		//if (tree[x].add != 0) {
			//tree[x + 1].push(l, y, tree[x].add);
			//tree[z].push(y + 1, r, tree[x].add);
			//tree[x].add = 0;
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
		if (rx <= y) {
			res = find(x + 1, l, y, lx, rx);
		} else {
			if (lx > y) {
			res = find(z, y + 1, r, lx, rx);
			} else {
			res = unite(find(x + 1, l, y, lx, rx), find(z, y + 1, r, lx, rx));
			}
		}
		pull(x, z);
		return res;
	}
	template <typename... M>
	void update(int x, int l, int r, int lx, int rx, const M&... v) {
		if (lx <= l && r <= rx) {
			tree[x].apply(l, r, v...);
			return;
		}
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		push(x, l, r);
		if (lx <= y) {
			update(x + 1, l, y, lx, rx, v...);
		}
		if (rx > y) {
			update(z, y + 1, r, lx, rx, v...);
		}
		pull(x, z);
	}
	template <typename... M>
	void add(int x, int l, int r, int lx, int rx, const M&... v) {
		if (lx <= l && r <= rx) {
			tree[x].push(l, r, v...);
			return;
		}
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		push(x, l, r);
		if (lx <= y) {
			add(x + 1, l, y, lx, rx, v...);
		}
		if (rx > y) {
			add(z, y + 1, r, lx, rx, v...);
		}
		pull(x, z);
	}
	int find_first_knowingly(int x, int l, int r, const function<bool(const node&)> &f) {
		if (l == r) {
			return l;
		}
		push(x, l, r);
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		int res;
		if (f(tree[x + 1])) {
			res = find_first_knowingly(x + 1, l, y, f);
		}
		else {
			res = find_first_knowingly(z, y + 1, r, f);
		}
		pull(x, z);
		return res;
	}
	int find_first(int x, int l, int r, int lx, int rx, const function<bool(const node&)> &f) {
		if (lx <= l && r <= rx) {
			if (!f(tree[x]))return -1;
			return find_first_knowingly(x, l, r, f);
		}
		push(x, l, r);
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		int res = -1;
		if (lx <= y) {
			res = find_first(x + 1, l, y, lx, rx, f);
		}
		if (rx > y && res == -1) {
			res = find_first(z, y + 1, r, lx, rx, f);
		}
		pull(x, z);
		return res;
	}
	int find_last_knowingly(int x, int l, int r, const function<bool(const node&)> &f) {
		if (l == r)return l;
		push(x, l, r);
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		int res;
		if (f(tree[z]))res = find_last_knowingly(z, y + 1, r, f);
		else res = find_last_knowingly(x + 1, l, y, f);
		pull(x, z);
		return res;
	}
	int find_last(int x, int l, int r, int lx, int rx, const function<bool(const node&)> &f) {
		if (lx <= l && r <= rx) {
			if (!f(tree[x]))return -1;
			return find_last_knowingly(x, l, r, f);
		}
		push(x, l, r);
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		int res = -1;
		if (rx > y) {
			res = find_last(z, y + 1, r, lx, rx, f);
		}
		if (lx <= y && res == -1) {
			res = find_last(x + 1, l, y, lx, rx, f);
		}
		pull(x, z);
		return res;
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
	template <typename... M>
	void update(int lx, int rx, const M&... v) {
		assert(0 <= lx && lx <= rx && rx <= n - 1);
		update(0, 0, n - 1, lx, rx, v...);
	}
	void add(int i, int v){
		assert(i >= 0 && i < n);
		add(i,i,v);
	}
	// All functions below
	void update(int i, int v){ // Sets value at index i to v
		assert(i >= 0 && i < n);
		update(i,i,v);
	}
	template <typename... M>
	void add(int lx, int rx, const M&... v) { // adds v to a[lx to rx]
		assert(0 <= lx && lx <= rx && rx <= n - 1);
		add(0, 0, n - 1, lx, rx, v...);
	}
	node find(int lx, int rx) { // value of lx to rx
		assert(0 <= lx && lx <= rx && rx <= n - 1);
		return find(0, 0, n - 1, lx, rx);
	}
	// find_first and find_last call all FALSE elements
	// to the left (right) of the sought position exactly once
	int find_first(int lx, int rx, const function<bool(const node&)> &f) {
		assert(0 <= lx && lx <= rx && rx <= n - 1);
		return find_first(0, 0, n - 1, lx, rx, f);
	}
	int find_last(int lx, int rx, const function<bool(const node&)> &f) {
		assert(0 <= lx && lx <= rx && rx <= n - 1);
		return find_last(0, 0, n - 1, lx, rx, f);
	}
};
