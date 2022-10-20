vector<int> manacher_odd(string s){
    int n = s.size();
    s = "$" + s + "^";
    vector<int> p(n + 2);
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
    return vector<int>(begin(p) + 1, end(p) - 1);
}
pair<vector<int>,vector<int>> manacher(string s){
    string t;
    for(auto c: s) {
        t += string("#") + c;
    }
    auto res = manacher_odd(t + "#");
    res = vector<int>(begin(res) + 1, end(res) - 1);
    vector<int> d1(s.size()),d2(s.size());
    for(int i = 0; i < (int)s.size(); i++){
		d1[i] = res[2*i] / 2;
		if(i){
			d2[i] = (res[2*i - 1] - 1) / 2;
		}
	}
    return {d1,d2};
}
/*
d1[i] = number of odd-length palindromes centered at i
d2[i] = number of even-length palindromes centered at (i-1,i)

Example
s = a {b a _b_ a b} c

d1[3] = 3

s = c {b a _a_ b} d

d2[3] = 2
*/
