#include "libMcTopo.h"
#include <iostream>
#include "McTopoAdaptor.h"


unsigned long topology_hash(McTopo * mctopo)
{
  return ::hash(make_topology(*mctopo));
}


const char * topology_info(McTopo *mctopo)
{
  return to_string(make_topology(*mctopo)).c_str();
}


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
