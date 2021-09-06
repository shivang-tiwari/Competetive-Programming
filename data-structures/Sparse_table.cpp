template<class T>
class sparse{
	public:
	int N, LG;
	vector<vector<T>>spt;
	vector<int>lg;
	sparse(){}
	sparse(vector<T> &a){
		N = a.size();
		LG = 32 - __builtin_clz(std::max(N - 1, 1));
		spt.resize(N+1, vector<T>(LG+1,0));
		lg.resize(N + 5,0);
		for(int i = 2;i <= N;i++)
			lg[i] = lg[i/2] + 1;
		build(a);
	}
	void build(vector<T> &a){
		for(int i = 0;i < N;i++)
			spt[i][0] = a[i];
		for(int j = 1;j <= LG;j++){
			int len = (1 << j), half = (1 << (j-1));
			for(int i = 0;i + len <= N;i++)
				spt[i][j] = combine(spt[i][j-1],spt[i+half][j-1]);
		}
	}
	T find(int l, int r){ // For idempotent operations in O(1)
		assert(l <= r && l >= 0 && r <= N-1);
		int lgg = lg[r-l+1], len = (1<<lgg);
		return combine(spt[l][lgg], spt[r-len+1][lgg]);
	}
	T query(int L, int R){ // For other operations in O(log n)
		T answer = 0;
		for (int j = LG; j >= 0; j--) {
			if (L + (1 << j) - 1 <= R) {
				answer = combine(answer,spt[L][j]);
				L += 1 << j;
			}
		}
		return answer;
	}
	T combine(T x,T y){
		return min(x,y);
	}
};
