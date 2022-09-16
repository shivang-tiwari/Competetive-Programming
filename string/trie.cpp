const int ALPHABET = 26;
struct trie{
	struct node{
		bool ends;
		int sub;
		array<node*,ALPHABET> next;
		node(){
			ends = false;
			for(int i = 0; i < 26; i++){
				next[i] = NULL;
			}
			sub = 0;
		}
	};
	
	inline int where(char c){
		return c - 'a';
	}
	
	node *head = NULL;
	trie(){
		head = new node();
	}
	void insert(const string &s){
		node *cur = head;
		for(int i = 0; i < (int)s.size(); i++){
			cur->sub++;
			int x = where(s[i]);
			
			if(cur->next[x] == NULL){
				cur->next[x] = new node();
			}
			cur = cur->next[x];
		}
		cur->sub++;
		cur->ends = true;
	}
	
	bool find(const string &s){
		node *cur = head;
		for(int i = 0; i < (int)s.size(); i++){
			int x = where(s[i]);
			if(cur->next[x] == NULL){
				return false;
			}
			cur = cur->next[x];
		}
		return cur->ends;
	}
	
	void erase(const string &s){ // Must be present
		node *cur = head;
		for(int i = 0; i < (int)s.size(); i++){
			cur->sub--;
			int x = where(s[i]);
			cur = cur->next[x];
		}
		cur->sub--;
		if(cur->sub == 0){
			cur->ends = false;
		}
	}
	
	int prefix(const string &s){ // Number of strings which have s as prefix
		node *cur = head;
		for(int i = 0; i < (int)s.size(); i++){
			int x = where(s[i]);
			if(cur->next[x] == NULL){
				return 0;
			}
			cur = cur->next[x];
		}
		return cur->sub;
	}
	
	void delt(node *cur){
		if(cur == NULL)return;
		for(int i = 0; i < 26; i++){
			delt(cur->next[i]);
		}
		free(cur);
	}
	
	~trie(){
		delt(head);
	}
};
