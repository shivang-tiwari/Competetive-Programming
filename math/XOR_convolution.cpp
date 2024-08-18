vector<Mint> xormul(vector<Mint> &a, vector<Mint> &b) {
	int m = (int) a.size();
	assert((int)b.size() == m);
	vector<Mint> c(m);
	for(int n = m / 2; n > 0; n /= 2){
		for(int i = 0; i < m; i += 2 * n){
			for(int j = 0; j < n; j++) {
				Mint x = a[i + j], y = a[i + j + n];
				a[i + j] = (x + y);
				a[i + j + n] = (x - y);
			}
		}
	}
	for(int n = m / 2; n > 0; n /= 2){
		for(int i = 0; i < m; i += 2 * n){
			for(int j = 0; j < n; j++) {
				Mint x = b[i + j], y = b[i + j + n];
				b[i + j] = (x + y);
				b[i + j + n] = (x - y);
			}
		}
	}
	
	for(int i = 0; i < m; i++){
		c[i] = a[i] * b[i];
	}
	
	for(int n = 1; n < m; n *= 2){
		for(int i = 0; i < m; i += 2 * n){
			for(int j = 0; j < n; j++) {
				Mint x = c[i + j], y = c[i + j + n];
				c[i + j] = (x + y);
				c[i + j + n] = (x - y);
			}
		}
	}
	
	Mint mrev = (Mint)1 / m;
	for(int i = 0; i < m; i++){
		c[i] = c[i] * mrev;
	}
	
	return c;
}
/*
Sizes of a and b must be 1 << bit

Returns c, such that

c[i] = sum_{j ^ k == i} a[j] * b[k]

*/
