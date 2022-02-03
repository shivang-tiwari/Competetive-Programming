bool sub(string pat, string txt){
	int M = pat.size();
	int N = txt.size();
	int lps[M];
	auto compute = [&](){
		int len = 0;
		lps[0] = 0;
		int i = 1;
		while (i < M){
			if (pat[i] == pat[len]) {
				len++;
				lps[i] = len;
				i++;
			}
			else{
				if (len != 0){
					len = lps[len - 1];
				}
				else{
					lps[i] = 0;
					i++;
				}
			}
		}
	};
	compute();
	int i = 0,j = 0;
	while (i < N) {
		if (pat[j] == txt[i]){
			j++;
			i++;
		}
		if (j == M){
			//Found pattern at index (i - j)
			return true;
			j = lps[j - 1];
		}
		else if (i < N && pat[j] != txt[i]) {
			if (j != 0)j = lps[j - 1];
			else i = i + 1;
		}
	}
	return false;
}
