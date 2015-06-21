#include "libMcTopo.h"
#include <iostream>
#include "McTopoAdaptor.h"
//ClassImp(MyEvent);
//
//MyEvent::MyEvent()
//{
//  std::cout << "Creating MyEvent"<< std::endl;
//}
//MyEvent::MyEvent(McTopo *mct)
//{
//  mctopo = mct;
//  std::cout << "Creating from mctopo"<< std::endl;
//}
//
//unsigned long MyEvent::hash(void)
//{
//  McTopoAdaptor ma(*mctopo);
//  auto topology = ma.MakeDecayTopology();
//  return ::hash(topology);
//}
//
//unsigned long hash(McTopo * mctopo)
//{
//  McTopoAdaptor ma(*mctopo);
//  auto topology = ma.MakeDecayTopology();
//  return ::hash(topology);
//}

decay_topology_t make_topology(McTopo *mctopo)
{
  decay_topology_t top;
  int indexmc = mctopo->indexmc;
  for(int i=0;i<indexmc; i++)
  {
    for(int j=0;j<indexmc;j++)
    {
      if(mctopo->idx[i] == mctopo->motheridx[j])
      {
        vertex_t parent(i);
        vertex_t child(j);
        boost::add_edge(parent,child,top);
        top[parent].name = PdgTable[top[parent].pdgid];
        top[parent].pdgid = mctopo->pdgid[parent];
        top[child].pdgid = mctopo->pdgid[child];
        top[child].name = PdgTable[top[child].pdgid];
      }
    }
  }
  return top;
}


unsigned long topology_hash(McTopo *mctopo)
{
  return hash(make_topology(mctopo));
}

const char * topology_info(McTopo *mctopo)
{
  return to_string(make_topology(mctopo)).c_str();
}
