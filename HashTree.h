// <d>Hash List. Ops. This is -> </d>
// Hash Tree
// 17.11.2006 15:10

#define HASHTREE_MAXSIZE	128


template<class HashTreeEl, class AllocHashTree = AListAllocDef<HashTreeEl> >
class HashTree;


template<class HashTreeEl>
class HashTreeElProto{
public:
	HashTreeEl *_p, *_n; // prev, next

	// Crc
	unsigned int crc;

	HashTreeElProto(){
		_p = 0;
		_n = 0;
	}

/* Add:
	MyData data;

public:
	void SetItem(AnyType &key, AnyType &val){}
	unsigned int GetHash(AnyType &key){}
	bool TestHash(HashTreeEl *el, AnyType &key){}
*/

	//

};


template<class HashTreeEl>
class HashTreeNode{
	// Next Node
	HashTreeNode *_p, *_n;

	// Elements
	union{
		HashTreeNode *_an;
		HashTreeEl *_a;
	};

	union{
		HashTreeNode *_en;
		HashTreeEl *_e;
	};

	// Crc & sz
	unsigned int crc;
	unsigned int sz;

public:

	HashTreeNode(){
		_p = 0;
		_n = 0;
		_a = 0;
		_e = 0;
		crc = 0;
		sz = 0;
	}

	void Move(HashTreeNode &node){
		_p = node._p;
		_n = node._n;
		_a = node._a;
		_e = node._e;
		crc = node.crc;
		sz = node.sz;

		node._p = node._n = node._an = node._en = 0;
		node.crc = 0;
		node.sz = 0;

	}

	template<class HashTreeEl, class AllocHashTree>
	friend class HashTree;
};


template<class HashTreeEl, class AllocHashTree>
class HashTree{
private:
	// Nodes Alloc
	AListAllocDef<HashTreeNode<HashTreeEl>> nodes;
	// Elements Alloc
	AllocHashTree elements;

	// Parents
	HashTreeNode<HashTreeEl> pnode;
	HashTreeEl pel;

	// All Elements Size
	unsigned int esz;

	// All Nodes Size
	unsigned int nsz;

	// Levels Size
	unsigned int lsz;

public:
	HashTree(){
		esz = 0;
		nsz = 0;
		lsz = 0;
	}

	template<typename TypeKey>
	HashTreeEl* Get(TypeKey &key){
		// Get Crc
		unsigned int crc = pel.GetHash(key);

		// Find Node
		HashTreeNode<HashTreeEl> *node;
		if(!(node = GetNode(crc)))
			return 0;
		
		while(1){
			// Find Element
			HashTreeEl *el = node->_a, *to = node->_e;
			if(to)
				to = to->_n;

			for(el; el != to; el = el->_n){
				if(el->crc == crc && pel.TestHash(el, key))
					return el;
				else if(el->crc > crc)
					return 0;
			}

			if(!(node = GetNodeNext(node, crc)))
				break;
		}

		return 0;
	}

	template<typename TypeKey, typename TypeVal>
	HashTreeEl* Add(TypeKey &key, TypeVal &val){
		// Get Crc
		unsigned int crc = pel.GetHash(key);

		// First element
		//if(!pel._p)
		//	NewNode(0, &pnode, 0);

		// Find Node
		HashTreeNode<HashTreeEl> *node;
		if(!(node  = GetNode(crc)))
			return 0;

		// Set Node
		HashTreeNode<HashTreeEl> *snd = node;
		HashTreeEl *sel = 0;

		while(1){
			// Find Element
			HashTreeEl *el = node->_a, *to = node->_e;
			if(to)
				to = to->_n;

			if(el && el->crc > crc)
				break;

			for(el; el != to; el = el->_n){
				if(el->crc > crc){
					snd = node;
					sel = el->_p;
					node = 0;
					
					break;
				}

				if(el->_n == to){
					snd = node;
					sel = el;
				}

			}

			if(!(node = GetNodeNext(node, crc)))
				break;
		}

		//Test();

		// Insert Element
		HashTreeEl *nel = elements.AllocNew();
		//OMatrixT<HashTreeEl>::OMAddExP(snd->_a, snd->_e, p, nel);
		OMatrixTemplateAddP(pel._p, pel._n, sel, nel);

		nel->crc = crc;
		nel->SetItem(key, val);

		if(!snd->_a)
			snd->_a = snd->_e = nel;

		if(!sel)
			snd->_a = nel;

		if(snd->_e == sel)
			snd->_e = nel;

		snd->sz ++;
		esz ++;

//		if(sel && sel->crc > crc)
//			print("FAIL!!");

		//Test();

		// Fragmentation
		if(snd->sz >= HASHTREE_MAXSIZE)
			FragNode(snd, lsz);

		//Test();

		return nel;
	}

	int Test(){
		HashTreeEl *el = pel._p;

		while(el){
			if(el->_n && el->crc > el->_n->crc)
				int err = 1;
			el = el->_n;
		}

		if(nsz)
			TestNode(&pnode, 0);
		return 0;
	}

	int TestNode(HashTreeNode<HashTreeEl> *pn, int lvl){
		HashTreeNode<HashTreeEl> *node = pn->_an;
		HashTreeEl *el = 0, *lel = 0;

		while(node){
			if(node->_n && node->crc > node->_n->crc)
				int err = 1;

			if(lvl != (lsz - 1)){
				TestNode(node, lvl + 1);
			} else{
				if(!el)
					el = node->_a;

				if(el != node->_a && node->sz)
					int err = 3;

				// Count size
				HashTreeEl *e = node->_a, *t = node->_e;
				int s = 0;

				if(t)
					t = t->_n;

				for(e; e != t; e = e->_n)
					s ++;

				if(s != node->sz)
					int err = 4;


				lel = el = node->_e;

				if(el)
					el = el->_n;
			}

			node = node->_n;
		}

		if(el != 0)
			int err = 2;
 
		return 0;
	}

	template<typename TypeKey>
	bool Del(TypeKey &key){
		// Get Crc
		unsigned int crc = pel.GetHash(key);

		// Find Node
		HashTreeNode<HashTreeEl> *node;
		if(!(node  = GetNode(crc)))
			return 0;

		while(1){
			// Find Element
			HashTreeEl *el = node->_a, *to = node->_e;
			if(to)
				to = to->_n;

			for(el; el != to; el = el->_n){
				if(el->crc == crc && pel.TestHash(el, key)){

					//Test();

					if(node->_a == node->_e){
						node->_a = node->_e = 0;
					}

					if(node->_a == el)
						node->_a = el->_n;

					if(node->_e == el)
						node->_e = el->_p;

					OMatrixTemplateDel(pel._p, pel._n, el);

					elements.AllocFree(el);
					node->sz --;
					esz --;

					//Test();

					if(!node->sz){
						UnFragNode(node, lsz - 1);
					}

					//Test();

					return 1;
				}
				else if(el->crc > crc)
					return 0;
			}

			if(!(node = GetNodeNext(node, crc)))
				break;
		}

		return 0;
	}

private:
	HashTreeNode<HashTreeEl>* NewNode(unsigned int crc, HashTreeNode<HashTreeEl> *parent, HashTreeNode<HashTreeEl> *prev){
		HashTreeNode<HashTreeEl> *n = nodes.AllocNew();
		OMatrixTemplateAddP(parent->_an, parent->_en, prev, n);

		n->crc = crc;

		return n;
	}

	HashTreeNode<HashTreeEl>* GetNode(unsigned int crc, HashTreeNode<HashTreeEl> *pn = 0, unsigned int lvl = 0){
		if(!pn)
			pn = &pnode;
		
		if(!nsz)
			return pn;

		// Find Node
		HashTreeNode<HashTreeEl> *nd = pn->_an, *to = pn->_en;
		if(to)
			to = to->_n;

		for(nd; nd != to; nd = nd->_n){
			if(nd->crc > crc){
				if(!nd->_p)
					return 0;
				return lvl == (lsz - 1) ? nd->_p : GetNode(crc, nd->_p, lvl + 1);
			}
			if(nd->crc == crc){
				if(!nd->_p)
					return lvl == (lsz - 1) ? nd : GetNode(crc, nd, lvl + 1);
				return lvl == (lsz - 1) ? nd->_p : GetNode(crc, nd->_p, lvl + 1);
			}
		}

		return lvl == (lsz - 1) ? pn->_en : GetNode(crc, pn->_en, lvl + 1);
	}

	HashTreeNode<HashTreeEl>* GetNodeNext(HashTreeNode<HashTreeEl>*nd, unsigned int crc){
		if(!nd || !nd->_n || nd->crc > crc)
			return 0;

		return nd->_n;
	}

	// Node, node level, parent, parent level.
	int FragNode(HashTreeNode<HashTreeEl> *node, int lvl = 0, HashTreeNode<HashTreeEl> *pn = 0, unsigned int plvl = 0){
		if(!nsz || !pn && pnode.sz >= HASHTREE_MAXSIZE){
			FragNodeLevel();
			lvl ++;
			return 1;
		}

		if(!pn){
			pn = &pnode;
			plvl = 0;
		}

		// Fragmentation
		if((lvl - 1) != plvl){
			// Find node parent
			HashTreeNode<HashTreeEl> *n = pn->_an, *to = pn->_en;
			if(to)
				to = to->_n;

			for(n; n != to; n = n->_n){
				if(n->crc <= node->crc){
					if(n->_n != to && n->_n->crc < node->crc)
						continue;

					if(FragNode(node, lvl, n, plvl + 1)){
						if(pn->sz >= HASHTREE_MAXSIZE)
							FragNode(pn, lvl);
						return 1;
					}
				}
			}
		} else {
			HashTreeNode<HashTreeEl> *n = NewNode(0, pn, node);
			n->sz = node->sz / 2;
			node->sz -= n->sz;

			HashTreeEl *el = node->_a;
			for(unsigned int i = 0; i < node->sz; i ++)
				el = el->_n;

			n->_a = el;
			n->_e = node->_e;
			n->crc = el->crc;
			node->_e = el->_p;
					 
			nsz ++;
			pn->sz ++;

			return 1;
		}

		return 0;
	}

		// Node, node level, parent, parent level.
	int UnFragNode(HashTreeNode<HashTreeEl> *node, int lvl, HashTreeNode<HashTreeEl> *pn = 0, unsigned int plvl = 0){
		if(!pn){
			pn = &pnode;
			plvl = 0;
		}

		// UnFragmentation
		if(lvl != plvl){
			// Find node parent
			HashTreeNode<HashTreeEl> *n = pn->_an, *to = pn->_en;
			if(to)
				to = to->_n;

			for(n; n != to; n = n->_n){
				if(n->crc <= node->crc){
					if(n->_n != to && n->_n->crc < node->crc)
						continue;

					if(UnFragNode(node, lvl, n, plvl + 1)){
						if(!pn->sz)
							UnFragNode(pn, lvl - 1);
						return 1;
					}
				}
			}
		} else {
			UnFragNodeDo(node, pn);

			//if(!lvl && pn->sz == 0)
			//	UnFragNodeDo(pn, &pnode);
			return 1;
		}

		return 0;
	}

	int UnFragNodeDo(HashTreeNode<HashTreeEl> *node, HashTreeNode<HashTreeEl> *pn = 0){
		if(!node || !pn)
			return 0;

		OMatrixTemplateDel(pn->_an, pn->_en, node);
		nodes.AllocFree(node);

		pn->sz --;
		nsz --;

		return 1;
	}

	void FragNodeLevel(){
		HashTreeNode<HashTreeEl> *node = nodes.AllocNew();
		node->Move(pnode);
		pnode.sz = 1;
		pnode._an = node;
		pnode._en = node;

		nsz ++;
		lsz ++;

		return ;
	}

	void ClearNode(HashTreeNode<HashTreeEl> *node, unsigned int lvl = 0){
		if(!node)
			return ;

		HashTreeNode<HashTreeEl> *nd = node->_an, *d;

		while(nd){
			d = nd;
			nd = nd->_n;

			if(lvl < lsz)
				ClearNode(d, lvl + 1);

			nodes.AllocFree(d);
		}
	}

public:

	int Size(){
		return esz; 
	}

	void Clear(){
		// Nodes
		HashTreeNode<HashTreeEl> *n = pnode._an, *d;
		
		while(n){
			d = n;
			n = n->_n;
			ClearNode(d);
			nodes.AllocFree(d);
		}

		pnode._a = 0;
		pnode._e = 0;

		// Clear Elements
		HashTreeEl *el = pel._p, *del;
		while(el){
			del = el;
			el = el->_n;
			elements.AllocFree(del);
		}

		pel._p = 0;
		pel._n = 0;

		return ;
	}

	~HashTree(){
		Clear();
	}

	//friend class OMatrixT<HashTreeNode<HashTreeEl> >;

};
