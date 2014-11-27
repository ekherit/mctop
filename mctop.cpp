#include "McTop.h"
#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>
#include <iomanip>


int main(int argc, char ** argv)
{
  McTop mctop("mctopo");
  std::cout << "Loading files:"<< std::endl;
  for(int i=1;i<argc; i++)
  {
    std::cout << argv[i] << std::endl;
    mctop.AddFile(argv[i]);
  }
  std::map<mctop_t,Long64_t> mapTop = mctop.Count();
  
  int ntop = 0;
  for(std::map<mctop_t,Long64_t>::iterator it=mapTop.begin(); it!=mapTop.end(); it++)
  {
    mctop_t top = it->first;
    std::cout << "Topology " << ntop << " count " << it->second << " times,  mcidx = " << top.pdgid.size() << ":" << std::endl;
    for(int i=0;i<top.pdgid.size();i++)
    {
      std::cout << std::setw(5) << top.pdgid[i];
    }
    std::cout << std::endl;
    for(int i=0;i<top.mother.size();i++)
    {
      std::cout << std::setw(5) << top.mother[i];
    }
    std::cout << std::endl;
  }
};
