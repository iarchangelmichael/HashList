// Idea:
// Data: Vector sorted.
// Add: Find position for GetHash() && memcpy(vector[pos + 1], vector[pos], sz - pos - 1 );
// Get: Find position for GetHash()
// Del: Find position for GetHash() && memcpy(vector[pos - 1], vector[pos], sz - pos);

// Comment: hardcore insert!

// Ps:
// Game Five: fast find random number = rand() % 100. 

template<class HashListEl, class AllocHashList = AListAllocDef<HashListEl> >
class HashListFive{
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
		unsigned int crc;
		HashListEl *el; //, *_e;
	};

	HashListNode *nodes;

public:
	HashListFive(){
		nodes = 0;
		nsz = 0;
		esz = 0;
	}

	~HashListFive(){
		Clear();
	}

	template<typename TypeKey, typename TypeVal>
	HashListEl* Add(TypeKey &key, TypeVal &val){
		//UpdateNodes();

		// Resize
		if(esz >= nsz){
			int nusz = nsz ? nsz << 1 : 128;
			HashListNode *nnodes = (HashListNode*) malloc(nusz * sizeof(HashListNode));

			memcpy(nnodes, nodes, nsz * sizeof(HashListNode));

			free(nodes);

			nsz = nusz;
			nodes = nnodes;
		}

		// Get Crc
		unsigned int crc = pel.GetHash(key);

		// Find
		HashListNode *f = nodes, *to = nodes + esz, *p = f;
		int sz = esz, s;

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

//		if(f < to && f->crc < crc || f > nodes && f[-1].crc > crc)
//			int err = 1;

		// Add
		HashListEl *nel = elements.AllocNew();
		nel->SetItem(key, val);

		if(f != (nodes + esz))
			memcpy(f + 1, f, (esz - (f - nodes)) * sizeof(HashListNode));
		
		f->el = nel;
		f->crc = crc;

		esz ++;

		//Test();

		return nel;
	}

	void Test(){
		HashListNode *f = nodes, *to = nodes + esz;

		while(f < to){
			if(f + 1 != to)
				if(f->crc > f[1].crc)
					int err = 1;

			f ++;
		}

		return ;
	}

	template<typename TypeKey>
	HashListEl* Get(TypeKey &key){
		if(!nodes)
			return 0;

		// Get Crc
		unsigned int crc = pel.GetHash(key);

		// Find
		HashListNode *f = nodes, *to = nodes + esz, *p = 0;
		int sz = esz, s;

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

		// Find
		HashListNode *f = nodes, *to = nodes + esz, *p = 0;
		int sz = esz, s;

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
			if(pel.TestHash(f->el, key)){
				elements.AllocFree(f->el);
				memcpy(f, f + 1, (nsz - (f - nodes) - 1) * sizeof(HashListNode));
				esz --;				
				return 1;
			}
			f ++;
		}

		return 0;
	}

	void Clear(){
		HashListNode *f = nodes, *to = nodes + esz;
		while(f < to){
			elements.AllocFree(f->el);
			f ++;
		}

		free(nodes);

		nodes = 0;
		esz = 0;
		nsz = 0;

		return ;
	}


};