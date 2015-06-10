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

libmctopo.o : libmctopo.h McTopoAdaptor.h pdg_table.h decay_topology.h 
		g++ -o libmctopo.o $(CXXFLAGS) -c libmctopo.cpp 


libmctopo.so: $(LIBFILE)  $(DICTOBJ)
	$(CMDPREFIX)g++ -shared -o $(ROOTLIB) $(DICTOBJ)  $(LIBFILE) -lpq -lcrypt


#генерация словарика
$(DICTCXX): $(DICTCLASS).h $(DICTCLASS)LinkDef.h
	$(CMDPREFIX)rootcint -f $(DICTCXX) -c $(DICTCLASS).h $(DICTCLASS)LinkDef.h

	
clean :
		rm $(BINDIR)/mctop *.o
