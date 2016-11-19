// Hash List
// 18.11.2016 14:27

// Configure:
// Set define HASHTESTSTD for use std::map

//#define HASHTESTSTD

#define HLTYPE_HL	1
#define HLTYPE_HLF	2
#define HLTYPE_STD	3
#define HLTYPE_STDU	4
#define HLTYPE_HT	5
#define HLTYPE_HLS	6

#ifndef HLSIZE
	#define HLSIZE (1024 * 1024)
#endif

#ifndef HLITS
	#define HLITS (128)
#endif

#if HLTYPE == HLTYPE_HL
	const char* hlname = "HashList";
#endif

#if HLTYPE == HLTYPE_HLF
	const char* hlname = "HashListFive";
#endif

#if HLTYPE == HLTYPE_STD
	#define HLUSESTD
	const char* hlname = "std::map";
#endif

#if HLTYPE == HLTYPE_STDU
	#define HLUSESTD
	const char* hlname = "std::unordered_map";
#endif

#if HLTYPE == HLTYPE_HT
	const char* hlname = "HashTree";
#endif

#if HLTYPE == HLTYPE_HLS
	const char* hlname = "HashListSix";
#endif


#ifndef HLTYPE
	#define HLSTOP
	#define HLTYPE HLTYPE_HL
	const char* hlname = "Unknown.";
#endif



// Special functions & defines
#include "special.h"

// Allocate memory
#include "AList.h"

// Hash List class
#include "HashList.h"

// Hash List Five class
#include "HashListFive.h"

// Hash List Six class
#include "HashListSix.h"

// Hash Tree class
#include "HashTree.h"

// Testing
#include "Test.h"


int main(){
	printf("Hash List project. Build: %s\r\n", hlname);

#ifdef HLSTOP
	printf("Please rebuild used HLTYPE.\r\n");
	return 0;
#endif

	HashTestStruct hts;

	hts.asz = HLSIZE;
	hts.its = HLITS;

	HashTestInt(hts);

	return 0;
}


