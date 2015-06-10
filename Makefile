all : mctopo

LIBS = `root-config --libs` -lMinuit  -lboost_program_options
CXXFLAGS = `root-config --cflags`  -std=c++1y

BINDIR=$(HOME)/work/bin

mctopo : McTopoAdaptor.o  McTopo.o mctopo.cpp pdg_table.h decay_topology.h mctop.h
		g++ -o $(BINDIR)/mctopo $(CXXFLAGS)  mctopo.cpp $(LIBS) McTopoAdaptor.o McTopo.o

McTopo.o : McTopo.cpp
		g++ -o McTopo.o $(CXXFLAGS) -c McTopo.cpp 

McTopoAdaptor.o : McTopoAdaptor.cpp McTopoAdaptor.h pdg_table.h decay_topology.h 
		g++ -o McTopoAdaptor.o $(CXXFLAGS) -c McTopoAdaptor.cpp 
	
clean :
		rm $(BINDIR)/mctop *.o
