#include "unit.hh"



int main()
{
   UnitTest ut;
   unit::trace::init(ut);
   unit::exception::init(ut);
   ut.execute();
   return 0;
}
