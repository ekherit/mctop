all : mctopo libMcTopo.so

LIBS = `root-config --libs` -lMinuit  -lboost_program_options
CXXFLAGS = `root-config --cflags`  -std=c++1y -fPIC

BINDIR=$(HOME)/work/bin

mctopo : McTopo.o  McTopoBase.o mctopo.cpp pdg_table.h decay_topology.h 
		g++ -o $(BINDIR)/mctopo $(CXXFLAGS)  mctopo.cpp $(LIBS) McTopo.o McTopoBase.o

McTopoBase.o : McTopoBase.cpp
		g++ -o McTopoBase.o $(CXXFLAGS) -c McTopoBase.cpp 

McTopo.o : McTopo.cpp McTopo.h pdg_table.h decay_topology.h 
		g++ -o McTopo.o $(CXXFLAGS) -c McTopo.cpp 


libMcTopo.so : libMcTopo.o libMcTopoDict.o McTopoBase.o McTopo.o
		g++ -fPIC -shared  libMcTopo.o libMcTopoDict.o McTopoBase.o McTopo.o -o libMcTopo.so $(LIBS)

libMcTopo.o : libMcTopo.cxx
		g++ -fPIC -o libMcTopo.o $(CXXFLAGS) -c libMcTopo.cxx

libMcTopoDict.cxx : libMcTopo.h libMcTopoLinkDef.h
		rootcint -f libMcTopoDict.cxx -c libMcTopo.h libMcTopoLinkDef.h
	
libMcTopoDict.o :  libMcTopoDict.cxx
		g++ -o libMcTopoDict.o $(CXXFLAGS) -c libMcTopoDict.cxx

clean :
		@rm -f $(BINDIR)/mctopo *.o *.so *Dict.h *Dict.cxx *~ *.pcm
