// Idea:
// Data: vector[nsz] + Elements: dynamic
// Add: vector[GetHash() % nsz] -> insert
// Get: vector[GetHash() % nsz] -> while{ el.TestHash() or next }
// Del: vector[GetHash() % nsz] -> while{ el.TestHash() && delete or next }

template<class HashListEl>
class HashListElProto{
public:
	HashListEl *_p, *_n; // prev, next

	// Crc
	//unsigned int crc;

	HashListElProto(){
		_p = 0;
		_n = 0;
	}

/* Add:
	MyData data;

public:
	void SetItem(AnyType &key, AnyType &val){}
	unsigned int GetHash(){}
	unsigned int GetHash(AnyType &key){}
	bool TestHash(HashTreeEl *el, AnyType &key){}
*/

};

template<class HashListEl, class AllocHashList = AListAllocDef<HashListEl> >
class HashList{
private:

	// Elements Alloc
	AllocHashList elements;
	
	// One temp element
	HashListEl pel;

	// Node Size
	unsigned int nsz;

	// Elements Size
	unsigned int esz;

	// Node
	class HashListNode{
	public:
		HashListEl *_a, *_e;
	};

	HashListNode *nodes;

public:
	HashList(){
		nodes = 0;
		nsz = 0;
		esz = 0;

		UpdateNodes();
	}

	~HashList(){
		Clear();
	}

	template<typename TypeKey, typename TypeVal>
	HashListEl* Add(TypeKey &key, TypeVal &val){
		UpdateNodes();

		// Get Crc
		unsigned int crc = pel.GetHash(key) % nsz;

		// Add
		HashListEl *nel = elements.AllocNew();
		OMatrixTemplateAdd(nodes[crc]._a, nodes[crc]._e, nel);

		nel->SetItem(key, val);
		esz ++;

		return nel;
	}

	template<typename TypeKey>
	HashListEl* Get(TypeKey &key){
		if(!nodes)
			return 0;

		// Get Crc
		unsigned int crc = pel.GetHash(key) % nsz;

		// Find
		HashListEl *el = nodes[crc]._a;
		while(el){
			if(pel.TestHash(el, key))
				return el;
			el = el->_n;
		}

		return 0;
	}

	template<typename TypeKey>
	bool Del(TypeKey &key){
		if(!nodes)
			return 0;

		// Get Crc
		unsigned int crc = pel.GetHash(key) % nsz;

		// Find
		HashListEl *el = nodes[crc]._a;
		while(el){
			if(pel.TestHash(el, key)){
				OMatrixTemplateDel(nodes[crc]._a, nodes[crc]._e, el);
				elements.AllocFree(el);

				esz --;
				return 1;
			}
			el = el->_n;
		}

		return 0;
	}

	void UpdateNodes(){
		if(esz / 2 < nsz)
			return ;

		unsigned int newsz = !nsz ? 128 : nsz << 1;

		HashListNode *newnodes = (HashListNode*) malloc(newsz * sizeof(HashListNode));

		memset(newnodes, 0, newsz * sizeof(HashListNode));

		// Migrate
		for(unsigned int i = 0; i < nsz; i ++){
			HashListEl *el = nodes[i]._a, *del;
			while(el){
				del = el;
				el = el->_n;

				unsigned int crc = del->GetHash();
				crc %= newsz;
				OMatrixTemplateAdd(newnodes[crc]._a, newnodes[crc]._e, del);						
			}
		}

		// Free
		free(nodes);
		nodes = newnodes;
		nsz = newsz;
	}

	void Clear(){
		for(unsigned int i = 0; i < nsz; i ++){
			HashListEl *el = nodes[i]._a, *del;
			while(el){
				del = el;
				el = el->_n;

				elements.AllocFree(del);

				esz --;				
			}
		}

		free(nodes);
		nodes = 0;
		nsz = 0;
		esz = 0;

		return ;
	}


};