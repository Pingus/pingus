//  boost utility.hpp header file  -------------------------------------------//

//  (C) Copyright boost.org 1999. Permission to copy, use, modify, sell
//  and distribute this software is granted provided this copyright
//  notice appears in all copies. This software is provided "as is" without
//  express or implied warranty, and with no claim as to its suitability for
//  any purpose.

//  See http://www.boost.org for most recent version including documentation.

//  Classes appear in alphabetical order

//  Revision History
//  26 Jan 00  protected noncopyable destructor added (Miki Jovanovic)
//  10 Dec 99  next() and prior() templates added (Dave Abrahams)
//  30 Aug 99  moved cast templates to cast.hpp (Beman Dawes)
//   3 Aug 99  cast templates added
//  20 Jul 99  name changed to utility.hpp 
//   9 Jun 99  protected noncopyable default ctor
//   2 Jun 99  Initial Version. Class noncopyable only contents (Dave Abrahams)

#ifndef BOOST_UTILITY_HPP
#define BOOST_UTILITY_HPP

#include "config.hpp"
#include <cstddef>            // for size_t
#include <utility>            // for std::pair

namespace boost
{

//  next() and prior() template functions  -----------------------------------//

    //  Helper functions for classes like bidirectional iterators not supporting
    //  operator+ and operator-.
    //
    //  Usage:
    //    const std::list<T>::iterator p = get_some_iterator();
    //    const std::list<T>::iterator prev = boost::prior(p);

    //  Contributed by Dave Abrahams

    template <class T>
    T next(T x) { return ++x; }

    template <class T>
    T prior(T x) { return --x; }


//  class noncopyable  -------------------------------------------------------//

    //  Private copy constructor and copy assignment ensure classes derived from
    //  class noncopyable cannot be copied.

    //  Contributed by Dave Abrahams

    class noncopyable
    {
    protected:
        noncopyable(){}
        ~noncopyable(){}
    private:  // emphasize the following members are private
        noncopyable( const noncopyable& );
        const noncopyable& operator=( const noncopyable& );
    }; // noncopyable

//  class tied  -------------------------------------------------------//

    // A helper for conveniently assigning the two values from a pair
    // into separate variables. The idea for this comes from Jaakko J„rvi's
    // Binder/Lambda Library.

    // Constributed by Jeremy Siek

    template <class A, class B>
    class tied {
    public:
      inline tied(A& a, B& b) : _a(a), _b(b) { }
      template <class U, class V>
      inline tied& operator=(const std::pair<U,V>& p) {
        _a = p.first;
        _b = p.second;
        return *this;
      }
    protected:
      A& _a;
      B& _b;
    };

    template <class A, class B>
    inline tied<A,B> tie(A& a, B& b) { return tied<A,B>(a, b); }

} // namespace boost

#endif  // BOOST_UTILITY_HPP

