#include "unit.hh"
#include "trace.h"
#include <iostream>
namespace unit {
/// @ingroup utest
/// @brief Tests stack tracing system.
///
/// Tests the stack tracing system, consisting of the Trace class.
namespace trace {




/// @brief Unit tests constructor and destructor.
///
/// This function unit tests the constructor and destructor of the Gwers::Trace
/// class. It makes sure the classes' constructor correctly adds a function item
/// to its static stack, along with making sure the destructor correctly removes
/// function items from its static stack. It performs these tests with two unit
/// tests.
///
/// -# Constructs a single Trace object with a function string, then checks the
/// stack to make sure the function string matches what was given to the
/// constructor.
///
/// -# Calls a nested series of Trace objects, exiting all the nests, thereby
/// calling all destructors and making sure the function stack of trace is
/// empty as a result.
void basic(UnitTest::Run& ut)
{
   using string = std::string;
   using fail = UnitTest::Run::Fail;
   using tr = Gwers::Trace;
   {
      tr t("test_function()");
      auto i = tr::begin();
      if (*i!=string("test_function()"))
      {
         throw fail();
      }
   }
   ut.next();
   {
      {
         tr t("first");
         {
            tr t("second");
            {
               tr t("third");
            }
         }
      }
      if (tr::begin()!=tr::end())
      {
         throw fail();
      }
   }
}



/// @brief Unit tests static lock function.
///
/// This function unit tests the static Gwers::Trace::lock() function. It makes
/// sure once this function is called, Trace destructors do not pop function
/// items on the static stack of function items. It performs these tests with a
/// single unit test.
///
/// -# Calls a nested series of Trace objects, then calls the lock function
/// within the nested series of objects. After all nested Trace objects are out
/// of scope and their destructors have been called the static stack if verified
/// to not be empty.
void lock(UnitTest::Run&)
{
   using fail = UnitTest::Run::Fail;
   using tr = Gwers::Trace;
   {
      tr t("first");
      {
         tr t("second");
         {
            tr t("third");
            tr::lock();
         }
      }
   }
   int count = 0;
   for (auto i = tr::begin();i!=tr::end();++i)
   {
      ++count;
   }
   if (count!=3)
   {
      throw fail();
   }
}



/// @brief Unit tests static flush function.
///
/// This function unit tests the static Gwers::Trace::flush() function. It makes
/// sure that the entire static stack of function strings is cleared out and the
/// stack is empty afterwords. It performs this test with a single unit test.
///
/// -# Adds three function strings to the static stack by instantiating three
/// Trace objects, calling the lock function so they remain on the stack. The
/// flush function is then called and the static stack is verified to the empty
/// afterwords.
void flush(UnitTest::Run&)
{
   using fail = UnitTest::Run::Fail;
   using tr = Gwers::Trace;
   {
      tr t("first");
      {
         tr t("second");
         {
            tr t("third");
            tr::lock();
         }
      }
   }
   tr::flush();
   if (tr::begin()!=tr::end())
   {
      throw fail();
   }
}



/// @brief Additional unit tests for entire class.
///
/// This function makes additional unit tests to the overall Gwers::Trace class,
/// verifying it functions as it should. It makes a stack of four function
/// items, then iterates through the entire list verifying each item matches the
/// string given to each Trace constructor. It then pops two items off of the
/// stack, and again verifies the strings of the two items remaining on the
/// stack. Lastly, it pops the last two items from the stack, verifying that the
/// stack is empty. It performs these unit tests with three unit tests.
///
/// -# Add four Trace objects, iterating through the stack and verifying the
/// strings of all four function items.
///
/// -# Remove two Trace objects from the previous test, verifying there are only
/// two items remaining on the stack and they are the items that should still
/// exist.
///
/// -# Remove the last two Trace objects, verifying the stack is then empty.
void extra(UnitTest::Run& ut)
{
   using string = std::string;
   using fail = UnitTest::Run::Fail;
   using tr = Gwers::Trace;
   tr* t1 = new tr("first");
   tr* t2 = new tr("second");
   tr* t3 = new tr("third");
   tr* t4 = new tr("fourth");
   auto i = tr::begin();
   if (*(i++)!=string("first")||*(i++)!=string("second")||
       *(i++)!=string("third")||*(i++)!=string("fourth")||i!=tr::end())
   {
      throw fail();
   }
   ut.next();
   delete t4;
   delete t3;
   i = tr::begin();
   if (*(i++)!=string("first")||*(i++)!=string("second")||i!=tr::end())
   {
      throw fail();
   }
   ut.next();
   delete t2;
   delete t1;
   if (tr::begin()!=tr::end())
   {
      throw fail();
   }
}



/// @brief Internal function that does nothing.
void none() {}



/// @brief Initialize all unit tests for Trace class.
void init(UnitTest& ut)
{
   UnitTest::Run& t = ut.add("Trace",none,none);
   t.add("basic",basic);
   t.add("lock",lock);
   t.add("flush",flush);
   t.add("extra",extra);
}



}
}
