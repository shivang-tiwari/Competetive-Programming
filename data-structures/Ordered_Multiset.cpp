#include <bits/extc++.h>
using namespace __gnu_pbds;
template <typename A, typename B = std::less <A>> using ordered_set = tree <A, null_type, B, rb_tree_tag, tree_order_statistics_node_update>;
template <typename A, typename B, typename C = std::less <A>> using ordered_map = tree <A, B, C, rb_tree_tag, tree_order_statistics_node_update>;

template <typename A>
struct ordered_multiset{
	static const int inf = 1e9 + 60;
	
	ordered_set<pair<A,int>> st;
	map<A,int> freq;
	
	void insert(const A &key){
		st.insert({key,freq[key]});
		freq[key]++;
	}
	
	void erase(const A &key){
		freq[key]--;
		st.erase(st.find({key,freq[key]}));
	}
	
	int count(const A &key){
		return freq.find(key) != freq.end() ? freq[key] : 0;
	}
	
	int less_than(const A &key){
		return st.order_of_key({key,-inf});
	}
	
	int less_than_or_equal_to(const A &key){
		return st.order_of_key({key,+inf});
	}
	
	int greater_than(const A &key){
		return st.size() - less_than_or_equal_to(key);
	}
	
	int greater_than_or_equal_to(const A &key){
		return st.size() - less_than(key);
	}
};
//order_of_key (k) : Number of items strictly smaller than k
