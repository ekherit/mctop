#ifndef MyEvent_H
#define MyEvent_H
#include <TObject.h>

#include "McTopo.h"

#include <string>
unsigned long topology_hash(McTopoBase * mctopo);
//const char  * topology_info(McTopoBase * mctopo);
std::string topology_info(McTopoBase * mctopo);

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
