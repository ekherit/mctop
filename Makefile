all : mctop

LIBS = `root-config --libs` -lMinuit 
CXXFLAGS = `root-config --cflags` 

BINDIR=$(HOME)/work/bin

mctop : McTop.o  mctop.cpp pdg_table.h decay_topology.h mctop.h
		g++ -o $(BINDIR)/mctop $(CXXFLAGS)  mctop.cpp $(LIBS) McTop.o

McTop.o : McTop.cpp McTop.h pdg_table.h decay_topology.h mctop.h
		g++ -o McTop.o $(CXXFLAGS) -c McTop.cpp 
	
clean :
		rm $(BINDIR)/mctop *.o
