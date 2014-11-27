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
  //resort the map
  std::multimap<Long64_t, mctop_t> Tops;
  for(std::map<mctop_t,Long64_t>::iterator it=mapTop.begin(); it!=mapTop.end(); it++)
  {
    Tops.insert(std::pair<Long64_t, mctop_t>(it->second,it->first));
  }
  int ntop = 0;
  //for(std::map<mctop_t,Long64_t>::iterator it=mapTop.begin(); it!=mapTop.end(); it++)
  for(std::map<Long64_t,mctop_t>::reverse_iterator it=Tops.rbegin(); it!=Tops.rend(); it++)
  {
    mctop_t top = it->second;
    std::cout << "Topology " << ntop << " count " << it->first << " times,  mcidx = " << top.pdgid.size() << ":" << std::endl;
    for(int i=0;i<top.pdgid.size();i++)
    {
      std::cout << std::setw(10) << top.pdgid[i];
    }
    std::cout << std::endl;
    for(int i=0;i<top.mother.size();i++)
    {
      std::cout << std::setw(10) << top.mother[i];
    }
    std::cout << std::endl;
    for(int i=0;i<80;i++) std::cout << "=";
    std::cout << std::endl;
    ntop++;
  }
};
