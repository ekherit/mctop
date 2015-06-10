all : mctop

LIBS = `root-config --libs` -lMinuit  -lboost_program_options
CXXFLAGS = `root-config --cflags`  -std=c++1y

BINDIR=$(HOME)/work/bin

mctop : McTopoAdaptor.o  McTopo.o mctop.cpp pdg_table.h decay_topology.h mctop.h
		g++ -o $(BINDIR)/mctop $(CXXFLAGS)  mctop.cpp $(LIBS) McTopoAdaptor.o McTopo.o

McTopo.o : McTopo.cpp
		g++ -o McTopo.o $(CXXFLAGS) -c McTopo.cpp 

McTopoAdaptor.o : McTopoAdaptor.cpp McTopoAdaptor.h pdg_table.h decay_topology.h 
		g++ -o McTopoAdaptor.o $(CXXFLAGS) -c McTopoAdaptor.cpp 
	
clean :
		rm $(BINDIR)/mctop *.o
