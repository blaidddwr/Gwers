#ifndef GWERS_TRACE_H
#define GWERS_TRACE_H
#include <string>
#include <vector>
#include <sstream>
#ifdef DTRACE
#define GWX_BEGIN(F,...) std::ostringstream GWX__tmp__string;\
                         GWX__tmp__string << F;\
                         ::Gwers::Trace::build(GWX__tmp__string,##__VA_ARGS__);\
                         ::Gwers::Trace x_trace(GWX__tmp__string.str());
#else
#define GWX_BEGIN(F,...)
#endif
namespace Gwers {



/// @ingroup exception
/// @brief Keeps track of the function stack for each individual thread.
///
/// Framework that allows user to track the function stack of each individual
/// thread within their program. The function stack is tracked by the Trace
/// class making its own static stack which it adds and removes called functions
/// from. The only code required in all functions that will be tracked within
/// the stack is the GWX_BEGIN macro at the beginning of each function. This
/// creates a Trace object, which adds the function to the stack. Because it is
/// an object, once the end of the function is reached the destructor removes
/// the function from the stack. There is a special lock() function which makes
/// it so functions are no longer removed from the stack, required if a stack
/// destroying exception is thrown. This, however, is all done by the macros and
/// the Exception class; the user does not need to use the constructor or most
/// class functions directly.
///
/// @warning Except for using begin() and end() to iterate through the recorded
/// stack, the user should not directly use this class. All the user needs to do
/// is enable DTRACE and add the GWX_BEGIN macro at the beginning of each
/// function to be tracked.
class Trace
{
public:
   // *
   // * DECLERATIONS
   // *
   /// @brief Used for all strings.
   using string = std::string;
   /// @brief Type used for iterating through function stack.
   using iter = std::vector<string>::iterator;
   // *
   // * BASIC METHODS
   // *
   /// @brief Adds new function to stack.
   ///
   /// @param fname Full function name that will be added to stack, including
   /// scope and all arguments.
   ///
   /// This will add a new function item to this classes' static stack.
   ///
   /// @warning This constructor should never be called directly by the user,
   /// instead use the X_BEGIN macro which will use this constructor. Also never
   /// include X_BEGIN to functions that are not nested within a
   /// Exception::base_catch() call.
   Trace(const string& fname);
   /// @brief Pops top function from stack.
   ///
   /// This will remove the top function from this classes' static stack.
   ~Trace();
   // *
   // * STATIC FUNCTIONS
   // *
   /// @brief Prevents the function stack from being popped.
   ///
   /// This locks this classes' static stack, preventing any function item on it
   /// from being popped by the destructor of Trace objects.
   ///
   /// @warning This function should never be called directly by the user. The
   /// locking of the classes' static stack is done for you when an Exception
   /// object is constructed; locking the stack in Trace and preventing any loss
   /// of information.
   static void lock();
   /// @brief Resets the function stack system.
   ///
   /// This unlocks this classes' static stack, allowing for the popping of
   /// function items on the stack. This also resets the same stack, clearing
   /// any function items currently loaded on the stack.
   ///
   /// @warning This function should never be called directly by the user. The
   /// flushing of the classes' static stack is done for you at the beginning of
   /// the Exception::base_catch() function before it passes control to the main
   /// function pointer.
   static void flush();
   /// @brief Get beginning of list iterator for classes' stack.
   static const iter begin();
   /// @brief Get one past end of list iterator for classes' stack.
   static const iter end();
   /// @brief Do nothing, wrapper for other build functions.
   static void build(std::ostringstream&) {}
   /// @brief End of list build function, see main build function for
   /// description.
   template<class T> static void build(std::ostringstream& str, T val);
   /// @brief Builds a variable list of function argument values.
   ///
   /// @tparam T First argument in list of variable function arguments.
   /// @tparam Args List of remaining arguments for provided function.
   ///
   /// @param str Output stream where list of variable arguments will be written
   /// to.
   /// @param val First argument in variable list of function arguments.
   /// @param args Variable list of remaining arguments for provided function.
   ///
   /// Adds a variable list of function argument values to an output string
   /// stream.
   ///
   /// @warning This function should never be called directly by the user. The
   /// GWX_BEGIN macro uses this function to build the function name header with
   /// a variable number of function argument values.
   template<class T, class... Args>
      static void build(std::ostringstream& str,T val, Args... args);
private:
   // *
   // * DECLERATIONS
   // *
   using list = std::vector<string>;
   // *
   // * STATIC VARIABLES
   // *
   thread_local static list _stack;
   thread_local static bool _lock;
};



//
//
//
// *==========================================================================*
// | INLINE/TEMPLATE                                                          |
// *==========================================================================*
//
//
//



inline Trace::Trace(const string& fname)
{
   _stack.emplace_back(fname);
}



inline void Trace::lock()
{
   _lock = true;
}



inline const Trace::iter Trace::begin()
{
   return _stack.begin();
}



inline const Trace::iter Trace::end()
{
   return _stack.end();
}



template<class T> inline void Trace::build(std::ostringstream& str, T val)
{
   str << "[" << val << "]";
}



template<class T, class... Args>
   void Trace::build(std::ostringstream& str, T val, Args... args)
{
   str << "[" << val << "],";
   build(str,args...);
}



}
#endif
