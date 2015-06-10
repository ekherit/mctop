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
  McTopoAdaptor(std::string tree_name);
  void AddFile(std::string  file_name);
  std::map<decay_topology_t, Long64_t> Count(int opt = Option::NONE);
  virtual void     Loop(){};
};

#endif
