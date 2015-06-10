#include "libMcTopo.h"

#include "McTopoAdaptor.h"
unsigned long hash(McTopo * mct)
{
  McTopoAdaptor ma(*mct);
  auto topology = ma.MakeDecayTopology();
  return hash(topology);
}

