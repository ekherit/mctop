#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <stdexcept>
#include <iomanip>


#include "pdg_table.h"

#include "McTopoAdaptor.h"

#include <boost/format.hpp>
#include <boost/program_options.hpp>


void print_byte(ostream & os, unsigned char c)
{
  for(int b = 7;b>=0;b--) os << ((c >> b) & 0x1);
}

size_t count_multibyte_symbols(const std::string & s)
{
  size_t count=0;
  for(size_t i=0;i<s.size();++i)
  {
    if( ((s[i] >> 6) & 0b11) == 0b10  ) 
    {
      count++;
    }
  }
  return count;
}

size_t utf_size(const std::string  & s)
{
  return s.size()-count_multibyte_symbols(s);
}

std::string adjast_utf_string(std::string  s, int width=0)
{
  int count=0;
  for(size_t i=0;i<s.size();++i)
  {
    if( ((s[i] >> 6) & 0b11) == 0b10  ) 
    {
      count++;
    }
  }
  width-=s.size();
  if(width<0) width=0;
  return s+std::string(width+count,' ');
}

int main(int argc, char ** argv)
{
  namespace po=boost::program_options;
  po::options_description opt_desc("Allowed options");
  std::string tree_name;
  std::vector<std::string> tree_files;
  unsigned long long N;
  opt_desc.add_options()
    ("help,h","Print this help")
    ("nogamma,g", "Reduce radiative gamma")
    ("reduce,r", "Merge topology and antitopology")
    ("number,N",po::value<unsigned long long>(&N)->default_value(0),"Number of event to proceed")
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
  McTopoAdaptor mctop(tree_files[0]);
  std::cout << "Loading files:"<< std::endl;
  for(int i=1;i<tree_files.size(); i++)
  {
    std::cout << "Adding file: " << tree_files[i] << std::endl;
    mctop.AddFile(tree_files[i].c_str());
  }
  int count_option=NONE;
  if(opt.count("nogamma")) count_option |=  REDUCE_PHOTON;
  if(opt.count("reduce"))  count_option |=  REDUCE;
  auto TopoMap = mctop.Count(N,count_option);
  size_t final_state_width=0;
  size_t topology_info_width=0;
  std::multimap<Long64_t, decay_topology_t> CountMap;
  for(auto & t : TopoMap)
  {
    CountMap.insert(std::pair<Long64_t, decay_topology_t>(t.second,t.first));
    auto & top = t.first;
  //calculate maximum width of the output
    final_state_width    = std::max(final_state_width, final_state(top).size());
    topology_info_width  = std::max(topology_info_width, to_string(top).size());
    //create count map
  }
  std::string fs = "%-"+std::to_string(final_state_width)+"s %-"+std::to_string(topology_info_width)+"s";
  std::string fh = "%3s%8s  "+fs + "%-10s";
  std::string fd = "%3d%8d  "+fs;
  boost::format fmt_head(fh);
  boost::format      fmt(fd);
  std::cout << fmt_head % "#" % "count"  % "final state" % "topology" % "hash list" << std::endl;
  Long64_t event_counter =0;
  Long64_t topology_counter=0;
  for(auto & it : CountMap)
  {
    auto & top = it.second;
    std::cout << fmt % topology_counter % it.first % adjast_utf_string(final_state(top),final_state_width) % adjast_utf_string(to_string(top),topology_info_width);
    std::cout << std::hex;
    for(auto hi = begin(top[boost::graph_bundle].hash_list); hi!=end(top[boost::graph_bundle].hash_list); hi++)
    {
      auto tmp_hi = hi;
      std::cout << *hi;
      tmp_hi++;
      if(tmp_hi!=end(top[boost::graph_bundle].hash_list)) std::cout << ',';
    }
    std::cout << std::endl;
    std::cout << std::dec;
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
