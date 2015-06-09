#define McTop_cxx
#include "McTop.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <stdexcept>
#include <iostream>

#include "pdg_table.h"

std::map<mctop_t,Long64_t> McTop::Count()
{
   if (fChain == 0) throw std::runtime_error("No chain");

   Long64_t nentries = fChain->GetEntriesFast();
   std::map<mctop_t, Long64_t> TopMap;
   Long64_t nbytes = 0, nb = 0;
   Long64_t event_counter=0;
   for (Long64_t jentry=0; jentry<nentries;jentry++,event_counter++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      mctop_t top;
      top.resize(indexmc);
      for(int i=0;i<indexmc; i++)
      {
        top.pdgid[i]=pdgid[i];
        top.mother[i]=motheridx[i];
        top.idx[i] = idx[i];
      }
      //ntop is charge conjugated topology
      mctop_t ntop = top;
      //fill it from initial topology
      for(int i=0;i<indexmc; i++)
      {
        if(ntop.pdgid[i]==100443 || ntop.pdgid[i]==443) continue;
        ntop.pdgid[i]=-ntop.pdgid[i];
        ntop.mother[i]=motheridx[i];
        ntop.idx[i]=idx[i];
      }
      //std::cout << hash(top) << std::endl;
      std::map<mctop_t, Long64_t>::iterator it;
      //find topology in the TopMap
      it = TopMap.find(top);
      //if unable to find 
      if(it==TopMap.end())
      {
        //then try to find anti topology
        it = TopMap.find(ntop);
        //if unable to find ant topolgoy then create new one
        if(it==TopMap.end()) it = TopMap.insert(it,std::pair<mctop_t, Long64_t>(top,0));
      }
      //here we alwase has correct iterator over TopMap container
      it->second++;
      //TopMap[top]++;
   }
   return TopMap;
}


std::map<decay_topology_t, Long64_t> McTop::Count2(void)
{
  if (fChain == 0) throw std::runtime_error("No chain");

  Long64_t nentries = fChain->GetEntriesFast();
  //std::map<mctop_t, Long64_t> TopMap;
  std::map<decay_topology_t, Long64_t> TopoMap;
  Long64_t nbytes = 0, nb = 0;
  Long64_t event_counter=0;
  for (Long64_t jentry=0; jentry<nentries;jentry++,event_counter++)
  {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    //create topology with indexmc vertex
    decay_topology_t top(indexmc);;
    //fill the topology
    for(int i=0;i<indexmc; i++)
    {
      for(int j=i+1;j<indexmc;j++)
      {
        if(idx[i] == motheridx[j])
        {
          boost::add_edge(i,j,top);
          top[i].pdgid = pdgid[i];
          top[j].pdgid = pdgid[j];
          top[i].name = PdgTable[top[i].pdgid];
          top[j].name = PdgTable[top[j].pdgid];
        }
      }
    }
    top[boost::graph_bundle].hash = hash(top);
    TopoMap[top]++;
    //std::cout << "hash = " << hash(top) << std::endl;
    //std::list<int> root_list;
    //find_root(top,root_list,indexmc-1);
    //if(root_list.empty())
    //{
    //  std::cerr << "WARNING: Unable to find root of the decay graph for entry " << jentry << std::endl;
    //}
    //if(root_list.size()>1)
    //{
    //  std::cerr << "WARNING: To much root vertex: " << root_list.size() << std::endl;
    //}

    //std::cout << "The root of the decay tree is: " << root_list.front() << std::endl;
  }
  return TopoMap;
}
