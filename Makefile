all : mctopo  libMcTopo.so

LIBS = `root-config --libs` -lMinuit  -lboost_program_options
CXXFLAGS = `root-config --cflags` -std=c++1y -fPIC

BINDIR=$(HOME)/work/bin


mctopo :  mctopo.cpp pdg_table.h decay_topology.h RootMCTopo.o libMcTopoDict.o
		g++ -o $(BINDIR)/$@ $(CXXFLAGS) $< $(LIBS) RootMCTopo.o libMcTopoDict.o



libMcTopo.so : RootMCTopo.o libMcTopoDict.o
		g++ -fPIC -shared  -o $@ $(LIBS) $^ 
	
libMcTopoDict.o :  libMcTopoDict.cxx
		g++ -o $@ $(CXXFLAGS) $(LIBS) -c $^

libMcTopoDict.cxx : RootMCTopo.h McTopoLinkDef.h
		rootcint -f $@ -c $^

RootMCTopo.o : RootMCTopo.C
	  g++ -o $@ $(CXXFLAGS) $(LIBS) -c  $^

clean :
		@rm -f $(BINDIR)/mctopo *.o *.so *Dict.h *Dict.cxx *~ *.pcm


#  $@ -- имя цели
#  $^ -- список всех зависимостей
#  $< -- имя первой зависиомости
