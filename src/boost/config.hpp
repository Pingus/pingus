//  Boost config.hpp configuration header file  ------------------------------//

//  (C) Copyright Boost.org 1999. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version.

//  Boost config.hpp policy and rationale documentation has been moved to
//  http://www.boost.org/libs/config

//  Revision History (excluding minor changes for specific compilers)
//   17 Nov 00  BOOST_NO_AUTO_PTR (John Maddock)
//    4 Oct 00  BOOST_NO_STD_MIN_MAX (Jeremy Siek)
//   29 Sep 00  BOOST_NO_INTEGRAL_INT64_T (Jens Maurer)
//   25 Sep 00  BOOST_NO_STD_ALLOCATOR (Jeremy Siek)
//   18 SEP 00  BOOST_NO_SLIST, BOOST_NO_HASH, 
//              BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS
//              BOOST_NO_LIMITS (Jeremy Siek)
//    1 Sep 00  BOOST_NO_PRIVATE_IN_AGGREGATE added. (Mark Rodgers)
//   23 Jul 00  Fixed spelling of BOOST_NO_INCLASS_MEMBER_INITIALIZATION in
//              comment (Dave Abrahams). 
//   10 Jul 00  BOOST_NO_POINTER_TO_MEMBER_CONST added (Mark Rodgers)
//   26 Jun 00  BOOST_NO_STD_ITERATOR, BOOST_MSVC_STD_ITERATOR,
//              BOOST_NO_STD_ITERATOR_TRAITS, BOOST_NO_USING_TEMPLATE,
//              added (Jeremy Siek)
//   20 Jun 00  BOOST_MSVC added (Aleksey Gurtovoy)
//   14 Jun 00  BOOST_NO_DEPENDENT_TYPES_IN_TEMPLATE_VALUE_PARAMETERS (Jens M.)
//   22 Mar 00  BOOST_MSVC6_MEMBER_TEMPLATES added (Dave Abrahams)
//   18 Feb 00  BOOST_NO_INCLASS_MEMBER_INITIALIZATION added (Jens Maurer)
//   26 Jan 00  Borland compiler support added (John Maddock)
//   26 Jan 00  Sun compiler support added (Jörg Schaible)
//   30 Dec 99  BOOST_NMEMBER_TEMPLATES compatibility moved here from
//              smart_ptr.hpp. (Dave Abrahams)
//   15 Nov 99  BOOST_NO_OPERATORS_IN_NAMESPACE,
//              BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION added (Beman Dawes)
//   11 Oct 99  BOOST_NO_STDC_NAMESPACE refined; <cstddef> supplied
//   29 Sep 99  BOOST_NO_STDC_NAMESPACE added (Ed Brey)
//   24 Sep 99  BOOST_DECL added (Ed Brey)
//   10 Aug 99  Endedness flags added, GNU CC support added
//   22 Jul 99  Initial version
 

#ifndef BOOST_CONFIG_HPP
#define BOOST_CONFIG_HPP

//  Conformance Flag Macros  -------------------------------------------------//
//
//  Conformance flag macros should identify the absence of C++ Standard 
//  conformance rather than its presence.  This ensures that standard conforming
//  compilers do not require a lot of configuration flag macros.  It places the
//  burden where it should be, on non-conforming compilers.  In the future,
//  hopefully, less rather than more conformance flags will have to be defined.

//  BOOST_NO_DEPENDENT_TYPES_IN_TEMPLATE_VALUE_PARAMETERS: Template value
//  parameters cannot have a dependent type, for example
//  "template<class T, typename T::type value> class X { ... };"

//  BOOST_NO_INCLASS_MEMBER_INITIALIZATION: Compiler violates std::9.4.2/4. 

//  BOOST_NO_INTEGRAL_INT64_T: int64_t as defined by <boost/cstdint.hpp> is
//  not an integral type.

//  BOOST_NO_MEMBER_TEMPLATES: Member template functions not fully supported.
//  Also see BOOST_MSVC6_MEMBER_TEMPLATES in the Compiler Control section below.

//  BOOST_NO_MEMBER_TEMPLATE_FRIENDS: Member template friend syntax
//  ("template<class P> friend class frd;") described in the C++ Standard,
//  14.5.3, not supported.

//  BOOST_NO_OPERATORS_IN_NAMESPACE: Compiler requires inherited operator
//  friend functions to be defined at namespace scope, then using'ed to boost.
//  Probably GCC specific.  See boost/operators.hpp for example.

//  BOOST_NO_POINTER_TO_MEMBER_CONST: The compiler does not correctly handle
//  pointers to const member functions, preventing use of these in overloaded 
//  function templates.  See boost/functional.hpp for example.

//  BOOST_NO_PRIVATE_IN_AGGREGATE: The compiler misreads 8.5.1, treating classes
//  as non-aggregate if they contain private or protected member functions. 

//  BOOST_NO_STD_ITERATOR: The C++ implementation fails to provide the
//  std::iterator class.

//  BOOST_NO_STD_ITERATOR_TRAITS: The compiler does not provide a standard
//  compliant implementation of std::iterator_traits. Note that
//  the compiler may still have a non-standard implementation.

//  BOOST_NO_STDC_NAMESPACE: The contents of C++ standard headers for C library
//  functions (the <c...> headers) have not been placed in namespace std.
//  Because the use of std::size_t is so common, a specific workaround for
//  <cstddef> (and thus std::size_t) is provided in this header (see below).
//  For other <c...> headers, a workaround must be provided in the boost header:
//
//    #include <cstdlib>  // for abs
//    #ifdef BOOST_NO_STDC_NAMESPACE
//      namespace std { using ::abs; }
//    #endif

//  BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION. Class template partial
//  specialization (14.5.4 [temp.class.spec]) not supported.

//  BOOST_NO_USING_TEMPLATE: The compiler will not accept a using declaration
//  that imports a template from the global namespace into a named namespace.
//  Probably Borland specific.

//  BOOST_NO_AUTO_PTR: If the compiler / library supplies non-standard or broken
//  std::auto_ptr.

//  Compiler Control or Information Macros  ----------------------------------//
//
//  Compilers often supply features outside of the C++ Standard which need to be
//  controlled or detected. As usual, reasonable default behavior should occur
//  if any of these macros are not defined.

//  BOOST_DECL:  Certain compilers for Microsoft operating systems require
//  non-standard class and function decoration if dynamic load library linking
//  is desired.  BOOST_DECL supplies that decoration, defaulting to a nul string
//  so that it is harmless when not required.  Boost does not encourage the use
//  of BOOST_DECL - it is non-standard and to be avoided if practical to do so.

//  BOOST_DECL_EXPORTS:  User defined, BOOST_DECL_EXPORTS causes BOOST_DECL to
//  be defined as __declspec(dllexport) rather than __declspec(dllimport).

//  BOOST_MSVC6_MEMBER_TEMPLATES:  Microsoft Visual C++ 6.0 has enough member
//  template idiosyncrasies (being polite) that BOOST_NO_MEMBER_TEMPLATES is
//  defined for this compiler. BOOST_MSVC6_MEMBER_TEMPLATES is defined to allow
//  compiler specific workarounds.

//  BOOST_MSVC: defined as _MSC_VER for the Microsoft compiler only. In general,
//  boost headers should test for a specific conformance flag macro (for
//  example, BOOST_NO_MEMBER_TEMPLATE_FRIENDS) rather than a specific compiler.
//  VC++ is a special case, however, since many libraries try to support it yet
//  it has so many conformance issues that sometimes it is just easier to test
//  for it directly. On the other hand, the obvious way to do this doesn't work,
//  as many non-Microsoft compilers define _MSC_VER.  Thus BOOST_MSVC.

//  BOOST_MSVC_STD_ITERATOR: Microsoft's broken version of std::iterator
//  is being used. 

//  BOOST_SYSTEM_HAS_STDINT_H: There are no 1998 C++ Standard headers <stdint.h> 
//  or <cstdint>, although the 1999 C Standard does include <stdint.h>. 
//  If <stdint.h> is present, <boost/stdint.h> can make good use of it,
//  so a flag is supplied (signalling presence; thus the default is not
//  present, conforming to the current C++ standard).

//  BOOST_NO_SLIST: The C++ implementation does not provide the slist class.

//  BOOST_NO_HASH: The C++ implementation does not provide the hash_set
//  or hash_map classes.

//  BOOST_STD_EXTENSION_NAMESPACE: The name of the namespace in which the slist,
//  hash_set and/or hash_map templates are defined in this implementation (if any).

//  BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS: The standard library does not provide
//  templated iterator constructors for its containers.

//  BOOST_NO_LIMITS: The C++ implementation does not provide the <limits> header.

//  BOOST_NO_INTRINSIC_WCHAR_T: The C++ implementation does not provide wchar_t,
//  or it is really a synonym for another integral type. Use this symbol to
//  decide whether it is appropriate to explicitly specialize a template on
//  wchar_t if there is already a specialization for other integer types.

//  BOOST_NO_STD_ALLOCATOR: The C++ standard library does not provide
//  a standards conforming std::allocator.

//  BOOST_NO_STD_MIN_MAX: The C++ standard library does not provide
//  the min() and max() template functions that should be in <algorithm>.

//  Compilers are listed in alphabetic order (except VC++ last - see below)---//

//  GNU CC (also known as GCC and G++)  --------------------------------------//

# if defined __GNUC__
#   if __GNUC__ == 2 && __GNUC_MINOR__ == 91
       // egcs 1.1 won't parse smart_ptr.hpp without this:
#      define BOOST_NO_AUTO_PTR
#   endif
#   if __GNUC__ == 2 && __GNUC_MINOR__ <= 95
#     include <iterator>  // not sure this is the right way to do this -JGS
#     if !defined(_CXXRT_STD) && !defined(__SGI_STL) // need to ask Dietmar about this -JGS
#       define BOOST_NO_STD_ITERATOR
#       define BOOST_NO_LIMITS
#     endif
#     define BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#     define BOOST_NO_OPERATORS_IN_NAMESPACE
#   endif
#   if __GNUC__ == 2 && __GNUC_MINOR__ <= 8
#     define BOOST_NO_MEMBER_TEMPLATES
#   endif

//  Kai C++ ------------------------------------------------------------------//

#elif defined __KCC
#   define BOOST_NO_SLIST
#   define BOOST_NO_HASH

//  Greenhills C++ -----------------------------------------------------------//

#elif defined __ghs
#   define BOOST_NO_SLIST
#   define BOOST_NO_HASH

//  Borland ------------------------------------------------------------------//

#elif defined __BORLANDC__
#   define BOOST_NO_SLIST
#   define BOOST_NO_HASH
#   if __BORLANDC__ <= 0x0551
#     define BOOST_NO_INTEGRAL_INT64_T
#     define BOOST_NO_PRIVATE_IN_AGGREGATE
#   endif
#   if __BORLANDC__ <= 0x0550
// Borland C++ Builder 4 and 5:
#     define BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#     define BOOST_NO_USING_TEMPLATE
#     if __BORLANDC__ == 0x0550
// Borland C++ Builder 5, command-line compiler 5.5:
#       define BOOST_NO_OPERATORS_IN_NAMESPACE
#     endif
#   endif
#   if defined BOOST_DECL_EXPORTS
#     define BOOST_DECL __declspec(dllexport)
#   else
#     define BOOST_DECL __declspec(dllimport)
#   endif

//  Intel  -------------------------------------------------------------------//

# elif defined __ICL
#   include <iterator>  // not sure this is the right way to do this -JGS
#   if __SGI_STL_PORT >= 0x400 || __SGI_STL_PORT >= 0x321 && defined(__STL_USE_NAMESPACES)
        // a perfectly good implementation of std::iterator is supplied
#   elif defined(__SGI_STL_ITERATOR)
#     define BOOST_NO_STD_ITERATOR // No std::iterator in this case
#   else // assume using dinkumware's STL that comes with VC++ 6.0
#     define BOOST_MSVC_STD_ITERATOR
#     define BOOST_NO_STD_ITERATOR_TRAITS
#     define BOOST_NO_STDC_NAMESPACE
#     define BOOST_NO_SLIST
#     define BOOST_NO_HASH
#     define BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS
#     define BOOST_NO_STD_ALLOCATOR
#     define BOOST_NO_STD_MIN_MAX
#   endif


//  Metrowerks CodeWarrior  --------------------------------------------------//

# elif defined  __MWERKS__
#   if __MWERKS__ <= 0x4000
#     define BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#   endif
#   if __MWERKS__ <= 0x2301
#     define BOOST_NO_POINTER_TO_MEMBER_CONST
#   endif
#   if __MWERKS__ >= 0x2300
#     define BOOST_SYSTEM_HAS_STDINT_H
#   endif
#   if defined BOOST_DECL_EXPORTS
#     define BOOST_DECL __declspec(dllexport)
#   else
#     define BOOST_DECL __declspec(dllimport)
#   endif

#   define BOOST_STD_EXTENSION_NAMESPACE Metrowerks

//  Sun Workshop Compiler C++ ------------------------------------------------//

# elif defined  __SUNPRO_CC
#    if __SUNPRO_CC <= 0x500
#      define BOOST_NO_MEMBER_TEMPLATES
#      define BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#    endif

//  Microsoft Visual C++ (excluding Intel/EDG front end)  --------------------//
//
//  Must remain the last #elif since some other vendors (Metrowerks, for
//  example) also #define _MSC_VER

# elif defined _MSC_VER
#   define BOOST_MSVC _MSC_VER

    // turn off the warnings before we #include anything
#   pragma warning( disable : 4786 ) // ident trunc to '255' chars in debug info
#   pragma warning( disable : 4503 ) // warning: decorated name length exceeded

#   if _MSC_VER <= 1200  // 1200 == VC++ 6.0
#     define BOOST_NO_INCLASS_MEMBER_INITIALIZATION
#     define BOOST_NO_PRIVATE_IN_AGGREGATE

#     define BOOST_NO_INTEGRAL_INT64_T
#     define BOOST_NO_INTRINSIC_WCHAR_T

//    VC++ 6.0 has member templates but they have numerous problems including
//    cases of silent failure, so for safety we define:
#     define BOOST_NO_MEMBER_TEMPLATES
//    For VC++ experts wishing to attempt workarounds, we define:
#     define BOOST_MSVC6_MEMBER_TEMPLATES

#     define BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#     define BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#     define BOOST_NO_DEPENDENT_TYPES_IN_TEMPLATE_VALUE_PARAMETERS

#     include <iterator>  // not sure this is the right way to do this -JGS
#     if __SGI_STL_PORT >= 0x400 || __SGI_STL_PORT >= 0x321 && defined(__STL_USE_NAMESPACES)
        // a perfectly good implementation of std::iterator is supplied
#     elif defined(__SGI_STL_ITERATOR)
#       define BOOST_NO_STD_ITERATOR // No std::iterator in this case
#     else
#       define BOOST_MSVC_STD_ITERATOR 1
#       define BOOST_NO_SLIST
#       define BOOST_NO_HASH
#       define BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS
#       define BOOST_NO_STD_ALLOCATOR
#ifndef _CPPLIB_VER
        // Updated Dinkum library defines this, and provides
        // its own min and max definitions.
#       define BOOST_NO_STD_MIN_MAX
#       undef min
#       undef max
#endif
#       define NOMINMAX
#     endif
#     define BOOST_NO_STD_ITERATOR_TRAITS


// Make sure at least one standard library header is included so that library
// implementation detection will work, even if no standard headers have been
// included in front of a boost header. (Ed Brey 5 Jun 00)
#     include <cstddef>

// Determine if the standard library implementation is already pulling names
// into std.  STLport defines the following if so. (Ed Brey 5 Jun 00)
#     ifndef __STL_IMPORT_VENDOR_CSTD
#       define BOOST_NO_STDC_NAMESPACE
#     endif

#   endif

#   if defined BOOST_DECL_EXPORTS
#     define BOOST_DECL __declspec(dllexport)
#   else
#     define BOOST_DECL __declspec(dllimport)
#   endif

# endif // Microsoft (excluding Intel/EDG frontend) 

# ifndef BOOST_DECL
#   define BOOST_DECL  // default for compilers not needing this decoration.
# endif

//  end of compiler specific portion  ----------------------------------------//

#ifndef BOOST_STD_EXTENSION_NAMESPACE
# define BOOST_STD_EXTENSION_NAMESPACE std
#endif

// Check for old name "BOOST_NMEMBER_TEMPLATES" for compatibility  -----------//
// Don't use BOOST_NMEMBER_TEMPLATES. It is deprecated and will be removed soon.
#if defined( BOOST_NMEMBER_TEMPLATES ) && !defined( BOOST_NO_MEMBER_TEMPLATES )
  #define BOOST_NO_MEMBER_TEMPLATES
#endif

//  BOOST_NO_STDC_NAMESPACE workaround  --------------------------------------//
//
//  Because std::size_t usage is so common, even in boost headers which do not
//  otherwise use the C library, the <cstddef> workaround is included here so
//  that ugly workaround code need not appear in many other boost headers.
//  NOTE WELL: This is a workaround for non-conforming compilers; <cstddef> 
//  must still be #included in the usual places so that <cstddef> inclusion
//  works as expected with standard conforming compilers.  The resulting
//  double inclusion of <cstddef> is harmless.

# ifdef BOOST_NO_STDC_NAMESPACE
#   include <cstddef>
    namespace std { using ::ptrdiff_t; using ::size_t; }
    // using ::wchar_t; removed since wchar_t is a C++ built-in type (Ed Brey)
# endif

#ifdef BOOST_NO_STD_MIN_MAX
namespace std {
  template <class _Tp>
  inline const _Tp& min(const _Tp& __a, const _Tp& __b) {
    return __b < __a ? __b : __a;
  }
  template <class _Tp>
  inline const _Tp& max(const _Tp& __a, const _Tp& __b) {
    return  __a < __b ? __b : __a;
  }
#ifdef BOOST_MSVC
  inline long min(long __a, long __b) {
    return __b < __a ? __b : __a;
  }
  inline long max(long __a, long __b) {
    return  __a < __b ? __b : __a;
  }
#endif
}
#endif

#endif  // BOOST_CONFIG_HPP




