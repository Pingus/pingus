
//  Copyright (C) 2004 Matthias Braun <matze@braunis.de>
//  code in this file based on lispreader from Mark Probst
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#ifndef __LISP_HPP__
#define __LISP_HPP__

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <assert.h>
#include <boost/shared_ptr.hpp>

namespace lisp
{

class Lisp
{
public:
  enum LispType {
    TYPE_LIST,
    TYPE_SYMBOL,
    TYPE_INT,
    TYPE_STRING,
    TYPE_FLOAT,
    TYPE_BOOL
  };

  /// construct a new Lisp object symbol or string object
  Lisp(LispType newtype, const std::string& value);
  Lisp(const std::vector<boost::shared_ptr<Lisp> >& list_elements);
  Lisp(int val);
  Lisp(float val);
  Lisp(bool val);
  ~Lisp();                                             

  LispType get_type() const
  { return type; } 

  size_t get_list_size() const
  {
    return v.list.size;
  }
  boost::shared_ptr<Lisp> get_list_elem(size_t i) const
  {
    assert(i < v.list.size);
    return v.list.entries[i];
  }

  const char* get_string() const
  {
    if(type != TYPE_STRING)
      throw std::runtime_error("Lisp is not a string");
    return v.string;
  }

  const char* get_symbol() const
  {
    if(type != TYPE_SYMBOL)
      throw std::runtime_error("Lisp is not a symbol");
    return v.string;
  }

  int get_int() const
  {
    if(type != TYPE_INT)
      throw std::runtime_error("Lisp is not an int");
    return v.int_;
  }

  float get_float() const
  {
    if(type != TYPE_FLOAT)
      {
        if(type != TYPE_INT)
          throw std::runtime_error("Lisp is not a float");
        else
          return (float)v.int_;
      }
    else
      {    
        return v.float_;
      }
  }

  bool get_bool() const
  {
    if(type != TYPE_BOOL)
      throw std::runtime_error("Lisp is not a bool");
    return v.bool_;
  }

  void print(std::ostream& out = std::cout, int indent = 0) const;

private:
  union
  {
    struct {
      boost::shared_ptr<Lisp>* entries;
      size_t size;
    } list;
    char* string;
    int int_;
    bool bool_;
    float float_;
  } v;
  LispType type;
};

} // end of namespace lisp

#endif

