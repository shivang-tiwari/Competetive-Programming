// O(n+m) LCS using suffix array
struct state {
    int len, link;
    map<char, int> next;
    state(){
        len = 0;
        link = 0;
        next.clear();
    }
};
const int MAXLEN = 100005;
state st[MAXLEN * 2];
int sz, last;
 
void sa_init() {
    st[0].len = 0;
    st[0].link = -1;
    sz++;
    last = 0;
}
void sa_reset(){
    for(int i = 0; i < sz; i++){
        st[i].len = 0;
        st[i].link = 0;
        st[i].next.clear();
    }
	sz = last = 0;
}
void sa_extend(char c) {
    int cur = sz++;
    st[cur].len = st[last].len + 1;
    int p = last;
    while (p != -1 && !st[p].next.count(c)) {
        st[p].next[c] = cur;
        p = st[p].link;
    }
    if (p == -1) {
        st[cur].link = 0;
    } 
    else {
        int q = st[p].next[c];
        if (st[p].len + 1 == st[q].len) {
            st[cur].link = q;
        }
        else {
            int clone = sz++;
            st[clone].len = st[p].len + 1;
            st[clone].next = st[q].next;
            st[clone].link = st[q].link;
            while (p != -1 && st[p].next[c] == q) {
                st[p].next[c] = clone;
                p = st[p].link;
            }
            st[q].link = st[cur].link = clone;
        }
    }
    last = cur;
}
string lcs(string &S, string &T) {
    sa_init();
    for (int i = 0; i < (int)S.size(); i++)
        sa_extend(S[i]);
 
    int v = 0, l = 0, best = 0, bestpos = 0;
    for (int i = 0; i < (int)T.size(); i++) {
        while (v && !st[v].next.count(T[i])) {
            v = st[v].link ;
            l = st[v].len;
        }
        if (st[v].next.count(T[i])) {
            v = st [v].next[T[i]];
            l++;
        }
        if (l > best) {
            best = l;
            bestpos = i;
        }
    }
    string res = T.substr(bestpos - best + 1, best); // LCS sting
    sa_reset();
    return res;
}
