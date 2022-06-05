/*
x | y == x is a factor of y


If g(n) = sum_{d | n} f(d)
then 

f(n) = sum_{d | n} g(d) mu(n/d)


mu(n) = [
			0 if n = k * p^2 for some prime number p
			+1 if n != p^2 and n has even number of prime factors
			-1 if n != p^2 and n has odd number of prime factors
		]

sum_{d | n} mu(d) = [
						1 if n == 1
						0 else
					]
				  = (n == 1)

For mobius inversion

bool(smth) = bool(n == 1)
 = sum_{d | n} mu(d)
*/

vector<int> mobius;
vector<int> lp;
void prep(int MAXN = 1e6){
	mobius.resize(MAXN+1);
	lp.resize(MAXN+1,-1);
	mobius[1] = 1;
    for (int i = 2; i <= MAXN; ++i) {
        if (lp[i] == -1){
			for (int j = i; j <= MAXN; j += i){
				if(lp[j] == -1){
					lp[j] = i;
				}
			}
		}
		mobius[i] = [](int x) {
			int cnt = 0;
			while (x > 1) {
				int k = 0, d = lp[x];
				while (x % d == 0) {
					x /= d;
					++k;
					if (k > 1) return 0;
				}
				++cnt;
			}
			return cnt&1 ? -1 : 1;
		}(i);
    }
}
