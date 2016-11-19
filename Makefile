CC=gcc
CXX=g++
RM=rm -f
SOURCE=HashList.cpp
CXXFLAGS= -std=c++11

all:
echo "Use: make [hash | std]"
echo "hash - for use HashList"
echo "std - for use std::unordered_map"

hash: build

std: build
CXXFLAGS+= -DHASHTESTSTD=1

build: $(SOURCE)
	$(CXX) $(SOURCE) $(CXXFLAGS) -o hashlist

clean:
	rm ./hashlist