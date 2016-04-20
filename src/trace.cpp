#include "trace.h"
namespace Gwers {



thread_local Trace::list Trace::_stack {};
thread_local bool Trace::_lock {false};



Trace::~Trace()
{
   if (!_lock)
   {
      _stack.pop_back();
   }
}



void Trace::flush()
{
   _stack.clear();
   _lock = false;
}



}
