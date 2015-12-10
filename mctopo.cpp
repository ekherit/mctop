#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <stdexcept>
#include <iomanip>

#include <boost/format.hpp>
#include <boost/program_options.hpp>

#include "pdg_table.h"

//#include "McTopo.h"
#include "RootMCTopo.h"
#include "mctopo.h"


#include "decay_topology.h"
#include "Options.h"

void print_byte(std::ostream & os, unsigned char c)
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

std::string format(std::string s, int width=0)
{
  size_t us = utf_size(s);
  size_t n = width > us ? width -us : 0;
  return s+std::string(n,' ');
}

//inline decay_topology_t make_topology(const RootMCTopo  & m)
//{
//  decay_topology_t top;
//  //fill the topology
//  for(int i=0;i<m.indexmc; i++)
//  {
//    for(int j=0;j<m.indexmc;j++)
//    {
//      if(m.idx[i] == m.motheridx[j])
//      {
//        vertex_t parent(i);
//        vertex_t child(j);
//        boost::add_edge(parent,child,top);
//        top[parent].name = PdgTable[top[parent].pdgid];
//        top[parent].pdgid = m.pdgid[parent];
//        top[child].pdgid = m.pdgid[child];
//        top[child].name = PdgTable[top[child].pdgid];
//      }
//    }
//  }
//  add_hash(top);
//  return top;
//}

std::map<decay_topology_t, Long64_t> count_topologies(RootMCTopo * mctopo , unsigned long long  N, int opt)
{
  if (mctopo->fChain == 0) throw std::runtime_error("No chain");
  Long64_t nentries = mctopo->fChain->GetEntriesFast();
  if(N>0) nentries = N;
  std::map<decay_topology_t, Long64_t> TopoMap;
  Long64_t nbytes = 0, nb = 0;
  Long64_t event_counter=0;
  for (Long64_t jentry=0; jentry<nentries;jentry++,event_counter++)
  {
    Long64_t ientry = mctopo->LoadTree(jentry);
    if (ientry < 0) break;
    nb = mctopo->fChain->GetEntry(jentry);   nbytes += nb;
    decay_topology_t top = make_topology(*mctopo);
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
    ("tree_files", po::value<std::vector<std::string>>(&tree_files), "List of files")
    ;
  po::positional_options_description pos;
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
  //McTopo mctop(tree_files[0]);
  TChain * chain = new TChain(tree_files[0].c_str(), tree_files[0].c_str());
  for(int i=1;i<tree_files.size(); i++)
  {
    std::cout << tree_files[i];
    chain->AddFile(tree_files[i].c_str());
    if(i+1 < tree_files.size())  std::cout << ',';
  }
  RootMCTopo mctop(chain);
  std::cout << "Loading files:"<< std::endl;
  std::cout << std::endl;
  int count_option=NONE;
  if(opt.count("nogamma")) count_option |=  REDUCE_PHOTON;
  if(opt.count("reduce"))  count_option |=  REDUCE;
  //auto TopoMap = mctop.Count(N,count_option);
  auto TopoMap = count_topologies(&mctop, N,count_option);
  size_t final_state_width=9;
  size_t topology_info_width=8;
  std::multimap<Long64_t, decay_topology_t> CountMap;
  for(auto & t : TopoMap)
  {
    CountMap.insert(std::pair<Long64_t, decay_topology_t>(t.second,t.first));
  //calculate maximum width of the output
    final_state_width    = std::max(final_state_width,  utf_size(final_state(t.first)));
    topology_info_width  = std::max(topology_info_width,utf_size(to_string(t.first)));
  }
  std::string hstrfmt ="%="+std::to_string(final_state_width)+"s  %="+ std::to_string(topology_info_width)+"s %-10s";
  boost::format fmt_head("%3s%8s "+ hstrfmt);
  boost::format fmt_data("%3d%8d  %-s  %-s ");
  std::cout << fmt_head % "#" % "count"  % "final st." % "topology" % "hash list" << std::endl;
  Long64_t event_counter =0;
  Long64_t topology_counter=0;
  for(auto & it : CountMap)
  {
    auto & top = it.second;
    std::cout << fmt_data % (topology_counter+1) % it.first % format(final_state(top),final_state_width) % format(to_string(top),topology_info_width);
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
};
