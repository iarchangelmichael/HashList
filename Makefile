CC=gcc
CXX=g++
RM=rm -f
SOURCE=HashList.cpp
CXXFLAGS= -std=c++11

all: hash

std: hash
		CXXFLAGS+= -DHASHTESTSTD=1

hash: $(SOURCE)
		$(CXX) $(SOURCE) $(CXXFLAGS) -o hashlist

clean:
		rm ./hashlist