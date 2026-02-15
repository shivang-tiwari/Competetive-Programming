class Manacher{
	public:
	
	vector<int> d1;
	vector<int> d2;
	void manacher(const string &s){
		int n = s.size();
		if((int)t.size() != 2 * n + 1 + 2){
			t.resize(2 * n + 1 + 2);
		}
		t[0] = '$';
		for(int i = 0; i < n; i++){
			t[2*i+1] = '#';
			t[2*i+2] = s[i];
		}
		t[2*n+1] = '#';
		t[2*n+2] = '^';
		span<int> res = manacher_odd(t);
		res = span<int>(begin(res) + 1, end(res) - 1);
		d1.resize(s.size());
		d2.resize(s.size());
		for(int i = 0; i < (int)s.size(); i++){
			d1[i] = res[2*i] / 2;
			if(i){
				d2[i] = (res[2*i - 1] - 1) / 2;
			}
		}
	}
	
	
	protected:
	
	string t;
	vector<int> p;
	
	span<int> manacher_odd(const string &s){
		int n = s.size() - 2;
		if((int)p.size() != n+2){
			p.resize(n+2);
		}
		int l = 1, r = 1;
		for(int i = 1; i <= n; i++) {
			p[i] = max(0, min(r - i, p[l + (r - i)]));
			while(s[i - p[i]] == s[i + p[i]]) {
				p[i]++;
			}
			if(i + p[i] > r) {
				l = i - p[i], r = i + p[i];
			}
		}
		return span<int>(begin(p) + 1, end(p) - 1);
	}
};
/*
d1[i] = number of odd-length palindromes centered at i
d2[i] = number of even-length palindromes centered at (i-1,i)

Example
s = a {b a _b_ a b} c

d1[3] = 3

s = c {b a _a_ b} d

d2[3] = 2
*/
