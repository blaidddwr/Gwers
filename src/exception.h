#ifndef GWERS_EXCEPTION_HH
#define GWERS_EXCEPTION_HH
#include <string>
#include "trace.h"
#ifdef DEBUG
#define GWX_DECLARE(N) static inline const char* GWX__get__who() { return #N; }
#define GWX_EXCEPTION(X) struct X : public ::Gwers::Exception\
                         {\
                            X(int l):\
                               ::Gwers::Exception(GWX__get__who(),#X,l)\
                            {}\
                         };
#define GWX_ASSERT(T,X,L) ::Gwers::Exception::assert<X>(T,L);
#define GWX_CHECK(T,X,L) ::Gwers::Exception::assert<X>(T,L);
#define GWX_PASS(V,C,F,X,L) ::Gwers::Exception::assert<X>(V C F,L);
#define GWX_TRY(S,X,L) try { S; } catch(...) { throw X(L); }
#else
#define GWX_DECLARE(N)
#define GWX_EXCEPTION(X)
#define GWX_ASSERT(T,X,L)
#define GWX_CHECK(T,X,L) T;
#define GWX_PASS(V,C,F,X,L) F;
#define GWX_TRY(S,X,L) S;
#endif
namespace Gwers {



/// @defgroup exception Exceptions and Tracing
/// @brief Handles optional exception based assertions and stack tracing.
///
/// This collection of classes and functions gives a framework for detecting
/// error states within the code base of the library and by extension code that
/// uses this library. None of the classes should be used directly inside the
/// code base. Instead, macros are defined for all exception and trace code to
/// allow the library to be compiled with no error checked for faster
/// processing. The two flags that can be defined for adding exceptions to the
/// code base are DEBUG and DTRACE.
///
/// DEBUG will enable the use of assertion checks with code. The macros used for
/// assertion checking are GWX_DECLARE, GWX_EXCEPTION, GWX_ASSERT, GWX_CHECK,
/// GWX_PASS, and GWX_TRY.
///
/// GWX_DECLARE(N) must be called once before any GWX_EXCEPTION is defined
/// within a namespace or class. N is the fully qualified name of the namespace
/// or class, including all namespaces and classes it is nested within. If DEBUG
/// is not defined then all GWX_DECLARE macros resolve to an empty string.
///
/// GWX_EXCEPTION(X) defines a new exception within the scope it is defined. It
/// is recommended to define all exceptions within the individual classes of
/// your code. X is the name of the new exception to be defined. If DEBUG is not
/// defined then all GWX_EXCEPTION macros resolve to an empty line.
///
/// GWX_ASSERT(T,X,L) tests a condition, throwing an exception if that condition
/// is false. The boolean condition to test is T. The exception type that will
/// be thrown is X. The line number where this exception was thrown is L, which
/// should always be passed with %__LINE__. If DEBUG is not defined then all
/// GWX_ASSERT macros resolve to an empty line and do NOT execute the
/// conditional T.
///
/// @warning For GWX_ASSERT(T,X,L), if the conditional T has a function call
/// within it, then disabling DEBUG will make it so that function is NOT called!
///
/// GWX_CHECK(T,X,L) works almost identical to X_ASSERT(T,X,L) with one
/// important difference. If DEBUG is not defined then the conditional T is
/// still executed but never evaluated. So if you pass a function or critical
/// part of code that needs to be executed no matter what then you must use
/// GWX_CHECK.
///
/// GWX_PASS(V,C,F,X,L) tests the return value of a function, throwing an
/// exception if the comparison fails. Like GWX_CHECK and GWX_ASSERT, the
/// exception type that will be thrown is X and L is the line number where it is
/// thrown. F is the function to be called, C is the comparison to use, and V is
/// the value compared to the function's return value. The order of comparison
/// is V first and F last. For example, using >= for the C value equates to V
/// being greater than or equal to the return value of the function. If DEBUG is
/// not defined then the function F will still be executed but with no
/// comparison or possibility of throwing an exception.
///
/// GWX_TRY(S,X,L) will catch any exception thrown by S, throwing a new
/// exception of type X. S will almost always be a system function that has the
/// potential of throwing an exception that is not of type Gwers::Exception. If
/// DEBUG is not defined then the statement S will be called but no exceptions
/// will be caught if they are thrown by S.
///
/// DTRACE will enable stack tracing that will give you a snapshot of the
/// function call stack if an exception is thrown. While you can enable DTRACE
/// without DEBUG, there is no reason to do so. Therefore, if you define DTRACE
/// you always want to have DEBUG defined also. Using DTRACE is much simpler, at
/// the beginning of each function of any type call the GWX_BEGIN(F,...) macro,
/// where F is a string defining the full function name, including the scope
/// resolution and all arguments, and the variable list of argument after F is
/// the list of all arguments given to the function, if any. By providing the
/// values of all function arguments the stack trace information will provide a
/// very rich depth of information. It is recommended to use
/// %__PRETTY_FUNCTION__ for the F argument of GWX_BEGIN. If DTRACE is not
/// defined then all X_BEGIN macros resolve to an empty line.
///
/// If an exception is caught, DTRACE is enabled, and you wish to examine the
/// function stack, then use the Trace::begin() and Trace::end() functions to
/// iterate through the stack list which consists of strings with values of the
/// full function name with arguments for each stack item.
///
/// The only function that should be used in the Exception class is
/// Exception::base_catch(), which is used for setting up the root of where all
/// exceptions are caught along with telling the Trace class where the root of
/// the stack trace begins.



/// @ingroup exception
/// @brief Represents a single exception object that has been thrown.
///
/// This holds information about a single exception that has been thrown. It
/// holds information about who, what, and the line number. When an object of
/// this type is constructed it also locks the Trace stack if DTRACE is defined.
/// This also contains static functions that handle catching or throwing these
/// exception objects. The base_catch() function should be used where you desire
/// the root of your function tracing to begin, very similar to the main
/// function in regular code.
///
/// @warning Exceptions are not designed to pass from one thread to another, so
/// there should be a base_catch call for each separate thread that exists.
///
/// @warning All functions that use the X_BEGIN macro should be nested within
/// the base_catch function. If any function within the code base has this macro
/// that is below the base_catch function in the stack then it will cause
/// undefined behavior.
///
/// @warning The static assert function within this class or the class
/// constructor should never be used directly by the user. It is used by the
/// X_ASSERT, X_CHECK, and X_PASS macros defined for the user to use, not the
/// static function itself.
class Exception
{
public:
   // *
   // * ENUMERATIONS
   // *
   /// @brief Defines possible exception types.
   ///
   /// This defines the possible exceptions that can be caught within the
   /// base_catch function.
   enum class Type
   {
      gwers, ///< %Gwers %Exception has been caught.
      std, ///< Standard library exception has been caught.
      unknown ///< Unknown exception has been caught.
   };
   // *
   // * DECLERATIONS
   // *
   /// @brief Used for all strings.
   using string = std::string;
   /// @brief Used for pass-through function of base_catch.
   using fp = void (*)(void);
   /// @brief Used for exception handling function of base_catch.
   ///
   /// @param t Type of exception that was caught by base_catch().
   /// @param e If the exception type is Type::gwers then this is a pointer to
   /// the exception, else it is nullptr.
   /// @param std If the exception type is Type::std then this is a pointer to
   /// the std exception, else it is nullptr.
   ///
   /// This is the function pointer given to base_catch() and will be used as
   /// the exception handler. It will be called by base_catch() if an exception
   /// is thrown and then caught in said function.
   using efp = void (*)(Type t, Exception* e, std::exception* std);
   // *
   // * BASIC METHODS
   // *
   /// @brief Initializes exception object.
   ///
   /// @param who Scope where this exception is thrown, either its namespace or
   /// class.
   /// @param what The specific type of exception that is thrown.
   /// @param line The line of code where this exception is being thrown.
   ///
   /// Initializes this exception object, setting all internal values to
   /// arguments given.
   ///
   /// @warning This constructor should never be called by the user, instead
   /// using the macros supplied for error checking.
   Exception(const string& who, const string& what, int line);
   // *
   // * FUNCTIONS
   // *
   /// @brief Get scope of exception.
   const string& who() const;
   /// @brief Get exception type.
   const string& what() const;
   /// @brief Get line number where exception was thrown.
   int line() const;
   // *
   // * STATIC FUNCTIONS
   // *
   /// @brief Make assertion that will throw exception if not passed.
   ///
   /// @tparam X %Exception type that will be thrown if condition fails.
   ///
   /// @param cond Condition to be tested, if true no exception is thrown, else
   /// if false exception of type given will be thrown.
   /// @param line Line number where assertion is being declared.
   ///
   /// Tests condition given to it, throwing an exception of the type given if
   /// and only if the condition is false.
   ///
   /// @warning This function should never be called by the user, instead using
   /// the macros supplied for error checking.
   template<class X> static void assert(bool cond, int line);
   /// @brief Base of function stack that will catch any exception.
   ///
   /// @param base Function that will be called immediately after calling this
   /// function, think of it like the function main().
   /// @param handler Function that will be called if any exception is caught by
   /// this function.
   ///
   /// This is used as a base of the function stack that will catch any
   /// exception thrown. Once this function catches an exception, it will return
   /// and not call the base function again. This means that any thrown
   /// exception will cease normal execution of the program running from the
   /// base function. See Exception::efp for more information about what is
   /// passed to the exception handling function.
   static void base_catch(fp base, efp handler);
private:
   // *
   // * VARIABLES
   // *
   string _who;
   string _what;
   int _line;
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



inline Exception::Exception(const string& who, const string& what, int line):
   _who {who},
   _what {what},
   _line {line}
{
   Trace::lock();
}



inline const Exception::string& Exception::who() const
{
   return _who;
}



inline const Exception::string& Exception::what() const
{
   return _what;
}



inline int Exception::line() const
{
   return _line;
}



template<class X> void Exception::assert(bool cond, int line)
{
   if (!cond)
   {
      throw X(line);
   }
}



}
#endif
