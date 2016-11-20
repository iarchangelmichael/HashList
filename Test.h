/// TEST
class HashListTest : public HashListElProto<HashListTest>{
	char* key;
	char* val;

public:
	void SetItem(const char* k, const char* v){
		key = (char*)k;
		val = (char*)v;
	}

	unsigned int GetHash(){
		return crc32((unsigned char*)key, strlen(key));
	}

	unsigned int GetHash(const char* key){
		return crc32((unsigned char*)key, strlen(key));
	}

	bool TestHash(HashListTest *el, const char* key){
		return !strcmp(el->key, key);
	}

};

class HashListIntTest : public HashListElProto<HashListIntTest>{
	unsigned int key;
	unsigned int val;

public:
	void SetItem(unsigned int k, unsigned int v){
		key = k;
		val = v;
	}

	unsigned int GetHash(){
		return key;
	}

	unsigned int GetHash(unsigned int &key){
		return key;
	}

	bool TestHash(HashListIntTest *el, unsigned int &key){
		return el->key == key;
	}

};

class HashTreeIntTest : public HashTreeElProto<HashTreeIntTest>{
	unsigned int key;
	unsigned int val;

public:
	void SetItem(unsigned int k, unsigned int v){
		key = k;
		val = v;
	}

	unsigned int GetHash(){
		return key;
	}

	unsigned int GetHash(unsigned int &key){
		return key;
	}

	bool TestHash(HashTreeIntTest *el, unsigned int &key){
		return el->key == key;
	}

};



#include <iostream>
#include <map>
#include <unordered_map>
using namespace std;

///#define HASHTESTSTD

class HashTestStructTime{
public:
	// All time, count, maximum, minimum, avg
	int all, cnt, max, avg, min;

	HashTestStructTime(){
		//memset(this, 0, sizeof(this));
		Clear();
	}

	void Add(int tm){
		if(max < tm)
			max = tm;

		if(min > tm)
			min = tm;

		all += tm;
		cnt ++;
		avg = all / cnt;
	}

	void Clear(){
		all = 0;
		cnt = 0;
		max = 0;
		avg = 0;
		min = 999999999;
	}

};

class HashTestStruct{
public:
	// array size
	int asz;

	// iterations
	int its;

	// Time
	HashTestStructTime tadd, tget, tdel; 

	HashTestStruct(){
		asz = 1024 * 1024;
		its = 128;
	}

	void ClearTime(){
		tadd.Clear();
		tget.Clear();
		tdel.Clear();
	}

};


int HashTestIntOne(HashTestStruct &hts);

int HashTestInt(HashTestStruct &hts){
	printf("Run Hash Test(%s). int[%d] * %d iterations.\r\n", hlname, hts.asz, hts.its);

	for(int i = 0; i < hts.its; i ++)
		HashTestIntOne(hts);

	printf("Result Hash Test(%s). Data: int[%d] * %d iterations.\r\n", hlname, hts.asz, hts.its);
	printf("Action: max / avg / min ms\r\n", hts.tadd.max, hts.tadd.avg, hts.tadd.min);
	printf("Insert: %d / %d / %d ms\r\n", hts.tadd.max, hts.tadd.avg, hts.tadd.min);
	printf("Find: %d / %d / %d ms\r\n", hts.tget.max, hts.tget.avg, hts.tget.min);
	printf("Del: %d / %d / %d ms\r\n", hts.tdel.max, hts.tdel.avg, hts.tdel.min);

	return 0;
}

int HashTestIntOne(HashTestStruct &hts){
#ifndef HLUSESTD
	#if HLTYPE == HLTYPE_HL
		HashList<HashListIntTest> list;
	#endif

	#if HLTYPE == HLTYPE_HLF
		HashListFive<HashListIntTest> list;
	#endif

	#if HLTYPE == HLTYPE_HT
		HashTree<HashTreeIntTest> list;
	#endif

	#if HLTYPE == HLTYPE_HLS
		HashListSix<HashTreeIntTest> list;
	#endif
	//HashListIntTest *p;

	#if HLTYPE == HLTYPE_AT // Disable error
		HashList<HashListIntTest> list;
	#endif

#else
	#if HLTYPE == HLTYPE_STD
		map <int, int> list;
	#endif

	#if HLTYPE == HLTYPE_STDU
		unordered_map <int, int> list;
	#endif	
#endif

	srand((unsigned int)time(0));

	//printf("Run Hash Test. int[%d]\r\n", asz);

	// Int Array
	unsigned int *arr = (unsigned int*) malloc(hts.asz * sizeof(unsigned int)), rnd = rand();

	for(int i = 0; i < hts.asz; i ++){
		arr[i] = rand();
		
		if(!arr[i])
			i --;
	}

	// Insert
	printf("Test: Insert...");
	tbtime;
	for(int i = 0; i < hts.asz; i ++){
#ifndef HLUSESTD
		if(!list.Add(arr[i], rnd))
			printf("Insert FAIL! ");			
#else
		list.insert(pair<char,int>(arr[i], rnd));
#endif
		rnd += i;
	}
	tetime;
	hts.tadd.Add(tbtime_result);
	printf("%d ms.\r\n", tbtime_result);

	// Get
	printf("Test: Find...");
	tctime;
	for(int i = 0; i < hts.asz; i ++){
#ifndef HLUSESTD
		if(!list.Get(arr[i]))
			
#else
		if(!list.find(arr[i])->first)
#endif
			printf("Find FAIL! ");
	}
	tetime;
	hts.tget.Add(tbtime_result);
	printf("%d ms.\r\n", tbtime_result);

	// Get
	printf("Test: Del...");
	tctime;
	for(int i = 0; i < hts.asz; i ++){
#ifndef HLUSESTD
		if(!list.Del(arr[i]))
			printf("Del FAIL! ");			
#else
		list.erase(arr[i]);
#endif
			
	}
	tetime;
	hts.tdel.Add(tbtime_result);
	printf("%d ms.\r\n", tbtime_result);

	free(arr);

	return 0;
}



class HashTestTwoInt{
public:
	int a, b;

};

template<class AllocClass>
int HashTestAllocateOne(HashTestStruct &hts){
	AllocClass list;

	HashTestTwoInt **arr = (HashTestTwoInt**) malloc(hts.asz * sizeof(void*));
	
	tbtime;
	for(int i = 0; i < hts.asz; i ++){
		arr[i] = list.AllocNew();
	}
	tetime;

	hts.tadd.Add(tbtime_result);

	tctime;
	for(int i = 0; i < hts.asz; i ++){
		list.AllocFree(arr[i]);
	}
	tetime;

	hts.tdel.Add(tbtime_result);	
	
	free(arr);

	return 0;
}


int HashTestAllocate(HashTestStruct &hts){
	char *inf;

	printf("Run Allocate Test. int[%d] * %d iterations.\r\n", hts.asz, hts.its);


	// Def
	hts.ClearTime();
	hts.asz /= 64;

	for(int i = 0; i < hts.its; i ++){
		HashTestAllocateOne<AListAllocDef<HashTestTwoInt>>(hts);
		printf(".");
	}
	printf("\r\n");
	hts.asz *= 64;

	inf = "[Def] ";

	//printf("Result Hash Test(%s). Data: int[%d] * %d iterations.\r\n", hlname, hts.asz, hts.its);
	//printf("Action: max / avg / min ms\r\n", hts.tadd.max, hts.tadd.avg, hts.tadd.min);
	printf("%sInsert: %d / %d / %d ms\r\n", inf, hts.tadd.max, hts.tadd.avg, hts.tadd.min);
	//printf("Find: %d / %d / %d ms\r\n", hts.tget.max, hts.tget.avg, hts.tget.min);
	printf("%sDel: %d / %d / %d ms\r\n", inf, hts.tdel.max, hts.tdel.avg, hts.tdel.min);
	

	// UList
	hts.ClearTime();

	for(int i = 0; i < hts.its; i ++){
		HashTestAllocateOne<AListAllocUList<HashTestTwoInt>>(hts);
		printf(".");
	}
	printf("\r\n");

	inf = "[UList] ";
	printf("%sInsert: %d / %d / %d ms\r\n", inf, hts.tadd.max, hts.tadd.avg, hts.tadd.min);
	printf("%sDel: %d / %d / %d ms\r\n", inf, hts.tdel.max, hts.tdel.avg, hts.tdel.min);

	// OList
	hts.ClearTime();

	for(int i = 0; i < hts.its; i ++){
		HashTestAllocateOne<AListAllocOList<HashTestTwoInt>>(hts);
		printf(".");
	}
	printf("\r\n");

	inf = "[OList] ";
	printf("%sInsert: %d / %d / %d ms\r\n", inf, hts.tadd.max, hts.tadd.avg, hts.tadd.min);
	printf("%sDel: %d / %d / %d ms\r\n", inf, hts.tdel.max, hts.tdel.avg, hts.tdel.min);

	return 0;
}