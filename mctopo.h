#pragma once

#include "decay_topology.h"
#include "RootMCTopo.h"






inline decay_topology_t make_topology(const RootMCTopo  & m)
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

inline unsigned long mctopo_hash(RootMCTopo  * m) 
{
  auto top = make_topology(*m);
  return top[boost::graph_bundle].hash();
}

inline std::string mctopo_info(RootMCTopo  * m) 
{
  auto top = make_topology(*m);
  return to_string(top);
}
