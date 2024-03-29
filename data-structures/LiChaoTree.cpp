template<class int64_t, class uint, uint L, uint R, bool FLIP = false>
struct LiChaoTree {
	struct Line {
		int64_t m, c;
		constexpr Line (const int64_t _m, const int64_t _c): m(_m), c(_c) {}
		constexpr Line () {}
		inline int64_t operator() (uint i) const { return m*i + c; }
	};
	static constexpr int64_t INF = std::numeric_limits<int64_t>::max();
	static inline const Line max{0, INF};
	
	struct Node {
		Line z;
		Node *l, *r;
		constexpr Node (const Line& x): z(x), l(nullptr), r(nullptr) {}
		constexpr Node () {}
	} *root = create(max);
	
	Node* create(Line x) {
		static constexpr size_t FIXED_SIZE = 2 * 200000 << 5;
		static Node memo[FIXED_SIZE];
		static int nx = 0;
		memo[nx++].z = x;
		return &memo[nx-1];
	}
	
	void insert_line (const int64_t m, const int64_t c) { const Line x(m, c); up(x, root, L, R, x(L), x(R-1)); }
	void up (const int64_t m, const int64_t c, const uint ql, const uint qr) {
		const Line x(m, c);
		auto call = [&](auto&& self, Node* a, uint l, uint r) -> void {
			if (l >= qr or r <= ql) return;
			if (l >= ql and r <= qr) return (up (x, a, l, r, x(l), x(r-1)));       
			const uint mid = l + (r - l) / 2;
			if (ql < m){ if (!a->l) a->l = create(max); call(call, a->l, l, mid); }
			if (qr > m){ if (!a->r) a->r = create(max); call(call, a->r, mid, r); }
		};
		call(call, root, L, R);
	}
	
	void up (Line x, Node* a, uint l, uint r, int64_t x_l, int64_t x_r) {
		if constexpr (FLIP) x.m = -x.m, x.c = -x.c;
		while(1) {
			const int64_t z_l = a->z(l), z_r = a->z(r-1);
			if (x_l <= z_l and x_r <= z_r) return void(a->z = x);
			if (x_l >= z_l and x_r >= z_r) return;
			const uint m = l + (r - l) / 2;
			int64_t z_m = a->z(m-1), x_m = x(m-1);
			if (x_l > z_l) {
				if (x_m < z_m) std::swap(x, a->z), r=m, x_l=z_l, x_r=z_m;
				else l=m, x_l=x_m+x.m, x_r=x_r;
			} else {                
				if (x_m+x.m < z_m+a->z.m) std::swap(x, a->z), l=m, x_l=z_m+a->z.m, x_r=z_r;
				else r=m, x_l=x_l, x_r=x_m;
			}
			if(r == m) { if (!(a -> l)) a -> l = create(x); a = a -> l; }
			if(l == m) { if (!(a -> r)) a -> r = create(x); a = a -> r; }
		}
	}
	
	int64_t find(uint i) { 
		uint l = L, r = R;
		int64_t z = INF;
		for (auto a = root; a; ) {
			z = std::min(z, a -> z(i));
			const uint m = l + (r - l) / 2;
			if (i < m) r = m, a = a->l;
			else l = m, a = a->r;
		}
		if constexpr (FLIP) z = -z;
		return z;
	}
};

// LiChaoTree<y_type, x_type, -range, range, FLIP >

// Set FLIP = TRUE for max query and FLIP = FALSE for min query
// All x lie in [-range,range]

// LiChaoTree.insert_line(m,c) : Adds line y = mx + c
// LiChaoTree.find(x)          : Find min at x
