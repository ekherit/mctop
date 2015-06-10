all : mctopo libMyEvent.so

LIBS = `root-config --libs` -lMinuit  -lboost_program_options
CXXFLAGS = `root-config --cflags`  -std=c++1y

BINDIR=$(HOME)/work/bin

mctopo : McTopoAdaptor.o  McTopo.o mctopo.cpp pdg_table.h decay_topology.h mctop.h
		g++ -o $(BINDIR)/mctopo $(CXXFLAGS)  mctopo.cpp $(LIBS) McTopoAdaptor.o McTopo.o

McTopo.o : McTopo.cpp
		g++ -o McTopo.o $(CXXFLAGS) -c McTopo.cpp 

McTopoAdaptor.o : McTopoAdaptor.cpp McTopoAdaptor.h pdg_table.h decay_topology.h 
		g++ -o McTopoAdaptor.o $(CXXFLAGS) -c McTopoAdaptor.cpp 


libMcTopoDict.cxx : McTopoLinkDef.h libMcTopo.h
		rootcint -f libMcTopoDict.cxx -c libMcTopo.h McTopoLinkDef.h
		
libMcTopoDict.o . :  libMcTopoDict.cxx
		g++ -o libMcTopoDict.o $(CXXFLAGS) -c libMcTopoDict.cxx


libMcTopo.so: McTopoAdaptor.o  McTopo.o libMcTopoDict.o
		g++ -shared -o libMcTopo.so  McTopoAdaptor.o  McTopo.o libMcTopoDict.o $(LIBS)


libMyEvent.so : MyEvent.o MyEventDict.o McTopo.o McTopoAdaptor.o
		g++ -shared  MyEvent.o MyEventDict.o McTopo.o McTopoAdaptor.o -o libMyEvent.so $(LIBS)

MyEvent.o : MyEvent.cxx
		g++ -o MyEvent.o $(CXXFLAGS) -c MyEvent.cxx

MyEventDict.cxx : MyEvent.h LinkDef.h
		rootcint -f MyEventDict.cxx -c MyEvent.h LinkDef.h
	
MyEventDict.o :  MyEventDict.cxx
		g++ -o MyEventDict.o $(CXXFLAGS) -c MyEventDict.cxx

clean :
		@rm -f $(BINDIR)/mctop *.o *.so *Dict.h *Dict.cxx
