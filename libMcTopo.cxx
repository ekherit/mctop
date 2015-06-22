#include "libMcTopo.h"
#include <iostream>

unsigned long topology_hash(McTopoBase * mctopo)
{
	McTopo m(*mctopo);
  ///return ::hash(make_topology(*mctopo));
	return m.hash();
}



std::string topology_info(McTopoBase *mctopo)
{
	McTopo m(*mctopo);
  //return to_string(make_topology(*mctopo)).c_str();
	return m.info();
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
