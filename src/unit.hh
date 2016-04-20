#ifndef UNIT_HH
#define UNIT_HH
#include "unittest.hh"


/// @brief Unit testing for entire code base.
///
/// This encompasses all unit testing code which is not part of the library
/// itself. All unit testing code for the entire code base is part of this name
/// space.
namespace unit {
namespace exception { void init(UnitTest&); }
namespace trace { void init(UnitTest&); }
}



#endif
