#include "libMcTopo.h"
#include "McTopoAdaptor.h"

ClassImp(Hash)

Hash::Hash(McTopo * mct)
{
  mctopo = mct;
}

unsigned long hash(McTopo * mct)
{
  McTopoAdaptor ma(*mct);
  auto topology = ma.MakeDecayTopology();
  return hash(topology);
}

