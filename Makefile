CC=gcc
CXX=g++
RM=rm -f
SOURCE=HashList.cpp
CXXFLAGS= ${HLFLAGS} -std=c++11

all:
	echo "Use: make [hl | hlf | std | stdu]"

hl: CXXFLAGS+= -DHLTYPE=1
hl: build

hlf: CXXFLAGS+= -DHLTYPE=2
hlf: build

std: CXXFLAGS+= -DHLTYPE=3
std: build

stdu: CXXFLAGS+= -DHLTYPE=4
stdu: build

ht: CXXFLAGS+= -DHLTYPE=5
ht: build

build: $(SOURCE)
	$(CXX) $(SOURCE) $(CXXFLAGS) -o hashlist

clean:
	rm ./hashlist