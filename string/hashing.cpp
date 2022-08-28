template<typename T_string = string>
struct string_hash{
	int N;
	string s;
	const int p = 31; // lowercase
	//const int p = 53; // lower + upper
	
	const int start = 'a';
	
	const int64_t MOD = 1e9 + 9;
	
	vector<int64_t> hsh;
	vector<int64_t> rev;
	
	string_hash(T_string str){
		N = str.size();
		hsh.resize(N);
		rev.resize(N);
		s = str;
		int64_t yet = 0;
		int64_t pp = 1;
		for(int i = 0; i < N; i++){
			(yet += pp * (s[i] - start + 1)) %= MOD;
			(pp *= p) %= MOD;
			hsh[i] = yet;
		}
		pp = 1;
		yet = 0;
		for(int i = N-1; i >= 0; i--){
			(yet += pp * (s[i] - start + 1)) %= MOD;
			(pp *= p) %= MOD;
			rev[i] = yet;
		}
	}
	int64_t find_hash(int l,int r){
		if(l <= r){
			assert(l >= 0 && r < N);
			int64_t res = ((hsh[r] - (l == 0 ? 0 : hsh[l-1])) * inv(power(p,l))) % MOD;
			return res < 0 ? res + MOD : res;
		}
		else{
			swap(l,r);
			assert(l >= 0 && r < N);
			int64_t res = ((rev[l] - (r == N-1 ? 0 : rev[r+1])) * inv(power(p,N-r-1))) % MOD;
			return res < 0 ? res + MOD : res;
		}
		
	}
	int64_t find_hash_scaled(int l,int r){ // Does not divide, scales the string to N
		if(l <= r){
			assert(l >= 0 && r < N);
			int64_t res = ((hsh[r] - (l == 0 ? 0 : hsh[l-1])) * power(p,N-r-1)) % MOD;
			return res < 0 ? res + MOD : res;
		}
		else{
			swap(l,r);
			assert(l >= 0 && r < N);
			int64_t res = ((rev[l] - (r == N-1 ? 0 : rev[r+1])) * power(p,l)) % MOD;
			return res < 0 ? res + MOD : res;
		}
		
	}
	int64_t power(int64_t x, int64_t y){
		int64_t res = 1;
		x = x % MOD;
		if (x == 0) return 0;
		while (y > 0){
			if (y & 1)
				res = (res*x) % MOD;
			y = y >> 1;
			x = (x*x) % MOD;
		}
		return res;
	}
	int64_t inv(int64_t x){
		x %= MOD;
		assert(x != 0);
		return power(x,MOD-2);
	}
	int64_t divi(int64_t x,int64_t y){
		x %= MOD;
		y %= MOD;
		return (x * inv(y)) % MOD;
	}
	
	bool palindrome(int l = 0,int r = -1){
		if(r == -1){
			r = N-1;
		}
		return find_hash_scaled(l,r) == find_hash_scaled(r,l);
	}
};

template<typename T_string = string>
bool same(const string_hash<T_string> &v1, int l1,int r1, const string_hash<T_string> &v2,int l2,int r2){ // v1[l1, ... ,r1] == v2[l2 , ... ,r2]
	return v1.find_hash(l1,r1) == v2.find_hash(l2,r2);
}

template<typename T_string = string>
int64_t concat(string_hash<T_string> &v1, int l1,int r1, string_hash<T_string> &v2,int l2,int r2){ // concatenates v1[l1, ... ,r1] and v2[l2, ... ,r2]
	assert(v1.MOD == v2.MOD);
	assert(v1.p == v2.p);
	int64_t res = v2.find_hash(l2,r2);
	(res *= v1.power(v1.p,max(r1,l1)-min(r1,l1)+1)) %= v1.MOD;
	return (res + v1.find_hash(l1,r1)) % v1.MOD;
}

// hash(s) = s[0] + p*s[1] + p^2*s[2] + ... + p^(n-1) s[n-1]
