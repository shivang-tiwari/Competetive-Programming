Mint lagrange_ap(const vector<Mint> &p, int x, int a = 0, int d = 1){
	int n = p.size() - 1;
	for(int i = 0; i <= n; i++){
		if(a + (i-1) * d == x){
			return p[i];
		}
	}
	vector<Mint> pref(n + 1, 1), suf(n + 1, 1);
	for (int i = 0; i < n; i++) pref[i + 1] = pref[i] * (x - (a + (Mint)d * i));
	for (int i = n; i > 0; i--) suf[i - 1] = suf[i] * (x - (a + (Mint)d * i));
	
	vector<Mint> lagrange_fact(n + 1, 1),lagrange_fact_inv(n + 1, 1);
	for (int i = 1; i <= n; i++) lagrange_fact[i] = lagrange_fact[i - 1] * d * i;
	lagrange_fact_inv[n] /= lagrange_fact[n];
	for (int i = n; i >= 1; i--) lagrange_fact_inv[i - 1] = lagrange_fact_inv[i] * d * i;
	
	Mint ans = 0;
	for (int i = 0; i <= n; i++) {
		Mint tmp = p[i] * pref[i] * suf[i] * lagrange_fact_inv[i] * lagrange_fact_inv[n-i];
		if ((n - i) & 1) ans -= tmp;
		else ans += tmp;
	}
	return ans;
}
// Input points must be a, a + d, a + 2d , ... 

Mint lagrange_general(const vector<pair<int,Mint>> &f, int xi){
	int n = f.size();
	Mint result = 0;
	
	for(int i = 0; i < n; i++){
		Mint term = f[i].second;
		for(int j = 0;j < n;j++){
			if (j != i)
				term = term * (xi - f[j].first) / Mint(f[i].first - f[j].first);
		}
		result += term;
	}
	return result;
}

Mint lagrange(const vector<pair<int,Mint>> &a,int x){
	assert(!a.empty());

	if(a.size() == 1){
		return a[0].second;
	}
	
	bool ap = true;
	bool sorted = true;
	for(int i = 1; i < (int)a.size(); i++){
		if(a[i].first <= a[i-1].first){
			sorted = false;
			break;
		}
	}
	
	for(int i = 2; i < (int)a.size(); i++){
		if(a[i].first - a[i-1].first != a[1].first - a[0].first){
			ap = false;
			break;
		}
	}
	if(ap){
		vector<Mint> p(a.size());
		for(int i = 0; i < (int)a.size(); i++){
			p[i] = a[i].second;
		}
		return lagrange_ap(p,x,a[0].first,a[1].first - a[0].first);
	}
	else{
		return lagrange_general(a,x);
	}
} // Input points must be in sorted order

Mint lagrange(const vector<Mint>& p,int x){
	return lagrange_ap(p,x,0,1);
}
// If input points are (0,1,2,3,...)
