const double EPS = 1e-9;
const int INF = 2; // it doesn't actually have to be infinity or a big number

template <typename T>
int gauss (vector<vector<T>> a, vector<T> &ans) { 
	int n = (int) a.size();
	int m = (int) a[0].size() - 1;
	vector<int> where (m, -1);
	for(int col = 0, row = 0; col < m && row < n; ++col){
		int sel = row;
		if(is_same<T,double>::value){
			for(int i = row; i < n; ++i)
				if(abs(a[i][col]) > abs(a[sel][col]))
					sel = i;
			if(abs(a[sel][col]) < EPS)
				continue;
		}
		else{
			if(a[sel][col] == 0){
				continue;
			}
		}
		for(int i = col;i <= m; ++i)
			swap (a[sel][i], a[row][i]);
		where[col] = row;
	
		for(int i = 0; i < n; ++i){
			if(i != row) {
				T c = a[i][col] / a[row][col];
				for(int j = col; j <= m; ++j)
					a[i][j] -= a[row][j] * c;
			}
		}
		++row;
	}
	ans.assign (m, 0);
	for(int i = 0; i < m; ++i)
		if(where[i] != -1)
			ans[i] = a[where[i]][m] / a[where[i]][i];
	for(int i = 0; i < n; ++i) {
		T sum = 0;
		for(int j = 0; j < m; ++j)
			sum += ans[j] * a[i][j];
		if(is_same<T,double>::value){
			if(abs(sum - a[i][m]) > EPS)
				return 0;
		}
		else{
			if(sum != a[i][m]){
				return 0;
			}
		}
	}
	for(int i = 0; i < m; ++i)
		if(where[i] == -1)
			return INF;
	return 1;
}
// solves Ax = B
// B is the last column of A
