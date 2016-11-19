// Hash List
// 18.11.2016 14:27

// Configure:
// Set define HASHTESTSTD for use std::map

//#define HASHTESTSTD

// Standart defines
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <new>

// Allocate memory
#include "AList.h"

// Special functions & defines
#include "special.h"

// Hash List class
#include "HashList.h"

// Hash List Five class
#include "HashListFive.h"

// Testing
#include "Test.h"


int main(){

	HashTestStruct hts;
	HashTestInt(hts);

	return 0;
}