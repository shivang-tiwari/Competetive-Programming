template <typename T>
class fenwick {
  public:
	vector<T> fenw;
	int n;
	fenwick(int _n) : n(_n) {
		fenw.resize(n);
	}
	void add(int x, T v) {
		while (x < n) {
			combine(fenw[x],v);
			x |= (x + 1);
		}
	}
	T find(int x) {
		T v{};
		while (x >= 0) {
			combine(v,fenw[x]);
			x = (x & (x + 1)) - 1;
		}
		return v;
	}
	T find(int l,int r) {
		return find(r) - (l == 0 ? 0 : find(l-1));
	}
	void combine(T &v, T val){
		 v += val;
	}
};

