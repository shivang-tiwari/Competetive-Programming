template<bool multiple = true>
struct avl_tree{
	struct node {
		int key;
		int height;
		int siz;
		
		node *left;
		node *right;
		
		node(int _key){
			key = _key;
			left = NULL;
			right = NULL;
			height = 1;
			siz = 1;
		}
		
	};
	
	
	void unite(node *& root){
		node *left = root->left, *right = root->right;
		
		root->height = 1 + max(height(left),height(right));
		root->siz = 1 + size(left) + size(right);
		
	}
	
	
	int height(node *N){
		if(N == NULL)
			return 0;
		return N->height;
	}
	
	int size(node *N){
		if(N == NULL)
			return 0;
		return N->siz;
	}
	
	int sm(node *N){
		if(N == NULL){
			return 0;
		}
		return N->sum;
	}
	
	node *rightRotate(node *y){
		node *x = y->left;
		node *T2 = x->right;
		
		x->right = y;
		y->left = T2;
	
		unite(y);
		unite(x);
		
		return x;
	}
	
	node *leftRotate(node *x){
		node *y = x->right;
		node *T2 = y->left;
	
		y->left = x;
		x->right = T2;
		
		unite(x);
		unite(y);
		
		return y;
	}
	
	int getBalance(node *N){
		if(N == NULL)
			return 0;
		return height(N->left) - height(N->right);
	}
	
	node* insert(node* root, int key){
		if(root == NULL){
			return new node(key);
		}
		
		if(!multiple && root->key == key){
			return root;
		}
		
		if(key < root->key)
			root->left = insert(root->left, key);
		else
			root->right = insert(root->right, key);
			
		unite(root);
	
		int balance = getBalance(root);
	
		if(balance > 1 && key < root->left->key)
			return rightRotate(root);
	
		if(balance < -1 && key > root->right->key)
			return leftRotate(root);
	
		if(balance > 1 && key > root->left->key){
			root->left = leftRotate(root->left);
			return rightRotate(root);
		}
	
		if(balance < -1 && key < root->right->key){
			root->right = rightRotate(root->right);
			return leftRotate(root);
		}
		return root;
	}
	node *minValueNode(node* root){
		node* current = root;
		while(current->left != NULL)
			current = current->left;
		return current;
	}
	
	node* deleteNode(node* root, int key) {
		if(root == NULL)
			return root;
		if(key < root->key)
			root->left = deleteNode(root->left, key);
			
		else if(key > root->key)
			root->right = deleteNode(root->right, key);
	
		else{
			if((root->left == NULL) || (root->right == NULL)){
				node *temp = root->left ? root->left : root->right;
				if(temp == NULL) {
					temp = root;
					root = NULL;
				}
				else{
					*root = *temp;
				}
				delete temp;
			}
			else {
				node* temp = minValueNode(root->right);
				root->key = temp->key;
				root->right = deleteNode(root->right, temp->key);
			}
		}
	
		if(root == NULL)
			return root;
	
		unite(root);
		
		int balance = getBalance(root);
		
		if(balance > 1 && getBalance(root->left) >= 0)
			return rightRotate(root);
	
		if(balance > 1 && getBalance(root->left) < 0){
			root->left = leftRotate(root->left);
			return rightRotate(root);
		}
	
		if(balance < -1 && getBalance(root->right) <= 0)
			return leftRotate(root);
	
		if(balance < -1 && getBalance(root->right) > 0){
			root->right = rightRotate(root->right);
			return leftRotate(root);
		}
		return root;
	}
	
	node *getNode(node *root, int key){
		if(root == NULL || root->key == key){
			return root;
		}
		if(root->key > key){
			return getNode(root->left,key);
		}
		else{
			return getNode(root->right,key);
		}
	}
	
	void deleteAll(node *root){
		if(root == NULL){
			return;
		}
		deleteAll(root->left);
		deleteAll(root->right);
		delete root;
	}
	
	node *Root = NULL;
	
	avl_tree(){
		Root = NULL;
	}
	
	~avl_tree(){
		clear();
	}
	
	void insert(int key){
		Root = insert(Root,key);
	}
	
	void erase(int key){
		Root = deleteNode(Root,key);
	}
	
	int size(){
		return size(Root);
	}
	
	void clear(){
		deleteAll(Root);
	}
	
	node *find(int key){
		return getNode(Root,key);
	}
	
};
