// Idea:
// Data: Vector[HASHLS_GNS] -> Vector sorted.
// Add: Vector[GetHash() % HASHLS_GNS] -> Find position for GetHash() && memcpy(vector[pos + 1], vector[pos], sz - pos - 1 );
// Get: Vector[GetHash() % HASHLS_GNS] -> Find position for GetHash()
// Del: Vector[GetHash() % HASHLS_GNS] -> Find position for GetHash() && memcpy(vector[pos - 1], vector[pos], sz - pos);


// Ps:
// Modified five(Five + 1).

#define HASHLS_GNS	16384

template<class HashListEl, class AllocHashList = AListAllocDef<HashListEl> >
class HashListSix{
private:

	// Elements Alloc
	AllocHashList elements;
	
	// One temp element
	HashListEl pel;

	// Node Size
	//unsigned int nsz;

	// Elements Size
	unsigned int esz;

	// Node
	class HashListNode{
	public:
		unsigned int crc;
		HashListEl *el;
	};

	// Global Node
	class HashListGNode{
	public:
		//Node
		HashListNode *nodes;

		// Elements Size
		unsigned int esz;

		// Node Size
		unsigned int nsz;
	};

	HashListGNode *nodes;

public:
	HashListSix(){
		nodes = 0;
//		nsz = 0;
		esz = 0;
	}

	~HashListSix(){
		Clear();
	}

	template<typename TypeKey, typename TypeVal>
	HashListEl* Add(TypeKey &key, TypeVal &val){

		if(!nodes){
			nodes = (HashListGNode*) malloc(HASHLS_GNS * sizeof(HashListGNode));
			memset(nodes, 0, HASHLS_GNS * sizeof(HashListGNode));
		}

		// Get Crc
		unsigned int crc = pel.GetHash(key);
		HashListGNode &gnode = nodes[crc % HASHLS_GNS];

		// Resize
		if(gnode.esz >= gnode.nsz){
			int nusz = gnode.nsz ? gnode.nsz << 1 : 128;
			HashListNode *nnodes = (HashListNode*) malloc(nusz * sizeof(HashListNode));

			memcpy(nnodes, gnode.nodes, gnode.nsz * sizeof(HashListNode));

			free(gnode.nodes);

			gnode.nsz = nusz;
			gnode.nodes = nnodes;
		}


		// Find
		HashListNode *f = gnode.nodes, *to = gnode.nodes + gnode.esz, *p = f;
		int sz = gnode.esz, s;

		while(sz){
			p = f;
			s = sz / 2;
			p += s;

			if(p->crc < crc){
				f = ++p; 
				sz -= s + 1;
			}
			else
				sz = s;
		}

		// Add
		HashListEl *nel = elements.AllocNew();
		nel->SetItem(key, val);

		if(f != (gnode.nodes + gnode.esz))
			memcpy(f + 1, f, (gnode.esz - (f - gnode.nodes)) * sizeof(HashListNode));
		
		f->el = nel;
		f->crc = crc;

		gnode.esz ++;
		esz ++;

		//Test();

		return nel;
	}

	//void Test(){
	//	HashListNode *f = nodes, *to = nodes + esz;

	//	while(f < to){
	//		if(f + 1 != to)
	//			if(f->crc > f[1].crc)
	//				int err = 1;

	//		f ++;
	//	}

	//	return ;
	//}

	template<typename TypeKey>
	HashListEl* Get(TypeKey &key){
		if(!nodes)
			return 0;

		// Get Crc
		unsigned int crc = pel.GetHash(key);
		HashListGNode &gnode = nodes[crc % HASHLS_GNS];

		// Find
		HashListNode *f = gnode.nodes, *to = gnode.nodes + gnode.esz, *p = 0;
		int sz = gnode.esz, s;

		while(sz){
			p = f;
			s = sz / 2;
			p += s;

			if(p->crc < crc){
				f = ++p; 
				sz -= s + 1; 
			}
			else
				sz = s;
		}

//		while(f > nodes && f[-1].crc == crc)
//			f --;

		while(f->crc == crc && f < to){
			if(pel.TestHash(f->el, key))
				return f->el;
			f ++;
		}

		return 0;
	}

	template<typename TypeKey>
	bool Del(TypeKey &key){
		if(!nodes)
			return 0;

		// Get Crc
		unsigned int crc = pel.GetHash(key);
		HashListGNode &gnode = nodes[crc % HASHLS_GNS];

		// Find
		HashListNode *f = gnode.nodes, *to = gnode.nodes + gnode.esz, *p = 0;
		int sz = gnode.esz, s;

		while(sz){
			p = f;
			s = sz / 2;
			p += s;

			if(p->crc < crc){
				f = ++p; 
				sz -= s + 1; 
			}
			else
				sz = s;
		}

		while(f->crc == crc && f < to){
			if(pel.TestHash(f->el, key)){
				elements.AllocFree(f->el);
				memcpy(f, f + 1, (gnode.nsz - (f - gnode.nodes) - 1) * sizeof(HashListNode));
				gnode.esz --;
				esz --;				
				return 1;
			}
			f ++;
		}

		return 0;
	}

	void Clear(){
		for(int i = 0; i < HASHLS_GNS; i ++){
			HashListNode *f = nodes[i].nodes, *to = nodes[i].nodes + nodes[i].esz;
			while(f < to){
				elements.AllocFree(f->el);
				f ++;
			}

			free(nodes[i].nodes);
		}

		free(nodes);

		nodes = 0;
		esz = 0;
//		nsz = 0;

		return ;
	}


};