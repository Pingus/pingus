//  Boost smart_ptr.hpp header file  -----------------------------------------//

//  (C) Copyright Greg Colvin and Beman Dawes 1998, 1999. Permission to copy,
//  use, modify, sell and distribute this software is granted provided this
//  copyright notice appears in all copies. This software is provided "as is"
//  without express or implied warranty, and with no claim as to its
//  suitability for any purpose.

//  See http://www.boost.org for most recent version including documentation.

//  Revision History
//  19 Oct 00  Make shared_ptr ctor from auto_ptr explicit. (Robert Vugts) 
//  24 Jul 00  Change throw() to // never throws.  See lib guidelines
//             Exception-specification rationale. (Beman Dawes)
//  22 Jun 00  Remove #if continuations to fix GCC 2.95.2 problem (Beman Dawes)
//   1 Feb 00  Additional shared_ptr BOOST_NO_MEMBER_TEMPLATES workarounds
//             (Dave Abrahams)
//  31 Dec 99  Condition tightened for no member template friend workaround
//             (Dave Abrahams)
//  30 Dec 99  Moved BOOST_NMEMBER_TEMPLATES compatibility code to config.hpp
//             (Dave Abrahams)
//  30 Nov 99  added operator ==, operator !=, and std::swap and std::less
//             specializations for shared types (Darin Adler)
//  11 Oct 99  replaced op[](int) with op[](std::size_t) (Ed Brey, Valentin
//             Bonnard), added shared_ptr workaround for no member template
//             friends (Matthew Langston)
//  25 Sep 99  added shared_ptr::swap and shared_array::swap (Luis Coelho).
//  20 Jul 99  changed name to smart_ptr.hpp, #include <boost/config.hpp>,
//             #include <boost/utility.hpp> and use boost::noncopyable
//  17 May 99  remove scoped_array and shared_array operator*() as
//             unnecessary (Beman Dawes)
//  14 May 99  reorder code so no effects when bad_alloc thrown (Abrahams/Dawes)
//  13 May 99  remove certain throw() specifiers to avoid generated try/catch
//             code cost (Beman Dawes)
//  11 May 99  get() added, conversion to T* placed in macro guard (Valentin
//             Bonnard, Dave Abrahams, and others argued for elimination
//             of the automatic conversion)
//  28 Apr 99  #include <memory> fix (Valentin Bonnard)
//  28 Apr 99  rename transfer() to share() for clarity (Dave Abrahams)
//  28 Apr 99  remove unsafe shared_array template conversions(Valentin Bonnard)
//  28 Apr 99  p(r) changed to p(r.px) for clarity (Dave Abrahams)
//  21 Apr 99  reset() self assignment fix (Valentin Bonnard)
//  21 Apr 99  dispose() provided to improve clarity (Valentin Bonnard)
//  27 Apr 99  leak when new throws fixes (Dave Abrahams)
//  21 Oct 98  initial Version (Greg Colvin/Beman Dawes)

#ifndef BOOST_SMART_PTR_HPP
#define BOOST_SMART_PTR_HPP

#include "config.hpp"   // for broken compiler workarounds
#include <cstddef>            // for std::size_t
#include <memory>             // for std::auto_ptr
#include <algorithm>          // for std::swap
#include "utility.hpp"  // for boost::noncopyable
#include <functional>         // for std::less

namespace boost {

//  scoped_ptr  --------------------------------------------------------------//

//  scoped_ptr mimics a built-in pointer except that it guarantees deletion
//  of the object pointed to, either on destruction of the scoped_ptr or via
//  an explicit reset().  scoped_ptr is a simple solution for simple needs;
//  see shared_ptr (below) or std::auto_ptr if your needs are more complex.

template<typename T> class scoped_ptr : noncopyable {

  T* ptr;

 public:
  typedef T element_type;

  explicit scoped_ptr( T* p=0 ) : ptr(p) {}  // never throws
  ~scoped_ptr()                 { delete ptr; }

  void reset( T* p=0 )          { if ( ptr != p ) { delete ptr; ptr = p; } }
  T& operator*() const          { return *ptr; }  // never throws
  T* operator->() const         { return ptr; }  // never throws
  T* get() const                { return ptr; }  // never throws
#ifdef BOOST_SMART_PTR_CONVERSION
  // get() is safer! Define BOOST_SMART_PTR_CONVERSION at your own risk!
  operator T*() const           { return ptr; }  // never throws 
#endif
  };  // scoped_ptr

//  scoped_array  ------------------------------------------------------------//

//  scoped_array extends scoped_ptr to arrays. Deletion of the array pointed to
//  is guaranteed, either on destruction of the scoped_array or via an explicit
//  reset(). See shared_array or std::vector if your needs are more complex.

template<typename T> class scoped_array : noncopyable {

  T* ptr;

 public:
  typedef T element_type;

  explicit scoped_array( T* p=0 ) : ptr(p) {}  // never throws
  ~scoped_array()                    { delete [] ptr; }

  void reset( T* p=0 )               { if ( ptr != p ) {delete [] ptr; ptr=p;} }

  T* get() const                     { return ptr; }  // never throws
#ifdef BOOST_SMART_PTR_CONVERSION
  // get() is safer! Define BOOST_SMART_PTR_CONVERSION at your own risk!
  operator T*() const                { return ptr; }  // never throws
#else 
  T& operator[](std::size_t i) const { return ptr[i]; }  // never throws
#endif
  };  // scoped_array

//  shared_ptr  --------------------------------------------------------------//

//  An enhanced relative of scoped_ptr with reference counted copy semantics.
//  The object pointed to is deleted when the last shared_ptr pointing to it
//  is destroyed or reset.

template<typename T> class shared_ptr {
  public:
   typedef T element_type;

   explicit shared_ptr(T* p =0) : px(p) {
      try { pn = new long(1); }  // fix: prevent leak if new throws
      catch (...) { delete p; throw; } 
   }

   shared_ptr(const shared_ptr& r) : px(r.px) { ++*(pn = r.pn); }  // never throws

   ~shared_ptr() { dispose(); }

   shared_ptr& operator=(const shared_ptr& r) {
      share(r.px,r.pn);
      return *this;
   }

#if !defined( BOOST_NO_MEMBER_TEMPLATES )
   template<typename Y>
      shared_ptr(const shared_ptr<Y>& r) : px(r.px) {  // never throws 
         ++*(pn = r.pn); 
      }
#ifndef BOOST_NO_AUTO_PTR
   template<typename Y>
      explicit shared_ptr(std::auto_ptr<Y>& r) { 
         pn = new long(1); // may throw
         px = r.release(); // fix: moved here to stop leak if new throws
      }
#endif 

   template<typename Y>
      shared_ptr& operator=(const shared_ptr<Y>& r) { 
         share(r.px,r.pn);
         return *this;
      }

#ifndef BOOST_NO_AUTO_PTR
   template<typename Y>
      shared_ptr& operator=(std::auto_ptr<Y>& r) {
         // code choice driven by guarantee of "no effect if new throws"
         if (*pn == 1) { delete px; }
         else { // allocate new reference counter
           long * tmp = new long(1); // may throw
           --*pn; // only decrement once danger of new throwing is past
           pn = tmp;
         } // allocate new reference counter
         px = r.release(); // fix: moved here so doesn't leak if new throws 
         return *this;
      }
#endif
#else
#ifndef BOOST_NO_AUTO_PTR
      explicit shared_ptr(std::auto_ptr<T>& r) { 
         pn = new long(1); // may throw
         px = r.release(); // fix: moved here to stop leak if new throws
      } 

      shared_ptr& operator=(std::auto_ptr<T>& r) {
         // code choice driven by guarantee of "no effect if new throws"
         if (*pn == 1) { delete px; }
         else { // allocate new reference counter
           long * tmp = new long(1); // may throw
           --*pn; // only decrement once danger of new throwing is past
           pn = tmp;
         } // allocate new reference counter
         px = r.release(); // fix: moved here so doesn't leak if new throws 
         return *this;
      }
#endif
#endif

   void reset(T* p=0) {
      if ( px == p ) return;  // fix: self-assignment safe
      if (--*pn == 0) { delete px; }
      else { // allocate new reference counter
        try { pn = new long; }  // fix: prevent leak if new throws
        catch (...) {
          ++*pn;  // undo effect of --*pn above to meet effects guarantee 
          delete p;
          throw;
        } // catch
      } // allocate new reference counter
      *pn = 1;
      px = p;
   } // reset

   T& operator*() const          { return *px; }  // never throws
   T* operator->() const         { return px; }  // never throws
   T* get() const                { return px; }  // never throws
 #ifdef BOOST_SMART_PTR_CONVERSION
   // get() is safer! Define BOOST_SMART_PTR_CONVERSION at your own risk!
   operator T*() const           { return px; }  // never throws 
 #endif

   long use_count() const        { return *pn; }  // never throws
   bool unique() const           { return *pn == 1; }  // never throws

   void swap(shared_ptr<T>& other)  // never throws
     { std::swap(px,other.px); std::swap(pn,other.pn); }

// Tasteless as this may seem, making all members public allows member templates
// to work in the absence of member template friends. (Matthew Langston)
// Don't split this line into two; that causes problems for some GCC 2.95.2 builds
#if defined(BOOST_NO_MEMBER_TEMPLATES) || !defined( BOOST_NO_MEMBER_TEMPLATE_FRIENDS )
   private:
#endif

   T*     px;     // contained pointer
   long*  pn;     // ptr to reference counter

// Don't split this line into two; that causes problems for some GCC 2.95.2 builds
#if !defined( BOOST_NO_MEMBER_TEMPLATES ) && !defined( BOOST_NO_MEMBER_TEMPLATE_FRIENDS )
   template<typename Y> friend class shared_ptr;
#endif

   void dispose() { if (--*pn == 0) { delete px; delete pn; } }

   void share(T* rpx, long* rpn) {
      if (pn != rpn) {
         dispose();
         px = rpx;
         ++*(pn = rpn);
      }
   } // share
};  // shared_ptr

template<typename T, typename U>
  inline bool operator==(const shared_ptr<T>& a, const shared_ptr<U>& b)
    { return a.get() == b.get(); }
  /* grumbel uncommented because: 
ObjectManager.cc:426:   instantiated from here
/usr/lib/gcc-lib/i386-linux/2.95.2/../../../../include/g++-3/stl_algo.h:94: ambiguous overload for `boost::shared_ptr<EditorObj> & != const boost::shared_ptr<EditorObj> &'
/usr/lib/gcc-lib/i386-linux/2.95.2/../../../../include/g++-3/stl_relops.h:37: candidates are: bool operator !=<boost::shared_ptr<EditorObj> >(const boost::shared_ptr<EditorObj> &, const boost::shared_ptr<EditorObj> &)
../boost/smart_ptr.hpp:252:                 bool boost::operator !=<EditorObj, EditorObj>(const boost::shared_ptr<EditorObj> &, const boost::shared_ptr<EditorObj> &)

template<typename T, typename U>
  inline bool operator!=(const shared_ptr<T>& a, const shared_ptr<U>& b)
    { return a.get() != b.get(); }
  */
//  shared_array  ------------------------------------------------------------//

//  shared_array extends shared_ptr to arrays.
//  The array pointed to is deleted when the last shared_array pointing to it
//  is destroyed or reset.

template<typename T> class shared_array {
  public:
   typedef T element_type;

   explicit shared_array(T* p =0) : px(p) {
      try { pn = new long(1); }  // fix: prevent leak if new throws
      catch (...) { delete [] p; throw; } 
   }

   shared_array(const shared_array& r) : px(r.px)  // never throws
      { ++*(pn = r.pn); }

   ~shared_array() { dispose(); }

   shared_array& operator=(const shared_array& r) {
      if (pn != r.pn) {
         dispose();
         px = r.px;
         ++*(pn = r.pn);
      }
      return *this;
   } // operator=

   void reset(T* p=0) {
      if ( px == p ) return;  // fix: self-assignment safe
      if (--*pn == 0) { delete [] px; }
      else { // allocate new reference counter
        try { pn = new long; }  // fix: prevent leak if new throws
        catch (...) {
          ++*pn;  // undo effect of --*pn above to meet effects guarantee 
          delete [] p;
          throw;
        } // catch
      } // allocate new reference counter
      *pn = 1;
      px = p;
   } // reset

   T* get() const                     { return px; }  // never throws
 #ifdef BOOST_SMART_PTR_CONVERSION
   // get() is safer! Define BOOST_SMART_PTR_CONVERSION at your own risk!
   operator T*() const                { return px; }  // never throws
 #else 
   T& operator[](std::size_t i) const { return px[i]; }  // never throws
 #endif

   long use_count() const             { return *pn; }  // never throws
   bool unique() const                { return *pn == 1; }  // never throws

   void swap(shared_array<T>& other)  // never throws
     { std::swap(px,other.px); std::swap(pn,other.pn); }

  private:

   T*     px;     // contained pointer
   long*  pn;     // ptr to reference counter

   void dispose() { if (--*pn == 0) { delete [] px; delete pn; } }

};  // shared_array

template<typename T>
  inline bool operator==(const shared_array<T>& a, const shared_array<T>& b)
    { return a.get() == b.get(); }

template<typename T>
  inline bool operator!=(const shared_array<T>& a, const shared_array<T>& b)
    { return a.get() != b.get(); }

} // namespace boost

//  specializations for things in namespace std  -----------------------------//

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

namespace std {

// Specialize std::swap to use the fast, non-throwing swap that's provided
// as a member function instead of using the default algorithm which creates
// a temporary and uses assignment.

template<typename T>
  inline void swap(boost::shared_ptr<T>& a, boost::shared_ptr<T>& b)
    { a.swap(b); }

template<typename T>
  inline void swap(boost::shared_array<T>& a, boost::shared_array<T>& b)
    { a.swap(b); }

// Specialize std::less so we can use shared pointers and arrays as keys in
// associative collections.

// It's still a controversial question whether this is better than supplying
// a full range of comparison operators (<, >, <=, >=).

template<typename T>
  struct less< boost::shared_ptr<T> >
    : binary_function<boost::shared_ptr<T>, boost::shared_ptr<T>, bool>
  {
    bool operator()(const boost::shared_ptr<T>& a,
        const boost::shared_ptr<T>& b) const
      { return less<T*>()(a.get(),b.get()); }
  };

template<typename T>
  struct less< boost::shared_array<T> >
    : binary_function<boost::shared_array<T>, boost::shared_array<T>, bool>
  {
    bool operator()(const boost::shared_array<T>& a,
        const boost::shared_array<T>& b) const
      { return less<T*>()(a.get(),b.get()); }
  };

} // namespace std

#endif  // ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif  // BOOST_SMART_PTR_HPP


