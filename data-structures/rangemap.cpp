// This map supports addition in a range of keys.

class rangemap{
	public:
	struct node{
		int key,val,leftAdd,rightAdd;
		node *left,*right;
		node(int k, int v){
			this->key = k;
			this->val = v;
			this->leftAdd = 0;
			this->rightAdd = 0;
			this->left = NULL;
			this->right = NULL;
		}
	};
	node *head = NULL;
	rangemap(){ head = NULL;}
	node* insert(node *root,int k,int v,int acc = 0){
		if(root == NULL){
			return new node(k,v-acc);
		}
		if(k == root->key){
			root->val = v - acc;
		}
		else if(k > root->key){
			root->right = insert(root->right,k,v,acc + root->rightAdd);
		}
		else{
			root->left = insert(root->left,k,v,acc + root->leftAdd);
		}
		return root;
	}
	int search(node* root, int k,int acc = 0){
		if(root == NULL)
			return -1;
		if(root->key == k)return root->val + acc;
		else if(root->key < k)
			return search(root->right, k,acc + root->rightAdd);
		else
			return search(root->left,k,acc + root->leftAdd);
	}

	void update(node *root,int x1,int c){ // Adds c to all keys <= x1
		if(root == NULL)return;
		if(root->key > x1)update(root->left,x1,c);
		else{
			root->val += c;
			root->leftAdd += c;
			update(root->right,x1,c);
		}
	}
	void update(node *root,int x1,int x2,int c){
		if(root == NULL)return;
		update(root,x2,c);
		update(root,x1-1,-c);
	}
	node* find(node *root,int &acc){
		node* temp = root;
		while (temp && temp->left != NULL){
			acc += temp->leftAdd;
			temp = temp->left;
		}
		return temp;
	}
	node* Delete(struct node* root, int k){
		if (root == NULL)
			return root;
		if (k < root->key)
			root->left = Delete(root->left, k);
		else if (k > root->key)
			root->right = Delete(root->right, k);
		else {
			if(root->left == NULL && root->right == NULL){
				free(root);
				return NULL;
			}
			if (root->left == NULL) {
				node* temp = root->right;
				temp->leftAdd += root->rightAdd;
				temp->rightAdd += root->rightAdd;
				temp->val += root->rightAdd;
				free(root);
				return temp;
			}
			else if (root->right == NULL) {
				node* temp = root->left;
				temp->leftAdd += root->leftAdd;
				temp->rightAdd += root->leftAdd;
				temp->val += root->leftAdd;
				free(root);
				return temp;
			}
			int acc = root->rightAdd;
			node* temp = find(root->right,acc);
			root->key = temp->key;
			root->val = temp->val + acc;
			root->right = Delete(root->right, temp->key);
		}
		return root;
	}
	// All functions below
	void insert(int key,int value){ // Updates if present
		head = insert(head,key,value);
	}
	int find(int key){ // Returns -1 if key not found
		return search(head,key);
	}
	void add(int l,int r,int v = 1){ // Adds v to values of all keys in the range [l,r]
		update(head,l,r,v);
	}
	void erase(int key){
		head = Delete(head,key);
	}
};
