#include "unit.hh"
#include "exception.h"
namespace unit {
/// @ingroup utest
/// @brief Tests exception handling system.
///
/// Tests the exception handling system, consisting of the Exception class.
namespace exception {



/// @brief Used for all strings.
using string = std::string;
/// @brief Used for throwing a unit test failure.
using fail = UnitTest::Run::Fail;
/// @brief Used for calling %Gwers exceptions.
using gwe = Gwers::Exception;
/// @brief Used as shorthand.
using gwtr = Gwers::Trace;



/// @brief Unit tests constructor and get functions.
///
/// This function unit tests the single constructor and get functions of the
/// Gwers::Exception class. It also makes sure the classes' constructor
/// correctly locks the Gwers::Trace function stack for inspection after an
/// exception is caught. It performs these tests with a single unit test.
///
/// -# Constructs an exception object by throwing it, catching it, and then
/// making sure the who(), what(), and line() functions return what was passed
/// to the constructor of the object. Before the object is thrown, a
/// Gwers::Trace object is also created. When the exception is caught, the
/// Gwers::Trace function stack is also checked, confirming that the name of the
/// single Trace object is correctly on its static function stack.
void basic(UnitTest::Run&)
{
   try
   {
      gwtr t("TestFunction");
      throw gwe("test_who","test_what",33);
   }
   catch (gwe t)
   {
      if (t.line()!=33||t.who()!=string("test_who")||
          t.what()!=string("test_what")||
          *(gwtr::begin())!=string("TestFunction"))
      {
         throw fail();
      }
   }
   gwtr::flush();
}



/// @brief Unit tests static assert function.
///
/// This function unit tests the static Gwers::Exception::assert() function. It
/// makes sure an exception is thrown when it should and also when it should
/// not. It also checks that the line number is correctly passed to the
/// exception if one is thrown. It performs these tests with two unit tests.
///
/// -# Call assert with a fake exception structure, giving it a false
/// condition and a specific line number, making sure the assert function does
/// throw the exception and the line number of the thrown exception matches what
/// was given to the assert function.
///
/// -# Call assert with a fake exception structure, giving it a true condition
/// and making sure it does not throw an exception.
void assert(UnitTest::Run& ut)
{
   struct Fake : public Gwers::Exception
   {
      Fake(int l): Exception("unit::exception","fake",l) {}
   };
   bool test {false};
   try
   {
      gwe::assert<Fake>(false,66);
   }
   catch (Fake e)
   {
      if (e.line()==66)
      {
         test = true;
      }
   }
   if (!test)
   {
      throw fail();
   }
   ut.next();
   test = true;
   try
   {
      gwe::assert<Fake>(true,66);
   }
   catch (Fake)
   {
      test = false;
   }
   if (!test)
   {
      throw fail();
   }
}



/// @brief Internal variable that is used with base_catch() unit testing.
bool base_touch;
/// @brief Internal variable that is used with base_catch() unit testing.
int handler_touch;



/// @brief Internal function that is used with base_catch() unit testing.
void base_func()
{
   base_touch = true;
   throw gwe("test_who","test_what",66);
}



/// @brief Internal function that is used with base_catch() unit testing.
void base_func_std()
{
   base_touch = true;
   std::vector<int> t;
   t.at(10000000000000) = 66;
}



/// @brief Internal function that is used with base_catch() unit testing.
void base_func_unknown()
{
   base_touch = true;
   throw int(66);
}



/// @brief Internal function that is used with base_catch() unit testing.
void handler_func(gwe::Type t, gwe* e, std::exception* std)
{
   switch (t)
   {
   case gwe::Type::gwers:
      if (e==nullptr||std!=nullptr||e->who()!=string("test_who")||
          e->what()!=string("test_what")||e->line()!=66)
      {
         throw fail();
      }
      handler_touch = 1;
      break;
   case gwe::Type::std:
      if (e!=nullptr||std==nullptr)
      {
         throw fail();
      }
      handler_touch = 2;
      break;
   case gwe::Type::unknown:
      if (e!=nullptr||std!=nullptr)
      {
         throw fail();
      }
      handler_touch = 3;
      break;
   }
}



/// @brief Unit tests static base_catch function.
///
/// This function unit tests the static Gwers::Exception::base_catch() function.
/// It makes sure the pass through base function is correctly called, along with
/// making sure all three possible exceptions are correctly caught and given to
/// the exception handling function correctly. It performs these tests with
/// three unit tests.
///
/// -# Call the base_catch function with a base function that will throw a
/// Gwers::Exception, making sure the base function was actually called and the
/// appropriate arguments were given to the exception handling function.
///
/// -# Call the base_catch function with a base function that will throw a
/// std::exception, making sure the appropriate arguments were given to the
/// exception handling function.
///
/// -# Call the base_catch function with a base function that will throw an
/// unknown(integer) exception, making sure the appropriate arguments were given
/// to the exception handling function.
void base_catch(UnitTest::Run& ut)
{
   base_touch = false;
   handler_touch = 0;
   gwe::base_catch(base_func,handler_func);
   if (!base_touch||handler_touch!=1)
   {
      throw fail();
   }
   ut.next();
   base_touch = false;
   handler_touch = 0;
   gwe::base_catch(base_func_std,handler_func);
   if (!base_touch||handler_touch!=2)
   {
      throw fail();
   }
   ut.next();
   base_touch = false;
   handler_touch = 0;
   gwe::base_catch(base_func_unknown,handler_func);
   if (!base_touch||handler_touch!=3)
   {
      throw fail();
   }
}



/// @brief Initialize all unit tests for Exception class.
void init(UnitTest& ut)
{
   UnitTest::Run& t = ut.add("Exception",nullptr,nullptr);
   t.add("basic",basic);
   t.add("assert",assert);
   t.add("base_catch",base_catch);
}



}
}
