enum AListOpt{ AListCon = 1, AListDes = 2, AListClear = 4 };

// Malloc / Free
template<class T, int OT = AListCon | AListDes | AListClear>
class AListAllocDef{

public:
	AListAllocDef(){
	}

	T* AllocNew(){
		T *t = (T*)malloc(sizeof(T));

		if(OT & AListCon)
			new(t)T;
		else
			memset(t, 0, sizeof(T));

		return t;
	}

	void AllocFree(T *t){
		if(OT & AListDes)
			t->~T();
		free(t);
		return ;
	}

};