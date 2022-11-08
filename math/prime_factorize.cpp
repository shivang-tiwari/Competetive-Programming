const int N = 1e6 + 50;
vector<int> smallest_factor;
vector<bool> prime;
vector<int> primes;
void prep(int maximum = N){
	maximum = max(maximum, 1);
	smallest_factor.assign(maximum + 1, 0);
	prime.assign(maximum + 1, true);
	prime[0] = prime[1] = false;
	primes = {};
	for(int p = 2;p <= maximum; p++){
		if(prime[p]) {
			smallest_factor[p] = p;
			primes.push_back(p);
			for(int64_t i = (int64_t)(p) * p; i <= maximum; i += p){
				if (prime[i]) {
					prime[i] = false;
					smallest_factor[i] = p;
				}
			}
		}
	}
}
// Prime factorizes n in worst case O(sqrt n / log n). Requires having run `sieve` up to at least sqrt(n).
// If we've run `sieve` up to at least n, takes O(log n) time.
vector<pair<int64_t, int>> prime_factorize(int64_t n){ // Prime,exponent
	int64_t sieve_max = (int64_t)(smallest_factor.size()) - 1;
	assert(1 <= n && n <= sieve_max * sieve_max);
	vector<pair<int64_t, int>> result;
	if(n <= sieve_max){
		while(n != 1){
			int64_t p = smallest_factor[n];
			int exponent = 0;
			do{
				n /= p;
				exponent++;
			} while (n % p == 0);
			result.emplace_back(p, exponent);
		}
		return result;
	}
	for(int64_t p : primes){
		if(p * p > n)
			break;
		if(n % p == 0){
			result.emplace_back(p, 0);
			do{
				n /= p;
				result.back().second++;
			} while (n % p == 0);
		}
	}
	if(n > 1)
		result.emplace_back(n, 1);
	return result;
}

vector<int64_t> get_factors(const vector<pair<int64_t,int>> &pf){
	vector<int64_t> res;
	
	function<void(int,int64_t)> dfs = [&](int yet,int i){
		if(i == (int)pf.size()){
			res.push_back(yet);
			return;
		}
		for(int x = 0;x <= pf[i].second;x++){
			dfs(yet,i+1);
			yet *= pf[i].first;
		}
	}; dfs(1,0);
	sort(res.begin(),res.end());
	return res;
}
vector<int64_t> get_factors(int x){
	return get_factors(prime_factorize(x));
}

vector<pair<int64_t,int>> combine(const vector<pair<int64_t,int>> &pf1,const vector<pair<int64_t,int>> &pf2){
	vector<pair<int64_t, int>> res;
	int n = pf1.size(),m = pf2.size();
	int i = 0,j = 0;
	while(i < n || j < m){
		if(i == n){
			res.push_back(pf2[j++]);
			continue;
		}
		if(j == m){
			res.push_back(pf1[i++]);
			continue;
		}
		if(pf1[i].first == pf2[j].first){
			res.push_back({pf1[i].first,pf1[i].second + pf2[j].second});
			i++;j++;
		}
		else{
			res.push_back(pf1[i].first < pf2[j].first ? pf1[i++] : pf2[j++]);
		}
	}
	return res;
}
