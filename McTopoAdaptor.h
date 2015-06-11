#ifndef IBN_MCTOPO_ADAPTOR_H
#define IBN_MCTOPO_ADAPTOR_H

#include "Options.h"
#include "decay_topology.h"

#include <string>

#include "McTopo.h"

#include <TChain.h>
struct McTopoAdaptor : public McTopo
{
  TChain * chain=0;
  McTopoAdaptor(McTopo &);
  McTopoAdaptor(std::string tree_name);
  void AddFile(std::string  file_name);
  std::map<decay_topology_t, Long64_t> Count(unsigned long long N=0, int opt = Option::NONE);
  decay_topology_t MakeDecayTopology(void);
  virtual void     Loop(){};
};

#endif
