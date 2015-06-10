#ifndef IBN_LIB_MCTOPO_H
#define IBN_LIB_MCTOPO_H

#include "McTopo.h"
#include <TObject.h>
//unsigned long hash(McTopo *);
unsigned long hash(McTopo &);

class Hash : public TObject
{
  McTopo * mctopo;
  public:
  Hash(McTopo * m);
  ClassDef(Hash,1)
};

#endif

