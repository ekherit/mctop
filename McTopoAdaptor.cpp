#include "McTopoAdaptor.h"

McTopoAdaptor::McTopoAdaptor(McTopo & mctopo)
{
  fChain = mctopo.fChain;
  chain = (TChain*) fChain;
  indexmc = mctopo.indexmc;

  Init(fChain);
  for(int i =0;i<sizeof(mctopo.pdgid)/sizeof(Int_t);i++)
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

decay_topology_t McTopoAdaptor::MakeDecayTopology(void)
{
  decay_topology_t top;
  //fill the topology
  for(int i=0;i<indexmc; i++)
  {
    for(int j=0;j<indexmc;j++)
    {
      if(idx[i] == motheridx[j])
      {
        vertex_t parent(i);
        vertex_t child(j);
        boost::add_edge(parent,child,top);
        top[parent].name = PdgTable[top[parent].pdgid];
        top[parent].pdgid = pdgid[parent];
        top[child].pdgid = pdgid[child];
        top[child].name = PdgTable[top[child].pdgid];
      }
    }
  }
  return top;
}

std::map<decay_topology_t, Long64_t> McTopoAdaptor::Count(int opt)
{
  if (fChain == 0) throw std::runtime_error("No chain");
  Long64_t nentries = fChain->GetEntriesFast();
  std::map<decay_topology_t, Long64_t> TopoMap;
  Long64_t nbytes = 0, nb = 0;
  Long64_t event_counter=0;
  for (Long64_t jentry=0; jentry<nentries;jentry++,event_counter++)
  {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    decay_topology_t top = MakeDecayTopology();
    //remove radiative photons if needed
    if(opt & REDUCE_PHOTON) remove_particle(-22,top);
    //merge process and conjugated process
    top[boost::graph_bundle].hash = hash(top);
    auto  it = TopoMap.find(top);
    if(opt & REDUCE && it == end(TopoMap)) //if unable to find topology the conjucate it
    {
      top = conj(top);
    }
    //cout topology
    TopoMap[top]++;
  }
  return TopoMap;
}
