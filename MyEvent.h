#ifndef MyEvent_H
#define MyEvent_H
#include <TObject.h>

#include "McTopo.h"

//unsigned long hash(McTopo * mctop);
unsigned long test_hash(int i);
double test_hash(double i);
unsigned long test_hash(McTopo * mctopo);
unsigned long test_hash2(McTopo * mctopo);
const char * info(McTopo * mctopo);

class MyEvent : public TObject
{
  McTopo * mctopo;
  public:
    MyEvent();
    MyEvent(McTopo * m);
    unsigned long hash(void);
    ClassDef(MyEvent,1);
}; 
#endif
