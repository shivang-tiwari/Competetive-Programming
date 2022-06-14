template <typename vector_type, typename ret_type>
class merge_sort_tree{
  public:
	struct node{
		int N;
		int L;
		int R;
		vector<vector_type> vec;
		void single(vector_type v){
			vec = {v};
			N = 1;
		}
		ret_type query(vector_type val){ // Indices of the original array
			ret_type res{};
			// Query for val in this node
			return res;
		}
	};
	
	ret_type combine(const ret_type &x , const ret_type &y) const { // Combining the result of two queries
		return x + y;
	}
	
	int n;
	vector<node> tree;
	node merge(const node &x, const node &y) const {
		node res;
		res.L = x.L;
		res.R = y.R;
		res.N = x.N + y.N;
		int i = 0,j = 0;
		while(i < (int)x.vec.size() || j < (int)y.vec.size()){
			if(i == (int)x.vec.size()){
				res.vec.push_back(y.vec[j++]);
			}
			else if(j == (int)y.vec.size()){
				res.vec.push_back(x.vec[i++]);
			}
			else{
				res.vec.push_back(x.vec[i] < y.vec[j] ? x.vec[i++] : y.vec[j++]);
			}
		}
		return res;
	}
	void pull(int Parent,int Right){
		tree[Parent] = merge(tree[Parent+1],tree[Right]);
	}
	void build(int x, int l, int r, const vector<vector_type> &v) {
		if (l == r) {
			tree[x].single(v[l]);
			tree[x].L = l;
			tree[x].R = r;
			return;
		}
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		build(x + 1, l, y, v);
		build(z, y + 1, r, v);
		pull(x, z);
	}
	ret_type find(int x, int l, int r, int lx, int rx,vector_type val) {
		if(lx <= l && r <= rx){ // completly within range
			return tree[x].query(val);
		}
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		ret_type res{};
		if(rx <= y){
			res = find(x + 1, l, y, lx, rx,val);
		}
		else{
			if(lx > y){
				res = find(z, y + 1, r, lx, rx,val);
			}
			else{
				res = combine(find(x + 1, l, y, lx, rx,val), find(z, y + 1, r, lx, rx,val));
			}
		}
		return res;
	}
	merge_sort_tree(const vector<vector_type> &v){
		n = v.size();
		assert(n > 0);
		tree.resize(2 * n - 1);
		build(0, 0, n - 1, v);
	}
	ret_type find(int l, int r,vector_type val) { // querying on [l .. r]
		assert(0 <= l && l <= r && r <= n - 1);
		return find(0, 0, n - 1, l, r,val);
	}
};
