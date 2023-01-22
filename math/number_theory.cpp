// हर हर महादेव
#include <bits/stdc++.h>
using namespace std;

// -------------------------------Constants------------------------------------------ //
const int N = 1e6 + 60;
vector<int> phi_array;
vector<int> smallest_factor;
vector<bool> prime;
vector<int> primes;


// -------------------------------Preprocess------------------------------------------ //
void phi_1_to_n(int n) {
	phi_array.assign(N + 1, 0);
    for (int i = 0; i <= n; i++)
        phi_array[i] = i;

    for (int i = 2; i <= n; i++) {
        if (phi_array[i] == i) {
            for (int j = i; j <= n; j += i)
                phi_array[j] -= phi_array[j] / i;
        }
    }
}

void sieve(){
	int maximum = N;
	smallest_factor.assign(maximum + 1, 0);
	prime.assign(maximum + 1, true);
	prime[0] = prime[1] = false;
	primes = {};
	for(int p = 2; p <= maximum; p++){
		if(prime[p]) {
			smallest_factor[p] = p;
			primes.push_back(p);
			for(int i = (int)(p) * p; i <= maximum; i += p){
				if (prime[i]) {
					prime[i] = false;
					smallest_factor[i] = p;
				}
			}
		}
	}
}

void prep(){
	phi_1_to_n(N);
	sieve();
}

// -------------------------------Functions------------------------------------------ //
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

int64_t phi_calc(int64_t n) {
	int64_t result = n;
	for(int i : primes){
		if((int64_t)i*i > n){
			break;
		}
		if (n % i == 0) {
			while (n % i == 0)
				n /= i;
			result -= result / i;
		}
	}
	if (n > 1)
		result -= result / n;
	return result;
}

int64_t phi(int64_t x){
	return (x < N ? phi_array[x] : phi_calc(x));
}

int64_t powmod(int64_t x, int64_t y,int64_t mod){
	int64_t res = 1;
	x = x % mod;
	if (x == 0) return 0;
	while (y > 0){
		if (y & 1)
			res = (res*x) % mod;
		y = y >> 1;
		x = (x*x) % mod;
	}
	return res;
}

int64_t inv(int64_t a, int64_t m) {
	int64_t u = 0, v = 1;
	int64_t old = m;
	while (a != 0) {
		int64_t t = m / a;
		m -= t * a; swap(a, m);
		u -= t * v; swap(u, v);
	}
	assert(m == 1);
	if(u < 0){
		u += old;
	}
	return u;
}


// x mod m[i] == a[i] 
// x == a[i] mod m[i]
int64_t crt(vector<int> a,vector<int> m){
	int n = a.size();
	int64_t prod = 1;
	vector<int> M(n);
	vector<int> y(n);
	for(int i = 0; i < n; i++){
		prod *= m[i];
	}
	for(int i = 0; i < n; i++){
		M[i] = prod / m[i];
		y[i] = inv(M[i],m[i]);
	}
	int64_t x = 0;
	for(int i = 0; i < n; i++){
		int64_t here = (a[i] * y[i]) % prod;
		(here *= M[i]) %= prod;
		(x += here) %= prod;
	}
	return x;
}


int primitive(int p) {
    vector<int> fact;
    int phi = p-1,  n = phi;
    for (int i=2; i*i<=n; ++i)
        if (n % i == 0) {
            fact.push_back (i);
            while (n % i == 0)
                n /= i;
        }
    if (n > 1)
        fact.push_back (n);

    for (int res = 2; res <= p; ++res) {
        bool ok = true;
        for (size_t i = 0; i < fact.size() && ok; ++i)
            ok &= powmod (res, phi / fact[i], p) != 1;
        if (ok)  return res;
    }
    return -1;
}

// ax + by == gcd(a,b)
int ex_gcd(int a, int b, int& x, int& y) {
    x = 1, y = 0;
    int x1 = 0, y1 = 1, a1 = a, b1 = b;
    while (b1) {
        int q = a1 / b1;
        tie(x, x1) = make_tuple(x1, x - q * x1);
        tie(y, y1) = make_tuple(y1, y - q * y1);
        tie(a1, b1) = make_tuple(b1, a1 - q * b1);
    }
    return a1;
}

int ord(int x,int p){
	if(__gcd(x,p) != 1)return -1;
	int ph = phi(p);
	int res = p+1;
	
	auto check = [&](int y){
		int prod = powmod(x,y,p);
		if(prod == 1){
			res = min(res,y);
		}
	};
	
	for(int i = 1; i*i <= ph; i++){
		if(ph % i == 0){
			check(i);
			check(ph/i);
		}
	}
	return res;
}

bool diophantine(int a, int b, int c, int &x0, int &y0) {
    int g = ex_gcd(abs(a), abs(b), x0, y0);
    if (c % g) {
        return false;
    }

    x0 *= c / g;
    y0 *= c / g;
    if (a < 0) x0 = -x0;
    if (b < 0) y0 = -y0;
    return true;
}

bool is_prime(int64_t x){
	if(x == 0 || x == 1)return false;
	int64_t sieve_max = (int64_t)(smallest_factor.size()) - 1;
	assert(x <= sieve_max * sieve_max);
	if(x < N){
		return prime[x];
	}
	for(int i : primes){
		if((int64_t)i * i > x){
			break;
		}
		if(x * i == 0){
			return false;
		}
	}
	return true;
}

int dis_log(int a, int b, int m) { // a^x == b modulo m
    a %= m, b %= m;
    int n = sqrt(m) + 1;
    map<int, int> vals;
    for (int p = 1; p <= n; ++p)
        vals[powmod(a, p * n, m)] = p;
    for (int q = 0; q <= n; ++q) {
        int cur = (powmod(a, q, m) * 1ll * b) % m;
        if (vals.count(cur)) {
            return vals[cur] * n - q;
        }
    }
    return -1;
}
// -------------------------------Single Instances------------------------------------------ //
void diophantine_one(int n = -1,int a = -1,int b = -1){ // ax + by == n
	if(n == -1){
		cin >> a >> b >> n;
	}
	cout << "Solving for " << a << "x + " << b << "y == " << n << endl;
	
	int x,y;
	
	if(!diophantine(a,b,n,x,y)){
		cout << "No Solution Exists" << endl;
	}
	else{
		cout << "x = " << x << "  y = " << y << endl;
	}
}


void ex_gcd_one(){
	int a,b;
	cin >> a >> b;
	int g = __gcd(a,b);
	diophantine_one(g,a,b);
}

void ord_one(){
	int x,p;
	cin >> x >> p;
	cout << "Calculating ord_" << p << "(" << x << ")" << endl;
	cout << ord(x,p) << endl;
}

void inv_one(){
	int x,p;
	cin >> x >> p;
	
	cout << "Calculating inv_" << p << "(" << x << ")" << endl;
	if(__gcd(x,p) != 1){
		cout << "Inverse doesn't exist" << endl;
		return;
	}
	
	cout << inv(x,p) << endl;
	
}

// x mod m[i] == a[i] 
// x == a[i] mod m[i]
void crt_one(){
	int n;
	cin >> n;
	vector<int> a(n),m(n);
	
	for(int i = 0; i < n; i++){
		cin >> m[i] >> a[i];
	}
	
	cout << "Solving the following system" << endl;
	
	for(int i = 0; i < n; i++){
		cout << "x mod " << m[i] << " == " << a[i] << endl;
	}
	int x = crt(a,m);
	cout << "x = " << x << endl;
}

void primitive_one(){
	int p;
	cin >> p;
	cout << "Finding a primitive of " << p << endl;
	cout << primitive(p) << endl;
}

void powmod_one(){
	int x,y,m;
	cin >> x >> y >> m;
	cout << "Calculating " << x << "^" << y << " mod " << m << endl;
	cout << powmod(x,y,m) << endl;
}

void phi_one(){
	int x;
	cin >> x;
	cout << "Calculating phi(" << x << ")" << endl;
	cout << phi(x) << endl;
}

void prime_one(){
	int x;
	cin >> x;
	cout << x << " is " << (is_prime(x) ? "prime" : "not prime") << endl;
}

// -------------------------------Main Program ------------------------------------------ //
int32_t main(){
	prep();
	ios::sync_with_stdio(false);cin.tie(0);
	
	while(true){
		string s;
		cin >> s;
		
		cout << "-----------------------------" << endl;
		
		if(s == "crt"){
			crt_one();
		}
		else if(s == "dio"){
			diophantine_one();
		}
		else if(s == "prime"){
			prime_one();
		}
		else if(s == "exgcd"){
			ex_gcd_one();
		}
		else if(s == "ord"){
			ord_one();
		}
		else if(s == "inv"){
			inv_one();
		}
		else if(s == "primitive"){
			primitive_one();
		}
		else if(s == "pow"){
			powmod_one();
		}
		else if(s == "phi"){
			phi_one();
		}
		else if(s == "-1"){
			break;
		}
		else{
			cout << "Not recoganized" << endl;
		}
		cout << "-----------------------------" << endl;
	}
	return 0;
}




