#include "exception.h"
namespace Gwers {



void Exception::base_catch(fp base, efp handler)
{
   Trace::flush();
   try
   {
      base();
   }
   catch (Exception e)
   {
      handler(Type::gwers,&e,nullptr);
   }
   catch (std::exception e)
   {
      handler(Type::std,nullptr,&e);
   }
   catch (...)
   {
      handler(Type::unknown,nullptr,nullptr);
   }
}



}
