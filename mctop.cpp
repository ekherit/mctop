#include "McTop.h"
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <stdexcept>
#include <iomanip>


#include "pdg_table.h"


#include <boost/format.hpp>
#include <boost/program_options.hpp>

int main(int argc, char ** argv)
{
  namespace po=boost::program_options;
  po::options_description opt_desc("Allowed options");
  std::string tree_name;
  std::vector<std::string> tree_files;
  opt_desc.add_options()
    ("help,h","Print this help")
    ("nogamma", "Reduce radiative gamma")
    ("reduce", "Merge topology and antitopology")
    //("tree_name",  po::value<std::string>(&tree_name), "Tree name")
    ("tree_files", po::value<std::vector<std::string>>(&tree_files), "List of files")
    ;
  po::positional_options_description pos;
  //pos.add("tree_name",-1);
  pos.add("tree_files",-1);
  po::variables_map opt; //options container
  try
  {
    po::store(po::command_line_parser(argc, argv).options(opt_desc).positional(pos).run(), opt);
    po::notify(opt);
  } 
  catch (boost::program_options::error & po_error)
  {
    std::cerr << "WARGNING: configuration: "<< po_error.what() << std::endl;
  }

  if(opt.count("help") || tree_files.size()<2)
  {
    std::cout << "Usage: mctop <mctopo_tree_name> <file1> [file2] .. [fileN]" << std::endl;
    std::clog << opt_desc;
    return 0;
  }
  McTop mctop(tree_files[0].c_str());
  std::cout << "Loading files:"<< std::endl;
  for(int i=1;i<tree_files.size(); i++)
  {
    std::cout << "Adding file: " << tree_files[i] << std::endl;
    mctop.AddFile(tree_files[i].c_str());
  }
  int count_option=NONE;
  if(opt.count("nogamma")) count_option |=  REDUCE_PHOTON;
  if(opt.count("reduce"))  count_option |=  REDUCE;
  auto TopoMap = mctop.Count2(count_option);
  std::multimap<Long64_t, decay_topology_t> CountMap;
  for(auto & it : TopoMap)
  {
    CountMap.insert({it.second,it.first});
  }
  boost::format fmt_head("%5s %10s %15s  %-20s  %-70s ");
  boost::format      fmt("%5d %10d %15d  %-20s  %-70s ");
  std::cout << fmt_head % "#" % "count" % "hash" % "final state" % "topology" << std::endl;
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
    std::cout << fmt % topology_counter % it.first % top[boost::graph_bundle].hash % final_state(top) % to_string(top)   << std::endl;
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
