template<typename T> struct rmq {
	vector<T> v; int n;
	static const int b = 30; // block size
	vector<int> mask, t; // mask and sparse table
	
	int op(int x, int y) {
		return v[x] < v[y] ? x : y;
	}
	int lsb(int x) {
		return x & -x;
	}
	int msb_index(int x) {
		return __builtin_clz(1) - __builtin_clz(x);
	}
	int small(int r, int size = b) {
		int dist_from_r = msb_index(mask[r] & ((1 << size) - 1));
		return r - dist_from_r;
	}
	rmq(const vector<T>& v_) : v(v_), n(v.size()), mask(n), t(n) {
		int curr_mask = 0;
		for (int i = 0; i < n; i++) {
			curr_mask = (curr_mask<<1) & ((1<<b)-1);
			while (curr_mask > 0 and op(i, i - msb_index(lsb(curr_mask))) == i) {
				curr_mask ^= lsb(curr_mask);
			}
			curr_mask |= 1;
			mask[i] = curr_mask;
		}
		for (int i = 0; i < n / b; i++) t[i] = small(b * i + b - 1);
		for (int j = 1; (1 << j) <= n / b; j++) for (int i = 0; i + (1 << j) <= n / b; i++)
			t[n/b*j+i] = op(t[n/b*(j-1)+i], t[n/b*(j-1)+i+(1<<(j-1))]);
	}
	T query(int l, int r) {
		if (r-l+1 <= b) return v[small(r, r-l+1)];
		int ans = op(small(l+b-1), small(r));
		int x = l / b + 1, y = r / b - 1;
		if (x <= y) {
			int j = msb_index(y-x+1);
			ans = op(ans, op(t[n/b*j+x], t[n/b*j+y-(1<<j)+1]));
		}
		return v[ans];
	}
};
