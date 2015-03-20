/** This header defines functions that assign the value of Lisp objects to
 * normal C++ datatypes.
 *
 * The get function is overloaded for a set of default datatypes. You can add
 * further overloads in your own code. The get functions return true if the
 * value could be converted and false otherwise.
 *
 * The property_get function expect a list whose values 1-n are transformed to a
 * C++ object. This is typically used from the code in the Properties class. You
 * can also write custom overloads in your code.
 */

#ifndef __GETTERS_HPP__
#define __GETTERS_HPP__

#include "lisp.hpp"

namespace lisp
{

bool get(const Lisp* lisp, bool& val);
bool get(const Lisp* lisp, float& val);
bool get(const Lisp* lisp, int& val);
bool get(const Lisp* lisp, std::string& val);
bool get(const Lisp* lisp, const Lisp*& val);

template<typename T>
static inline bool property_get(const Lisp* lisp, T& val)
{
  if(lisp->get_list_size() != 2)
    return false;

  const Lisp* el = lisp->get_list_elem(1);
  return get(el, val);
}

static inline bool property_get(const Lisp* lisp, const Lisp*& val)
{
  val = lisp;
  return true;
}

template<typename T>
static inline bool property_get(const Lisp* lisp, std::vector<T>& list)
{
  list.clear();
  for(size_t n = 1; n < lisp->get_list_size(); ++n) {
    T val;
    if(get(lisp->get_list_elem(n), val) == false) {
      list.clear();
      return false;
    }
    list.push_back(val);
  }
  return true;
}

}

#endif

