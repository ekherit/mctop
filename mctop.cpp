#include "McTop.h"
#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>
#include <iomanip>


int main(int argc, char ** argv)
{
  if(argc<3) 
  {
    std::cout << "Usage: mctop <mctopo_tree_name> <file1> [file2] .. [fileN]" << std::endl;
    exit(1);
  }
  McTop mctop(argv[1]);
  std::cout << "Loading files:"<< std::endl;
  for(int i=2;i<argc; i++)
  {
    std::cout << "Adding file: " << argv[i] << std::endl;
    mctop.AddFile(argv[i]);
  }
  std::map<mctop_t,Long64_t> mapTop = mctop.Count();
  //resort the map
  std::multimap<Long64_t, mctop_t> Tops;
  std::map<int,std::string> PdgTable;
  PdgTable[-22]="gamma";
  PdgTable[22]="gamma";
  PdgTable[100443]="psi(2S)";
  PdgTable[443]="J/psi";
  PdgTable[13]="mu+";
  PdgTable[-13]="mu-";
  PdgTable[-11]="e-";
  PdgTable[+11]="e+";
  PdgTable[-321]="K-";
  PdgTable[+321]="K+";
  PdgTable[+211]="pi+";
  PdgTable[-211]="pi-";
  PdgTable[10323]="K1(1270)+";
  PdgTable[-10323]="K1(1270)-";
  PdgTable[-313]="-K*(892)0";
  PdgTable[313]="K*(892)0";
  PdgTable[10311]="K0*(1430)0";
  PdgTable[-10311]="-K0*(1430)0";
  PdgTable[113]="rho(770)0";
  PdgTable[-113]="-rho(770)0";
  PdgTable[213]="rho(770)+";
  PdgTable[-213]="rho(770)-";
  for(std::map<mctop_t,Long64_t>::iterator it=mapTop.begin(); it!=mapTop.end(); it++)
  {
    Tops.insert(std::pair<Long64_t, mctop_t>(it->second,it->first));
  }
  int ntop = 0;
  //for(std::map<mctop_t,Long64_t>::iterator it=mapTop.begin(); it!=mapTop.end(); it++)
  Long64_t event_counter=0;
  for(std::map<Long64_t,mctop_t>::reverse_iterator it=Tops.rbegin(); it!=Tops.rend(); it++)
  {
    mctop_t top = it->second;
    std::cout << "Topology " << ntop << " count " << it->first << " times,  mcidx = " << top.pdgid.size() << ":" << std::endl;
    event_counter+=it->first;
    for(int i=0;i<top.pdgid.size();i++)
    {
      std::map<int,std::string>::iterator it = PdgTable.find(top.pdgid[i]);
      if(it!=PdgTable.end()) std::cout << std::setw(13) << PdgTable[top.pdgid[i]];
      else std::cout << std::setw(13) << top.pdgid[i];
    }
    std::cout << std::endl;
    for(int i=0;i<top.pdgid.size();i++)
    {
      std::cout << std::setw(13) << top.pdgid[i];
    }
    std::cout << std::endl;
    for(int i=0;i<top.mother.size();i++)
    {
      std::cout << std::setw(13) << top.mother[i];
    }
    std::cout << std::endl;
    for(int i=0;i<80;i++) std::cout << "=";
    std::cout << std::endl;
    ntop++;
  }
  std::cout << "Total number of events: " << event_counter << std::endl;
  if(!Tops.empty())
  {
    std::cout << "Found " << Tops.size() << " different topologies" << std::endl;
    std::cout << "Most probable topology: " << Tops.rbegin()->first << std::endl;
    std::cout << "Other topologies: " << event_counter - Tops.rbegin()->first << std::endl;
  }
};
