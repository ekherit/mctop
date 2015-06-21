all : mctopo libMcTopo.so

LIBS = `root-config --libs` -lMinuit  -lboost_program_options
CXXFLAGS = `root-config --cflags`  -std=c++1y -fPIC

BINDIR=$(HOME)/work/bin

mctopo : McTopoAdaptor.o  McTopo.o mctopo.cpp pdg_table.h decay_topology.h 
		g++ -o $(BINDIR)/mctopo $(CXXFLAGS)  mctopo.cpp $(LIBS) McTopoAdaptor.o McTopo.o

McTopo.o : McTopo.cpp
		g++ -o McTopo.o $(CXXFLAGS) -c McTopo.cpp 

McTopoAdaptor.o : McTopoAdaptor.cpp McTopoAdaptor.h pdg_table.h decay_topology.h 
		g++ -o McTopoAdaptor.o $(CXXFLAGS) -c McTopoAdaptor.cpp 


libMcTopo.so : libMcTopo.o libMcTopoDict.o McTopo.o McTopoAdaptor.o
		g++ -fPIC -shared  libMcTopo.o libMcTopoDict.o McTopo.o McTopoAdaptor.o -o libMcTopo.so $(LIBS)

libMcTopo.o : libMcTopo.cxx
		g++ -fPIC -o libMcTopo.o $(CXXFLAGS) -c libMcTopo.cxx

libMcTopoDict.cxx : libMcTopo.h libMcTopoLinkDef.h
		rootcint -f libMcTopoDict.cxx -c libMcTopo.h libMcTopoLinkDef.h
	
libMcTopoDict.o :  libMcTopoDict.cxx
		g++ -o libMcTopoDict.o $(CXXFLAGS) -c libMcTopoDict.cxx

clean :
		@rm -f $(BINDIR)/mctopo *.o *.so *Dict.h *Dict.cxx
