const int mod = 998244353;

struct base {
	double x, y;
	base(){
		x = y = 0;
	}
	base(double a, double b){
		x = a;
		y = b;
	}
};
inline base operator + (base a, base b) { return base(a.x + b.x, a.y + b.y); }
inline base operator - (base a, base b) { return base(a.x - b.x, a.y - b.y); }
inline base operator * (base a, base b) { return base(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x); }
inline base conj(base a) { return base(a.x, -a.y); }
int lim = 1;
vector<base> roots = {{0, 0}, {1, 0}};
vector<int> rev = {0, 1};
const double PI = acosl(- 1.0);
void ensure_base(int p){
	if(p <= lim) return;
	rev.resize(1 << p);
	for(int i = 0; i < (1 << p); i++)
		rev[i] = (rev[i >> 1] >> 1) + ((i & 1)  <<  (p - 1));
	roots.resize(1 << p);
	while(lim < p) {
		double angle = 2 * PI / (1 << (lim + 1));
		for(int i = 1 << (lim - 1); i < (1 << lim); i++){
			roots[i << 1] = roots[i];
			double angle_i = angle * (2 * i + 1 - (1 << lim));
			roots[(i << 1) + 1] = base(cos(angle_i), sin(angle_i));
		}
		lim++;
	}
}
void fft(vector<base> &a, int n = -1) {
	if(n == -1) n = a.size();
	assert((n & (n - 1)) == 0);
	int zeros = __builtin_ctz(n);
	ensure_base(zeros);
	int shift = lim - zeros;
	for(int i = 0; i < n; i++)if(i < (rev[i] >> shift)) swap(a[i], a[rev[i] >> shift]);
	for(int k = 1; k < n; k <<= 1) {
		for(int i = 0; i < n; i += 2 * k) {
			for(int j = 0; j < k; j++) {
				base z = a[i + j + k] * roots[j + k];
				a[i + j + k] = a[i + j] - z;a[i + j] = a[i + j] + z;
			}
		}
	}
}
vector<int> multiply(vector<int> &a, vector<int> &b, bool eq = 0){
	int need = a.size() + b.size() - 1;
	int p = 0;
	while((1 << p) < need)p++;
	ensure_base(p);
	int sz = 1 << p;
	vector<base> A, B;
	if(sz > (int)A.size()) A.resize(sz);
	for(int i = 0; i < (int)a.size(); i++){
		int x = (a[i] % mod + mod) % mod;
		A[i] = base(x & ((1 << 15) - 1), x >> 15);
	}
	fill(A.begin() + a.size(), A.begin() + sz, base{0, 0});
	fft(A, sz);
	if(sz > (int)B.size()) B.resize(sz);
	if(eq) copy(A.begin(), A.begin() + sz, B.begin());
		else{
	for(int i = 0; i < (int)b.size(); i++){
			int x = (b[i] % mod + mod) % mod;
			B[i] = base(x & ((1 << 15) - 1), x >> 15);
		}
		fill(B.begin() + b.size(), B.begin() + sz, base{0, 0});
		fft(B, sz);
	}
	double ratio = 0.25 / sz;
	base r2(0,  - 1), r3(ratio, 0), r4(0,  - ratio), r5(0, 1);
	for(int i = 0; i <= (sz >> 1); i++) {
		int j = (sz - i) & (sz - 1);
		base a1 = (A[i] + conj(A[j])), a2 = (A[i] - conj(A[j])) * r2;
		base b1 = (B[i] + conj(B[j])) * r3, b2 = (B[i] - conj(B[j])) * r4;
		if(i != j) {
			base c1 = (A[j] + conj(A[i])), c2 = (A[j] - conj(A[i])) * r2;
			base d1 = (B[j] + conj(B[i])) * r3, d2 = (B[j] - conj(B[i])) * r4;
			A[i] = c1 * d1 + c2 * d2 * r5;
			B[i] = c1 * d2 + c2 * d1;
		}
		A[j] = a1 * b1 + a2 * b2 * r5;
		B[j] = a1 * b2 + a2 * b1;
	}
	fft(A, sz); fft(B, sz);
	vector<int> res(need);
	for(int i = 0; i < need; i++){
		long long aa = A[i].x + 0.5;
		long long bb = B[i].x + 0.5;
		long long cc = A[i].y + 0.5;
		res[i] = (aa + ((bb % mod) << 15) + ((cc % mod) << 30))%mod;
	}
	return res;
}

// NTT below, needs modular
template <typename T>
class NTT {
 public:
	using Type = typename decay<decltype(T::value)>::type;
	static Type md;
	static Modular<T> root;
	static int base;
	static int max_base;
	static vector<Modular<T>> roots;
	static vector<int> rev;
	static void clear() {
		root = 0;
		base = 0;
		max_base = 0;
		roots.clear();
		rev.clear();
	}
	static void init() {
		md = T::value;
		assert(md >= 3 && md % 2 == 1);
		auto tmp = md - 1;
		max_base = 0;
		while (tmp % 2 == 0) {
			tmp /= 2;
			max_base++;
		}
		root = 2;
		while (power(root, (md - 1) >> 1) == 1) {
			root++;
		}
		assert(power(root, md - 1) == 1);
		root = power(root, (md - 1) >> max_base);
		base = 1;
		rev = {0, 1};
		roots = {0, 1};
	}
	static void ensure_base(int nbase) {
		if (md != T::value) {
			clear();
		}
		if (roots.empty()) {
			init();
		}
		if (nbase <= base) {
			return;
		}
		assert(nbase <= max_base);
		rev.resize(1 << nbase);
		for (int i = 0; i < (1 << nbase); i++) {
			rev[i] = (rev[i >> 1] >> 1) + ((i & 1) << (nbase - 1));
		}
		roots.resize(1 << nbase);
		while (base < nbase) {
			Modular<T> z = power(root, 1 << (max_base - 1 - base));
			for (int i = 1 << (base - 1); i < (1 << base); i++) {
				roots[i << 1] = roots[i];
				roots[(i << 1) + 1] = roots[i] * z;
			}
			base++;
		}
	}
	static void fft(vector<Modular<T>> &a) {
		int n = (int) a.size();
		assert((n & (n - 1)) == 0);
		int zeros = __builtin_ctz(n);
		ensure_base(zeros);
		int shift = base - zeros;
		for(int i = 0; i < n; i++) {
			if (i < (rev[i] >> shift)) {
				swap(a[i], a[rev[i] >> shift]);
			}
		}
		for(int k = 1; k < n; k <<= 1) {
			for (int i = 0; i < n; i += 2 * k) {
				for (int j = 0; j < k; j++) {
					Modular<T> x = a[i + j];
					Modular<T> y = a[i + j + k] * roots[j + k];
					a[i + j] = x + y;
					a[i + j + k] = x - y;
				}
			}
		}
	}
	static vector<Modular<T>> multiply(vector<Modular<T>> a, vector<Modular<T>> b) {
		if(a.empty() || b.empty()) {
			return {};
		}
		int eq = (a == b);
		int need = (int) a.size() + (int) b.size() - 1;
		int nbase = 0;
		while ((1 << nbase) < need) nbase++;
		ensure_base(nbase);
		int sz = 1 << nbase;
		a.resize(sz);
		b.resize(sz);
		fft(a);
		if (eq) b = a; else fft(b);
		Modular<T> inv_sz = 1 / static_cast<Modular<T>>(sz);
		for (int i = 0; i < sz; i++) {
			a[i] *= b[i] * inv_sz;
		}
		reverse(a.begin() + 1, a.end());
		fft(a);
		a.resize(need);
		return a;
	}
};
template <typename T> typename NTT<T>::Type NTT<T>::md;
template <typename T> Modular<T> NTT<T>::root;
template <typename T> int NTT<T>::base;
template <typename T> int NTT<T>::max_base;
template <typename T> vector<Modular<T>> NTT<T>::roots;
template <typename T> vector<int> NTT<T>::rev;
template <typename T>
vector<Modular<T>> inverse(const vector<Modular<T>>& a) {
	assert(!a.empty());
	int n = (int) a.size();
	vector<Modular<T>> b = {1 / a[0]};
	while((int) b.size() < n){
		vector<Modular<T>> x(a.begin(), a.begin() + min(a.size(), b.size() << 1));
		x.resize(b.size() << 1);
		b.resize(b.size() << 1);
		vector<Modular<T>> c = b;
		NTT<T>::fft(c);
		NTT<T>::fft(x);
		Modular<T> inv = 1 / static_cast<Modular<T>>((int) x.size());
		for(int i = 0; i < (int) x.size(); i++){
			x[i] *= c[i] * inv;
		}
		reverse(x.begin() + 1, x.end());
		NTT<T>::fft(x);
		rotate(x.begin(), x.begin() + (x.size() >> 1), x.end());
		fill(x.begin() + (x.size() >> 1), x.end(), 0);
		NTT<T>::fft(x);
		for(int i = 0; i < (int) x.size(); i++){
			x[i] *= c[i] * inv;
		}
		reverse(x.begin() + 1, x.end());
		NTT<T>::fft(x);
		for(int i = 0; i < ((int) x.size() >> 1); i++){
			b[i + ((int) x.size() >> 1)] = -x[i];
		}
	}
	b.resize(n);
	return b;
}
template <typename T>
vector<Modular<T>> inverse_old(vector<Modular<T>> a) {
	assert(!a.empty());
	int n = (int) a.size();
	if(n == 1){
		return {1 / a[0]};
	}
	int m = (n + 1) >> 1;
	vector<Modular<T>> b = inverse(vector<Modular<T>>(a.begin(), a.begin() + m));
	int need = n << 1;
	int nbase = 0;
	while((1 << nbase) < need) {
		++nbase;
	}
	NTT<T>::ensure_base(nbase);
	int size = 1 << nbase;
	a.resize(size);
	b.resize(size);
	NTT<T>::fft(a);
	NTT<T>::fft(b);
	Modular<T> inv = 1 / static_cast<Modular<T>>(size);
	for(int i = 0; i < size; ++i){
		a[i] = (2 - a[i] * b[i]) * b[i] * inv;
	}
	reverse(a.begin() + 1, a.end());
	NTT<T>::fft(a);
	a.resize(n);
	return a;
}
template <typename T>
vector<Modular<T>> operator*(const vector<Modular<T>>& a, const vector<Modular<T>>& b) {
	if(a.empty() || b.empty()){
		return {};
	}
	if(min(a.size(), b.size()) < 150){
		vector<Modular<T>> c(a.size() + b.size() - 1, 0);
		for (int i = 0; i < (int) a.size(); i++) {
			for (int j = 0; j < (int) b.size(); j++){
				c[i + j] += a[i] * b[j];
			}
		}
		return c;
	}
	return NTT<T>::multiply(a, b);
}
template <typename T>
vector<Modular<T>>& operator*=(vector<Modular<T>>& a, const vector<Modular<T>>& b) {
	return a = a * b;
}
// Needs Modular
