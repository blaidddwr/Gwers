#include "unittest.hh"
#include <iostream>
#include <utility>
#include "exception.h"



int UnitTest::_count {0};



UnitTest::Run& UnitTest::add(const string& name, fp in, fp out)
{
   _runs.emplace_back(name,in,out);
   return _runs.back();
}



void UnitTest::execute()
{
   bool pass {true};
   for (auto i = _runs.begin();i!=_runs.end();++i)
   {
      if (!(*i).execute())
      {
         pass = false;
         break;
      }
   }
   if (pass)
   {
      std::cout << _count << " unit test(s) passed.\n";
   }
}



//
//
//
// *==========================================================================*
// | RUN                                                                      |
// *==========================================================================*
//
//
//



bool UnitTest::Run::execute()
{
   bool ret {true};
   std::cout << _name;
   _in();
   for (auto i:_tests)
   {
      _count = 1;
      ++(UnitTest::_count);
      std::cout << ".";
      try
      {
         i.second(*this);
      }
      catch (Gwers::Exception e)
      {
         std::cout << i.first << _count << " FAILED.\n";
         std::cout << "Gwers: " << e.who() << ":" << e.what() << "\n";
         std::cout << "TRACE:\n";
         for (auto i = Gwers::Trace::begin();i!=Gwers::Trace::end();++i)
         {
            std::cout << *i;
            if (++i!=Gwers::Trace::end())
            {
               std::cout << " --->\n";
            }
            else
            {
               std::cout << "\n";
            }
         }
         _out();
         ret = false;
         break;
      }
      catch (std::exception e)
      {
         std::cout << i.first << _count << " FAILED.\n";
         std::cout << "Std: " << e.what() << "\n";
         _out();
         ret = false;
         break;
      }
      catch (...)
      {
         std::cout << i.first << _count << " FAILED.\n";
         _out();
         ret = false;
         break;
      }
   }
   if (ret)
   {
      _out();
      std::cout << "\n";
   }
   return ret;
}



void UnitTest::Run::next()
{
   std::cout << ".";
   ++_count;
   ++(UnitTest::_count);
}
