all : mctop

LIBS = `root-config --libs` -lMinuit 
CXXFLAGS = `root-config --cflags` 

BINDIR=$(HOME)/work/bin

mctop : McTop.o  mctop.cpp pdg_table.h
		g++ -o $(BINDIR)/mctop $(CXXFLAGS)  mctop.cpp $(LIBS) McTop.o

McTop.o : McTop.cpp McTop.h pdg_table.h
		g++ -o McTop.o $(CXXFLAGS) -c McTop.cpp 
	
graph_test : graph_test.cpp
		g++  graph_test.cpp $(CXXFLAGS) -o graph_test

clean :
		rm $(BINDIR)/mctop *.o
