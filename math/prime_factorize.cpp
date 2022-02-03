vector<int> smallest_factor;
vector<bool> prime;
vector<int> primes;
void prep(int maximum = 1e6) {
	maximum = max(maximum, 1);
	smallest_factor.assign(maximum + 1, 0);
	prime.assign(maximum + 1, true);
	prime[0] = prime[1] = false;
	primes = {};
	for(int p = 2; p <= maximum; p++){
		if(prime[p]) {
			smallest_factor[p] = p;
			primes.push_back(p);
			for(ll i = (ll)(p) * p; i <= maximum; i += p){
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
vector<pair<ll, int>> prime_factorize(ll n){ // Prime,exponent
	ll sieve_max = (ll)(smallest_factor.size()) - 1;
	assert(1 <= n && n <= sieve_max * sieve_max);
	vector<pair<ll, int>> result;
	if(n <= sieve_max){
		while(n != 1){
			ll p = smallest_factor[n];
			int exponent = 0;
			do{
				n /= p;
				exponent++;
			} while (n % p == 0);
			result.emplace_back(p, exponent);
		}
		return result;
	}
	for(ll p : primes){
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
