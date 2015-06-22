#ifndef IBN_MCTOPO_ADAPTOR_H
#define IBN_MCTOPO_ADAPTOR_H

#include "Options.h"
#include "decay_topology.h"

#include <string>

#include "McTopoBase.h"

#include <TChain.h>

//decay_topology_t make_topology(const McTopoBase & );

struct McTopo : public McTopoBase
{
  TChain * chain=0;
	McTopo(TTree * tree =0);
  McTopo(McTopoBase &);
  McTopo(std::string tree_name);
  void AddFile(std::string  file_name);
  std::map<decay_topology_t, Long64_t> Count(unsigned long long N=0, int opt = Option::NONE);
  decay_topology_t MakeDecayTopology(void) const;
	unsigned long hash(void) const;
	std::string info(void) const;
  virtual void     Loop(){};
};

#endif
