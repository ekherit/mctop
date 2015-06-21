#ifndef MyEvent_H
#define MyEvent_H
#include <TObject.h>

#include "McTopo.h"
#include "McTopoAdaptor.h"

unsigned long topology_hash(McTopo * mctopo);
const char  * topology_info(McTopo * mctopo);

//class MyEvent : public TObject
//{
//  McTopo * mctopo;
//  public:
//    MyEvent();
//    MyEvent(McTopo * m);
//    unsigned long hash(void);
//    ClassDef(MyEvent,1);
//}; 
#endif
