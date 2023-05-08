
// c[i] = sum_{j | k == i} a[j] * b[k]
// size of array must be 2 ^ n

template<typename T>
vector<T> or_convolution(vector<T> a,vector<T> b,int n){
	
	assert((int)a.size() == 1 << n);
	assert((int)b.size() == 1 << n);
	
	for(int i = 0; i < n; i++){
		for(int j = 0; j < (1 << n); j++){
			if(1 << i & j){
				a[j] += a[j ^ (1 << i)];
				b[j] += b[j ^ (1 << i)];
			}
		}
	}
	
	vector<T> c(1 << n);
	
	for(int i = 0; i < (1 << n); i++){
		c[i] = a[i] * b[i];
	}
	
	for(int i = n-1; i >= 0; i--){
		for(int j = (1 << n) - 1; j >= 0; j--){
			if(1 << i & j){
				c[j] -= c[j ^ (1 << i)];
			}
		}
	}
	
	return c;
}

template<typename T>
void bit_flip(vector<T> &a,int n){
	vector<T> c(1 << n);
	
	for(int i = 0; i < (1 << n); i++){
		c[i ^ ((1 << n) - 1)] = a[i];
	}
	
	a = c;
}


// c[i] = sum_{j & k == i} a[j] * b[k]
// size of array must be 2 ^ n
template<typename T>
vector<T> and_convolution(vector<T> a,vector<T> b,int n){
	
	bit_flip(a,n); bit_flip(b,n);
	
	auto c = or_convolution(a,b,n);
	
	bit_flip(c,n);
	
	return c;
}
