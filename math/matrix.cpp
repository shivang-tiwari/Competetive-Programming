template <typename T>
vector<vector<T>> multiply(vector<vector<T>> a, vector<vector<T>> b){
	assert(a[0].size() == b.size());
	int r1 = a.size(),c1 = a[0].size(),c2 = b[0].size();
	vector<vector<T>> res(r1,vector<T>(c1,0));
    for (int i = 0; i < r1; i++){
        for (int j = 0; j < c2; j++){
            for (int k = 0; k < c1; k++)
                res[i][j] += a[i][k]*b[k][j];
        }
    }
    return res;
}

template <typename T>vector<T> multiply(vector<vector<T>> a, vector<T> b){return multiply({b},a)[0];}

template <typename T>
vector<vector<T>> power(vector<vector<T>> x, int y){
	int n = x.size();
	vector<vector<T>> res(n,vector<T>(n));
	for(int i = 0; i < n; i++){
		res[i][i] = 1;
	}
	while (y > 0){
		if (y & 1)
			res = multiply(res,x);
		y = y >> 1;
		x = multiply(x,x);
	}
	return res;
}
