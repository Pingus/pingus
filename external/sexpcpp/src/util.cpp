// SExp - A S-Expression Parser for C++
// Copyright (C) 2006 Matthias Braun <matze@braunis.de>
//               2015 Ingo Ruhnke <grumbel@gmail.com>
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

#include "sexp/util.hpp"

#include <sstream>
#include <stdexcept>

#include "sexp/io.hpp"
#include "sexp/error.hpp"

namespace sexp {

bool
is_list(Value const& sx)
{
  if (sx.is_nil())
  {
    return true;
  }
  else if (sx.is_cons())
  {
    return is_list(sx.get_cdr());
  }
  else
  {
    return false;
  }
}

int
list_length(Value const& sx)
{
  if (sx.is_nil())
  {
    return 0;
  }
  else if (sx.is_cons())
  {
    return 1 + list_length(sx.get_cdr());
  }
  else
  {
    throw TypeError(sx.get_line(), "sexp::list_length(): wrong type, expected list");
  }
}

Value const&
list_ref(Value const& sx, int index)
{
  if (index == 0)
  {
    return sx.get_car();
  }
  else
  {
    return list_ref(sx.get_cdr(), index - 1);
  }
}

Value const&
assoc_ref(Value const& sx, std::string const& key)
{
  if (sx.is_nil())
  {
    return Value::nil_ref();
  }
  else if (sx.is_cons())
  {
    Value const& pair = sx.get_car();
    if (pair.is_cons() &&
        pair.get_car().is_symbol() &&
        pair.get_car().as_string() == key)
    {
      return pair.get_cdr();
    }
    else
    {
      return assoc_ref(sx.get_cdr(), key);
    }
  }
  else
  {
    std::ostringstream msg;
    msg << "malformed input to sexp::assoc_ref(): sx:\"" << sx << "\" key:\"" << key << "\"";
    throw std::runtime_error(msg.str());
  }
}

} // namespace sexp

/* EOF */
