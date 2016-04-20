#ifndef UNITTEST_HH
#define UNITTEST_HH
#include <string>
#include <vector>



/// @defgroup utest Unit Testing
/// @brief Framework where all unit testing and supporting classes reside.
///
/// This encompasses all unit tests, which are provided in unit testing
/// functions, and the unit testing support class UnitTest. Unit testing is
/// divided into each of its own namespaces based off the name of the class or
/// hierarchy being tested. Those namespaces are encompassed into another
/// namespace called unit.
///
/// All unit testing is done through a single instance of the UnitTest class,
/// adding additional unit tests to it and then executing all tests that were
/// added. Every unit testing namespace will have a function called
/// init(UnitTest&) which will add all unit tests for that namespace to the
/// UnitTest object for execution. All output for unit testing is printed to
/// standard output.



/// @ingroup utest
/// @brief Stores a list of UnitTest::Run objects that will perform unit
/// testing.
///
/// This stores a list of UnitTest::Run objects that will perform unit testing.
/// The objects stored must coincide with the namespaces of the separate unit
/// tests. The name given to the add function must be the namespace. Objects
/// can only be added, not removed. Once all unit test objects are added, they
/// can be executed. All unit tests will be ran and pass or a unit test will be
/// encountered that fails. If a unit test fails any remaining objects will not
/// be tested.
class UnitTest
{
public:
   // *
   // * DECLERATIONS
   // *
   class Run;
   /// @brief Used for all strings.
   using string = std::string;
   /// @brief Used for adding test runs.
   using fp = void (*)(void);
   // *
   // * BASIC METHODS
   // *
   UnitTest() = default;
   // *
   // * COPY METHODS
   // *
   UnitTest(const UnitTest&) = delete;
   UnitTest& operator=(const UnitTest&) = delete;
   // *
   // * MOVE METHODS
   // *
   UnitTest(UnitTest&&) = delete;
   UnitTest& operator=(UnitTest&&) = delete;
   // *
   // * FUNCTIONS
   // *
   /// @brief Creates a new UnitTest::Run object and returns a reference.
   ///
   /// @param name The name for this unit test object that is the namespace of
   /// the collected unit tests.
   /// @param in Function that will be called before the unit test functions are
   /// called. Should be used to initialize any data the unit tests depend on.
   /// @param out Function that will be called once all unit tests are complete
   /// or one of the unit tests failed.
   ///
   /// This creates a new UnitTest::Run object and adds it to the internal list
   /// of this object.
   ///
   /// @return The new UnitTest::Run object that was just created.
   Run& add(const string& name, fp in, fp out);
   /// @brief Executes all stored UnitTest::Run objects.
   ///
   /// Iterates through this object's list of all UnitTest::Run objects that
   /// were added and calls their execute function. If an execute call returns
   /// false to signal a failure, iteration will not continue and this function
   /// will return immediately.
   void execute();
private:
   // *
   // * DECLERATIONS
   // *
   using list = std::vector<Run>;
   // *
   // * VARIABLES
   // *
   list _runs;
   static int _count;
};



//
//
//
// *==========================================================================*
// | RUN                                                                      |
// *==========================================================================*
//
//
//



/// @brief Stores a list of function pointers that will perform unit testing.
///
/// This stores a list of name and function pointer pairs. The function pointers
/// will perform unit testing, throwing any exception if it fails else returning
/// with no exception if it passes. The list can only be added to, not removed
/// from. Once all unit test functions are added, execution of all unit tests
/// can begin. If a unit test fails, iteration through the list will stop and
/// any remaining unit test functions will not be called.
///
/// There is one last special function, next, which is to be called only inside
/// unit testing functions if there is more than one test being performed in the
/// function. For each subsequent test that is performed, call the next function
/// before the beginning of the next test. A number, starting with 0, will be
/// appended to the name of the unit test as output if it fails.
///
/// @warning The execution of these objects are not meant to be called directly,
/// it is called through the main UnitTest object's execution task.
class UnitTest::Run
{
public:
   // *
   // * DECLERATIONS
   // *
   struct Fail {};
   /// @brief Used for all strings.
   using string = UnitTest::string;
   /// @brief Used for in and out functions.
   using fp = UnitTest::fp;
   /// @brief Used for unit test functions.
   using tfp = void(*)(UnitTest::Run&);
   // *
   // * BASIC METHODS
   // *
   /// @brief Initializes object with empty unit testing list.
   ///
   /// @param name The namespace for all unit tests added to this object.
   /// @param in Pointer to function that will be called before unit tests are
   /// executed.
   /// @param out Pointer to function that will be called at end of all unit
   /// tests or when a unit test fails.
   ///
   /// Initializes this object with empty unit testing list and setting the in
   /// and out functions.
   Run(const string& name, fp in, fp out);
   // *
   // * COPY METHODS
   // *
   Run(const Run&) = delete;
   Run& operator=(const Run&) = delete;
   // *
   // * MOVE METHODS
   // *
   /// @brief Default move constructor.
   Run(Run&&) = default;
   /// @brief Default move operator.
   Run& operator=(Run&&) = default;
   // *
   // * FUNCTIONS
   // *
   /// @brief Add a new unit test function.
   ///
   /// @param name Name for specific unit test.
   /// @param test Pointer to unit testing function.
   ///
   /// Add a new unit test function for this object with the given name.
   void add(const string& name, tfp test);
   /// @brief Run list of all unit tests.
   ///
   /// Perform all unit tests in this object's list of functions, only stopping
   /// prematurely if a failure has occurred. Unit testing functions signal a
   /// failure by throwing an exception, else returning with no exception if
   /// unit testing passed.
   ///
   /// @warning This function should not be directly called, instead the
   /// execution task should be called from the main UnitTest object which will
   /// in turn call the execute task for all of this class's objects stored in
   /// the UnitTest object.
   ///
   /// @return True if all unit tests for this object pass without error, else
   /// false if a unit test has failed.
   bool execute();
   /// @brief Signals another unit test is being done.
   ///
   /// Increments internal number signaling another unit test is being done in
   /// the current unit test function that has been called.
   ///
   /// @warning This function should only be called inside a unit testing
   /// function while it is being ran. using this function outside of this scope
   /// will have undefined behavior.
   void next();
private:
   // *
   // * DECLERATIONS
   // *
   using list = std::vector<std::pair<string,tfp>>;
   // *
   // * VARIABLES
   // *
   string _name;
   fp _in;
   fp _out;
   list _tests;
   int _count {0};
};



//
//
//
// *==========================================================================*
// | RUN INLINE/TEMPLATE                                                      |
// *==========================================================================*
//
//
//



inline UnitTest::Run::Run(const string& name, fp in, fp out):
   _name {name},
   _in {in},
   _out {out}
{}



inline void UnitTest::Run::add(const string& name, tfp test)
{
   _tests.emplace_back(name,test);
}



#endif
