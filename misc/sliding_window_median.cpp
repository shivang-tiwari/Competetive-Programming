struct slmed{
	multiset<int> bef,aft;
	help(){}
	void fix(){
		if(bef.size()){
			auto itr = bef.end(); itr--;
			aft.insert(*itr); bef.erase(itr);
		}
		while(bef.size() < aft.size()){
			bef.insert(*aft.begin()); aft.erase(aft.begin());
		}
		
	}
	void insert(int x){
		bef.insert(x);
		fix();
	}
	void remove(int x){ // Can't remove if not present. Will throw error
		auto itr = bef.end(); itr--;
		if(*itr < x) aft.erase(aft.lower_bound(x));
		else bef.erase(bef.lower_bound(x));
		fix();
	}
	int tell(){
		auto itr = bef.end(); itr--;
		if(bef.size() == aft.size() + 1)return *itr;
		return (*itr + *aft.begin())/2;
	}
};
