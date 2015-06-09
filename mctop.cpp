#include "McTop.h"
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <stdexcept>
#include <iomanip>


#include "pdg_table.h"


#include <boost/format.hpp>

int main(int argc, char ** argv)
{
  std::cout << PdgTable[-22] << std::endl;
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
  auto TopoMap = mctop.Count2();
  std::multimap<Long64_t, decay_topology_t> CountMap;
  for(auto & it : TopoMap)
  {
    CountMap.insert({it.second,it.first});
  }
  boost::format fmt_head("%5s %10s %15s %10s %-60s");
  boost::format      fmt("%5d %10d %15d %10d %-60s");
  std::cout << fmt_head % "#" % "count" % "hash" % "num part" % "topology"<< std::endl;
  Long64_t event_counter =0;
  Long64_t topology_counter=0;
  for(auto & it : CountMap)
  {
    auto & top = it.second;
    //std::list<int> root_list;
    //find_root(top,root_list,0);
    //std::function<void(int)> print_topo;
    //std::ostringstream os;
    //print_topo = [&](int idx)
    //{
    //  decay_topology_t::adjacency_iterator begin, end;
    //  tie(begin, end) = adjacent_vertices(idx, top);
    //  std::string name = top[idx].name;
    //  if(name == "") name = "["+std::to_string(top[idx].pdgid)+"]";
    //  if(begin != end)
    //  {
    //    os << "(" << name << " -> ";
    //    for (; begin != end; ++begin)
    //    {   
    //      print_topo(*begin);
    //    }
    //    os << ")";
    //  }
    //  else 
    //  {
    //    os << name;
    //  }
    //};
    //for(auto root : root_list)
    //{
    //  print_topo(root);
    //  os << ",";
    //}
    std::cout << fmt % topology_counter % it.first % top[boost::graph_bundle].hash % num_vertices(top) % to_string(top) << std::endl;
    topology_counter++;
    event_counter+=it.first;
  }
  std::cout << "Total number of events: " << event_counter << std::endl;
  if(!CountMap.empty())
  {
    std::cout << "Found " << CountMap.size() << " different topologies" << std::endl;
    std::cout << "Most probable topology: " << CountMap.rbegin()->first << std::endl;
    std::cout << "Other topologies: " << event_counter - CountMap.rbegin()->first << std::endl;
  }
};
