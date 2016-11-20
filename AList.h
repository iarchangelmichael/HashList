enum AListOpt{ AListCon = 1, AListDes = 2, AListClear = 4 };

// Malloc / Free
template<class AListEl, int OT = AListCon | AListDes | AListClear>
class AListAllocDef{

public:
	AListAllocDef(){
	}

	AListEl* AllocNew(){
		AListEl *el = (AListEl*)malloc(sizeof(AListEl));

		if(OT & AListCon)
			new(el)AListEl;
		else
			memset(el, 0, sizeof(AListEl));

		return el;
	}

	void AllocFree(AListEl *el){
		if(!el)
			return ;

		if(OT & AListDes)
			el->~AListEl();
		free(el);
		return ;
	}

};

// Allocate block[AListEl * x]. No free(block), free in destroy.
// Warning: sizeof(AListEl) must be >= sizeof(void*);
template<class AListEl, int OT = AListCon | AListDes | AListClear, int blocksize = 16384>
class AListAllocUList{

	// Node structure
	class AListAllocUListNode{
	public:
		AListAllocUListNode *_n;
//		AListEl data[0];
	};

	// Free structure
	class AListAllocUListFel{
	public:
		AListAllocUListFel *_n;
	};

	// Nodes
	AListAllocUListNode *_a, *_e;

	// Free elements
	AListAllocUListFel *fel;

public:
	AListAllocUList(){
		_a = 0;
		_e = 0;
		fel = 0;
	}

	AListEl* AllocNew(){
		AListEl *el;

		if(!fel)
			AllocNode();

		el = (AListEl*)fel;
		fel = fel->_n;		

		if(OT & AListCon)
			new(el)AListEl;
		else
			memset(el, 0, sizeof(AListEl));

		return el;
	}

	void AllocFree(AListEl *el){
		if(!el)
			return ;

		if(OT & AListDes)
			el->~AListEl();

		AListAllocUListFel *del = (AListAllocUListFel*)el;
		del->_n = fel;
		fel = del;
		
		return ;
	}

private:
	void AllocNode(){
		int els = (blocksize - sizeof(AListAllocUListNode)) / sizeof(AListEl);
		AListAllocUListNode *node = (AListAllocUListNode*) malloc(sizeof(AListAllocUListNode) + els * sizeof(AListEl));
		OMatrixTemplateAddO(_a, _e, node);

		AListAllocUListFel *el = (AListAllocUListFel*)(node + 1), *to = el + els;

		while(el < to){
			el->_n = fel;
			fel = el;
			el ++;
		}

		return ;
	}

public:

	void Clear(){
		AListAllocUListNode *n = _a, *d;

		while(n){
			d = n;
			n = n->_n;
			free(d);
		}

		_a = 0;
		_e = 0;
		fel = 0;

		return ;
	}

	~AListAllocUList(){
		Clear();
	}

};



// Object Allocate
// Allocate: if(!free_element) Allocate node; return free_element;
// Allocate node: malloc(array of AListEl); => free_element;
// Free: Binary tree -> node.used_size --; if(!node.used size) free(node);

	// Free structure
class AListAllocOListFel{
public:
	AListAllocOListFel *_n;
};

// Node structure
class AListAllocOListNode{
public:
	// Left & right (tree)
	AListAllocOListNode *left, *right;

	// Prev & Next free (linked) node
	AListAllocOListNode *pfel, *nfel;

	// Free elements
	AListAllocOListFel *fel;

	// Used size
	unsigned int usz;

	// Data
	//AListEl data[0];

	void Clear(){
		left = 0;
		right = 0;
		pfel = 0;
		nfel = 0;
		fel = 0;
		usz = 0;
	}

	AListAllocOListNode(){
		Clear();
	}

};

template<class AListEl, int OT = AListCon | AListDes | AListClear, int blocksize = 16384>
class AListAllocOList{
	// Nodes
	//AListAllocUListNode nodes;
	//AListAllocUListNode *efel;
	//AListAllocUListNode *nodes, *fel;

	// Nodes
	AListAllocOListNode *nodes;

	// Prev & Next free (linked) node
	AListAllocOListNode *pfel, *nfel;

public:
	AListAllocOList(){
		pfel = 0;
		nfel = 0;
		nodes = 0;
	}

	AListEl* AllocNew(){
		if(!pfel)
			AllocNode();

		AListEl *el = (AListEl*) pfel->fel;
		pfel->usz ++;

		pfel->fel = pfel->fel->_n;

		if(!pfel->fel){
			if(pfel->pfel)
				pfel = pfel->nfel;
			else
				pfel = 0;
		}

		if(OT & AListCon)
			new(el)AListEl;
		else
			memset(el, 0, sizeof(AListEl));

		return el;
	}

	void AllocFree(AListEl *el){
		if(!el)
			return ;

		if(OT & AListDes)
			el->~AListEl();

		AListAllocOListNode *node, *pnode;
		node = GetNode(el, pnode);

		if(!node)
			int err = 67;

		AListAllocOListFel *fel = (AListAllocOListFel*)el;
		fel->_n = node->fel;
		node->fel = fel;
		node->usz --;

		if(!node->usz)
			FreeNode(node, pnode);

		return ;
	}

private:

	void AllocNode(){
		int els = (blocksize - sizeof(AListAllocOListNode)) / sizeof(AListEl);
		AListAllocOListNode *node = (AListAllocOListNode*) malloc(sizeof(AListAllocOListNode) + els * sizeof(AListEl));
		node->Clear();

		AListAllocOListNode *tnode = nodes;

		if(!nodes)
			nodes = node;
		else
			while(tnode){
				if(node < tnode){
					if(tnode->left)
						tnode = tnode->left;
					else{
						tnode->left = node;
						break;
					}
				}

			if(node > tnode){
					if(tnode->right)
						tnode = tnode->right;
					else{
						tnode->right = node;
						break;
					}
				}
		}

		//Test();

		AListAllocOListFel *el = (AListAllocOListFel*)(node + 1), *to = el + els;

		while(el < to){
			el->_n = node->fel;
			node->fel = el;
			el ++;
		}

		OMatrixTemplateAddF(pfel, nfel, node, pfel, nfel);
		// node->nfel = nodes->nfel;
		// nodes->nfel = node;

		return ;
	}

	void Test(){
		TestNode(nodes);
	}

	void TestNode(AListAllocOListNode *node){
		if(!node)
			return ;

		if(node->left >= node || node->right <= node && node->right)
			int err = 64;

		TestNode(node->left);
		TestNode(node->right);
	}

	AListAllocOListNode* GetNode(AListEl *el, AListAllocOListNode *&pnode){
		AListAllocOListNode *tnode = pnode = nodes;
		int els = (blocksize - sizeof(AListAllocOListNode)) / sizeof(AListEl);
		int sz = sizeof(AListAllocOListNode) + els * sizeof(AListEl);

		while(tnode){
			if((char*)el < (char*)tnode + sz  && (char*)el > (char*)tnode)
				return tnode;

			if((char*)el < (char*)tnode){
				pnode = tnode;
				tnode = tnode->left;
				continue;
			}

			pnode = tnode;
			tnode = tnode->right;
		}

		return tnode;
	}

	void FreeNode(AListAllocOListNode *node, AListAllocOListNode *pnode){
		return ;

		if(node == pnode->left){
			pnode->left = node->right;
			node->right = 0;
		}

		if(node == pnode->right){
			pnode->right = node->left;
			node->left = node->right;
			node->right = 0;
		}

		free(node);

		return ;
	}

	void ClearNode(AListAllocOListNode *node){
		if(!node)
			return ;

		ClearNode(node->left);
		ClearNode(node->right);

		free(node);

		return ;
	}

public:

	void Clear(){
		ClearNode(nodes);

		pfel = 0;
		nfel = 0;
		nodes = 0;

		//AListAllocOListNode::Clear();

		return ;
	}

	~AListAllocOList(){
		Clear();
	}

};