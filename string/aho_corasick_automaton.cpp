const int ALPHABET = 26;
const int START    = 'a';

struct aho_corasick{
	
	struct node{
		int nxt[ALPHABET];
		int go[ALPHABET];
		int link;
		int p;
		char pch;
		bool end_string = false;
		
		node(int _p = -1, char ch = '$') : p(_p), pch(ch){
			memset(nxt,-1,sizeof(nxt));
			memset(go,-1,sizeof(go));
			link = -1;
			end_string = false;
		}
	};
	
	vector<node> t = vector<node>(1);
	
	int get_link(int v){
		if (t[v].link == -1) {
			if (v == 0 || t[v].p == 0)
				t[v].link = 0;
			else
				t[v].link = go(get_link(t[v].p), t[v].pch);
		}
		return t[v].link;
	}
	
	int go(int v, char ch){
		int c = ch - START;
		if(t[v].go[c] == -1){
			if (t[v].nxt[c] != -1)
				t[v].go[c] = t[v].nxt[c];
			else
				t[v].go[c] = v == 0 ? 0 : go(get_link(v), ch);
		}
		return t[v].go[c];
	}
	
	void add(string const& s){
		int v = 0;
		for (char ch : s){
			int c = ch - START;
			if(t[v].nxt[c] == -1){
				t[v].nxt[c] = t.size();
				t.emplace_back(v,ch);
			}
			v = t[v].nxt[c];
		}
		t[v].end_string = true;
	}
};
