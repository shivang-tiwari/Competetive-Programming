template<typename T>
vector<vector<T>> multiply(const vector<vector<T>> &a, const vector<vector<T>> &b) {
	int N = a.size(), M = a[0].size(), K = b[0].size();
	assert(M == (int)b.size());
	vector<vector<T>> c(N,vector<T>(K));
	for(int i = 0; i < N; i++) {
		for(int k = 0; k < M; k++) {
			for(int j = 0; j < K; j++) {
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	return c;
}


template <typename T, typename U>
vector<vector<T>> power(const vector<vector<T>> &x, U y){
	int n = x.size();
	assert(n == (int)x[0].size());
	vector<vector<T>> res(n,vector<T>(n));
	for(int i = 0; i < n; i++){
		res[i][i] = 1;
	}
	while(y > 0){
		if(y & 1)
			res = multiply(res,x);
		y = y >> 1;
		x = multiply(x,x);
	}
	return res;
}
