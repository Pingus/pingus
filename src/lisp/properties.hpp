#ifndef __PROPERTIES_HPP__
#define __PROPERTIES_HPP__

#include <vector>
#include "getters.hpp"
#include "property_iterator.hpp"

namespace lisp
{

class Properties
{
public:
  Properties(const lisp::Lisp* lisp);
  ~Properties();

  /**
   * fetches the value of a property. returns true if the property was defined
   * and could be converted into the target type, false otherwise.
   * You have to define get() or property_get() functions to support new
   * datatypes. See getters.hpp for details
   */
  template<typename T>
  bool get(const std::string& name, T& val)
  {
    PropertyMap::iterator i = properties.find(name);
    if(i == properties.end())
      return false;
    if(property_get(i->second.lisp, val) == false)
      return false;
    i->second.used = true;
    return true;
  }

  /** 
   * Ignore a property so that it doesn't give a warning, usefull for
   * example if some tags are only used by the editor but not by the
   * game.
   */
  void ignore(const std::string& name)
  {
    PropertyMap::iterator i = properties.find(name);
    if(i != properties.end())
      i->second.used = true;
  }

  /**
   * returns an iterator over all properties with a certain name
   */
  template<typename T>
  bool get_iter(const std::string& name, PropertyIterator<T>& iter)
  {
    PropertyMap::iterator beg = properties.lower_bound(name);
    if(beg == properties.end() || beg->first != name) {
      iter = PropertyIterator<T>(properties.end(), properties.end());
      return false;
    }
    PropertyMap::iterator end = properties.upper_bound(name);
        
    iter = PropertyIterator<T>(beg, end);
    return true;
  }

  /**
   * returns an iterator over all properties
   */
  PropertyIterator<const Lisp*> get_iter()
  {
    return PropertyIterator<const Lisp*>(properties.begin(), properties.end());
  }

  /**
   * Prints a warning for properties that have not been accessed. This typically
   * indicates typos that the user should know about
   */
  void print_unused_warnings(const std::string& context) const;

private:
  PropertyMap properties;
};

}

#endif

