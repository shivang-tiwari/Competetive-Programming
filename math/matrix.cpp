template<typename T>
struct matrix{
	int n;
	int m;
	vector<vector<T>> a;
	
	matrix(int _n,int _m){
		n = _n;
		m = _m;
		a = vector<vector<T>> (n,vector<T>(m));
	}
	
	matrix operator + (const matrix &oth){
		assert(n == oth.n && m == oth.m);
		matrix<T> res;
		for(int i = 0; i < n; i++){
			for(int j = 0; j < m; j++){
				res[i][j] = a[i][j] + oth.a[i][j];
			}
		}
		return res;
	}
	void operator += (const matrix &oth){
		assert(n == oth.n && m == oth.m);
		for(int i = 0; i < n; i++){
			for(int j = 0; j < m; j++){
				a[i][j] += oth.a[i][j];
			}
		}
	}
	matrix operator - (const matrix &oth){
		assert(n == oth.n && m == oth.m);
		matrix<T> res;
		for(int i = 0; i < n; i++){
			for(int j = 0; j < m; j++){
				res[i][j] = a[i][j] - oth.a[i][j];
			}
		}
		return res;
	}
	void operator -= (const matrix &oth){
		assert(n == oth.n && m == oth.m);
		for(int i = 0; i < n; i++){
			for(int j = 0; j < m; j++){
				a[i][j] -= oth.a[i][j];
			}
		}
	}
	matrix operator * (const matrix &oth){
		assert(m == oth.n);
		matrix<T> res(n,oth.m);
		for(int i = 0; i < n; i++){
			for(int j = 0; j < oth.m; j++){
				for(int k = 0; k < m; k++){
					res[i][j] += a[i][k] * oth.a[k][j];
				}
			}
		}
		return res;
	}
	
	void operator *= (const matrix &oth){
		assert(m == oth.n);
		matrix<T> res(n,oth.m);
		for(int i = 0; i < n; i++){
			for(int j = 0; j < oth.m; j++){
				for(int k = 0; k < m; k++){
					res[i][j] += a[i][k] * oth.a[k][j];
				}
			}
		}
		a = res.a;
		n = res.n;
		m = res.m;
	}
	
	
	// Scalar operations
	matrix operator * (const int64_t num){
		matrix<T> res(n,m);
		for(int i = 0; i < n; i++){
			for(int j = 0; j < m; j++){
				res[i][j] = a[i][j] * num;
			}
		}
	}
	matrix operator *= (const int64_t num){
		for(int i = 0; i < n; i++){
			for(int j = 0; j < m; j++){
				a[i][j] *= num;
			}
		}
	}
	vector<T>& operator [] (int i) {return a[i];}
};

template<typename T>
matrix<T> power(const matrix<T> &x, int64_t y){
	assert(x.n == x.m);
	matrix<T> res(x.n,x.n);
	for(int i = 0; i < x.n; i++){
		res[i][i] = 1;
	}
	auto b = x;
	while(y > 0){
		if(y & 1){
			res *= b;
		}
		y = y >> 1;
		b *= b;
	}
	return res;
}
