#include "McTopoAdaptor.h"

McTopoAdaptor::McTopoAdaptor(McTopo & mctopo)
{
  //fChain = mctopo.fChain;
  //chain = (TChain*) fChain;
  //Init(fChain);
  chain =0;
  fChain = 0;
  indexmc = mctopo.indexmc;
  for(int i =0;i<indexmc;i++)
  {
    pdgid[i] = mctopo.pdgid[i];
    motheridx[i] = mctopo.motheridx[i];
    idx[i] = mctopo.idx[i];
  }
}

McTopoAdaptor::McTopoAdaptor(std::string tree_name)
{
  chain= new TChain(tree_name.c_str(), tree_name.c_str());
  fChain=chain;
  Init(fChain);
}

void McTopoAdaptor::AddFile(std::string  file_name)
{
  if(chain)
  {
    chain->AddFile(file_name.c_str());
  }
}

decay_topology_t make_topology(const McTopo  & m)
{
  decay_topology_t top;
  //fill the topology
  for(int i=0;i<m.indexmc; i++)
  {
    for(int j=0;j<m.indexmc;j++)
    {
      if(m.idx[i] == m.motheridx[j])
      {
        vertex_t parent(i);
        vertex_t child(j);
        boost::add_edge(parent,child,top);
        top[parent].name = PdgTable[top[parent].pdgid];
        top[parent].pdgid = m.pdgid[parent];
        top[child].pdgid = m.pdgid[child];
        top[child].name = PdgTable[top[child].pdgid];
      }
    }
  }
  add_hash(top);
  return top;
}

decay_topology_t McTopoAdaptor::MakeDecayTopology(void)
{
  return make_topology(*this);
}

std::map<decay_topology_t, Long64_t> McTopoAdaptor::Count(unsigned long long  N, int opt)
{
  if (fChain == 0) throw std::runtime_error("No chain");
  Long64_t nentries = fChain->GetEntriesFast();
  if(N>0) nentries = N;
  std::map<decay_topology_t, Long64_t> TopoMap;
  Long64_t nbytes = 0, nb = 0;
  Long64_t event_counter=0;
  for (Long64_t jentry=0; jentry<nentries;jentry++,event_counter++)
  {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    decay_topology_t top = MakeDecayTopology();
    auto initial_hash = top[boost::graph_bundle].hash();
    //remove radiative photons if needed
    if(opt & REDUCE_PHOTON) 
    {
      remove_particle(-22,top);
    }
    auto  it = TopoMap.find(top);
    if(it == end(TopoMap)) //if unable to find topology the conjucate it
    {
      if(opt & REDUCE) top = conj(top);
      it = TopoMap.insert(it,{top,0});
    }
    //now it is alwais look into existing topology
    it->second++; //count topology
    //update hash list
    std::list<unsigned long> &  lst = const_cast<std::list<unsigned long>&>(it->first[boost::graph_bundle].hash_list);
    std::list<unsigned long>::iterator i = begin(lst); //pointer to main hash
    lst.insert(lst.end(),initial_hash);
    lst.sort();
    std::swap(*i,*begin(lst));
    lst.unique();
  }
  return TopoMap;
}
