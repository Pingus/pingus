// SExp - A S-Expression Parser for C++
// Copyright (C) 2015 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_SEXP_UTIL_HPP
#define HEADER_SEXP_UTIL_HPP

#include <sexp/value.hpp>

namespace sexp {

class Value;

inline Value& car(Value& sx) { return sx.get_car(); }
inline Value& cdr(Value& sx) { return sx.get_cdr(); }
inline Value& caar(Value& sx) { return sx.get_car().get_car(); }
inline Value& cadr(Value& sx) { return sx.get_car().get_cdr(); }
inline Value& cdar(Value& sx) { return sx.get_cdr().get_car(); }
inline Value& cddr(Value& sx) { return sx.get_cdr().get_cdr(); }
inline Value& caaar(Value& sx) { return sx.get_car().get_car().get_car(); }
inline Value& caadr(Value& sx) { return sx.get_car().get_car().get_cdr(); }
inline Value& cadar(Value& sx) { return sx.get_car().get_cdr().get_car(); }
inline Value& caddr(Value& sx) { return sx.get_car().get_cdr().get_cdr(); }
inline Value& cdaar(Value& sx) { return sx.get_cdr().get_car().get_car(); }
inline Value& cdadr(Value& sx) { return sx.get_cdr().get_car().get_cdr(); }
inline Value& cddar(Value& sx) { return sx.get_cdr().get_cdr().get_car(); }
inline Value& cdddr(Value& sx) { return sx.get_cdr().get_cdr().get_cdr(); }
inline Value& caaaar(Value& sx) { return sx.get_car().get_car().get_car().get_car(); }
inline Value& caaadr(Value& sx) { return sx.get_car().get_car().get_car().get_cdr(); }
inline Value& caadar(Value& sx) { return sx.get_car().get_car().get_cdr().get_car(); }
inline Value& caaddr(Value& sx) { return sx.get_car().get_car().get_cdr().get_cdr(); }
inline Value& cadaar(Value& sx) { return sx.get_car().get_cdr().get_car().get_car(); }
inline Value& cadadr(Value& sx) { return sx.get_car().get_cdr().get_car().get_cdr(); }
inline Value& caddar(Value& sx) { return sx.get_car().get_cdr().get_cdr().get_car(); }
inline Value& cadddr(Value& sx) { return sx.get_car().get_cdr().get_cdr().get_cdr(); }
inline Value& cdaaar(Value& sx) { return sx.get_cdr().get_car().get_car().get_car(); }
inline Value& cdaadr(Value& sx) { return sx.get_cdr().get_car().get_car().get_cdr(); }
inline Value& cdadar(Value& sx) { return sx.get_cdr().get_car().get_cdr().get_car(); }
inline Value& cdaddr(Value& sx) { return sx.get_cdr().get_car().get_cdr().get_cdr(); }
inline Value& cddaar(Value& sx) { return sx.get_cdr().get_cdr().get_car().get_car(); }
inline Value& cddadr(Value& sx) { return sx.get_cdr().get_cdr().get_car().get_cdr(); }
inline Value& cdddar(Value& sx) { return sx.get_cdr().get_cdr().get_cdr().get_car(); }
inline Value& cddddr(Value& sx) { return sx.get_cdr().get_cdr().get_cdr().get_cdr(); }

inline Value const& car(Value const& sx) { return sx.get_car(); }
inline Value const& cdr(Value const& sx) { return sx.get_cdr(); }
inline Value const& caar(Value const& sx) { return sx.get_car().get_car(); }
inline Value const& cadr(Value const& sx) { return sx.get_car().get_cdr(); }
inline Value const& cdar(Value const& sx) { return sx.get_cdr().get_car(); }
inline Value const& cddr(Value const& sx) { return sx.get_cdr().get_cdr(); }
inline Value const& caaar(Value const& sx) { return sx.get_car().get_car().get_car(); }
inline Value const& caadr(Value const& sx) { return sx.get_car().get_car().get_cdr(); }
inline Value const& cadar(Value const& sx) { return sx.get_car().get_cdr().get_car(); }
inline Value const& caddr(Value const& sx) { return sx.get_car().get_cdr().get_cdr(); }
inline Value const& cdaar(Value const& sx) { return sx.get_cdr().get_car().get_car(); }
inline Value const& cdadr(Value const& sx) { return sx.get_cdr().get_car().get_cdr(); }
inline Value const& cddar(Value const& sx) { return sx.get_cdr().get_cdr().get_car(); }
inline Value const& cdddr(Value const& sx) { return sx.get_cdr().get_cdr().get_cdr(); }
inline Value const& caaaar(Value const& sx) { return sx.get_car().get_car().get_car().get_car(); }
inline Value const& caaadr(Value const& sx) { return sx.get_car().get_car().get_car().get_cdr(); }
inline Value const& caadar(Value const& sx) { return sx.get_car().get_car().get_cdr().get_car(); }
inline Value const& caaddr(Value const& sx) { return sx.get_car().get_car().get_cdr().get_cdr(); }
inline Value const& cadaar(Value const& sx) { return sx.get_car().get_cdr().get_car().get_car(); }
inline Value const& cadadr(Value const& sx) { return sx.get_car().get_cdr().get_car().get_cdr(); }
inline Value const& caddar(Value const& sx) { return sx.get_car().get_cdr().get_cdr().get_car(); }
inline Value const& cadddr(Value const& sx) { return sx.get_car().get_cdr().get_cdr().get_cdr(); }
inline Value const& cdaaar(Value const& sx) { return sx.get_cdr().get_car().get_car().get_car(); }
inline Value const& cdaadr(Value const& sx) { return sx.get_cdr().get_car().get_car().get_cdr(); }
inline Value const& cdadar(Value const& sx) { return sx.get_cdr().get_car().get_cdr().get_car(); }
inline Value const& cdaddr(Value const& sx) { return sx.get_cdr().get_car().get_cdr().get_cdr(); }
inline Value const& cddaar(Value const& sx) { return sx.get_cdr().get_cdr().get_car().get_car(); }
inline Value const& cddadr(Value const& sx) { return sx.get_cdr().get_cdr().get_car().get_cdr(); }
inline Value const& cdddar(Value const& sx) { return sx.get_cdr().get_cdr().get_cdr().get_car(); }
inline Value const& cddddr(Value const& sx) { return sx.get_cdr().get_cdr().get_cdr().get_cdr(); }

int list_length(Value const& sx);
Value const& list_ref(Value const& sx, int index);
bool is_list(Value const& sx);
Value const& assoc_ref(Value const& sx, std::string const& key);

class ListIterator
{
private:
  Value const* cur;

public:
  ListIterator() :
    cur(nullptr)
  {}

  ListIterator(Value const& sx) :
    cur(!sx.is_cons() ? nullptr : &sx)
  {}

  bool operator==(ListIterator const& rhs) const { return cur == rhs.cur; }
  bool operator!=(ListIterator const& rhs) const { return cur != rhs.cur; }

  Value const& operator*() const { return cur->get_car(); /* NOLINT */ }
  Value const* operator->() const { return &cur->get_car(); }

  ListIterator& operator++()
  {
    if (cur)
    {
      cur = &cur->get_cdr();
      if (!cur->is_cons())
      {
        // if the list is malformed we stop at the last valid element
        // and silently ignore the rest
        cur = nullptr;
      }
    }
    return *this;
  }

  ListIterator operator++(int)
  {
    ListIterator tmp = *this;
    operator++();
    return tmp;
  }
};

class ListAdapter
{
private:
  Value const& m_sx;

public:
  ListAdapter(Value const& sx) :
    m_sx(sx)
  {}

  ListIterator begin() const { return ListIterator(m_sx); }
  ListIterator end() const { return ListIterator(); }
};

} // namespace sexp

#endif

/* EOF */
