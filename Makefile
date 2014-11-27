all : mctop

LIBS = `root-config --libs` -lMinuit 
CXXFLAGS = `root-config --cflags` 

BINDIR=./

mctop : McTop.o 
		g++ -o $(BINDIR)/mctop $(CXXFLAGS)  mctop.cpp $(LIBS) McTop.o

McTop.o : McTop.cpp McTop.h
		g++ -o McTop.o $(CXXFLAGS) -c McTop.cpp 
	
clean :
		rm $(BINDIR)/mctop *.o
