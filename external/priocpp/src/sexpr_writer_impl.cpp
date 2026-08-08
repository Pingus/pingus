// prio - Property I/O for C++
// Copyright (C) 2005-2020 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "sexpr_writer_impl.hpp"

#include <assert.h>
#include <map>

namespace prio {

namespace {

void write_escaped(std::ostream& os, std::string_view text)
{
  os << '"';
  for(char const c : text) {
    switch(c) {
      case '"':
        os << "\\\"";
        break;

      case '\\':
        os << "\\\\";
        break;

      default:
        os << c;
        break;
    }
  }
  os << '"';
}

} // namespace

SExprWriterImpl::SExprWriterImpl(std::ostream& out_) :
  out(&out_),
  level(0)
{
}

SExprWriterImpl::~SExprWriterImpl()
{
  assert(level == 0);
}

std::string
SExprWriterImpl::indent() const
{
  return std::string(level*2, ' ');
}

void
SExprWriterImpl::begin_mapping(std::string_view key)
{
  if (level != 0) {
    (*out) << std::endl;
  }
  (*out) << indent() << "(" << key;
  ++level;
}

void
SExprWriterImpl::end_mapping()
{
  --level;
  (*out) << ")";

  // insert trailing newline and EOF marker at end of file
  if (level == 0)
  {
    (*out) << "\n\n;; EOF ;;\n";
  }
}

void
SExprWriterImpl::begin_keyvalue(std::string_view key)
{
  if (level != 0) {
    (*out) << std::endl;
  }
  (*out) << indent() << "(" << key;
  ++level;
}

void
SExprWriterImpl::end_keyvalue()
{
  --level;
  (*out) << ")";
}

void
SExprWriterImpl::begin_collection(std::string_view key)
{
  begin_mapping(key);
}

void
SExprWriterImpl::end_collection()
{
  end_mapping();
}

void
SExprWriterImpl::begin_object(std::string_view type)
{
  begin_mapping(type);
}

void
SExprWriterImpl::end_object()
{
  end_mapping();
}

void
SExprWriterImpl::write(std::string_view key, bool value)
{
  (*out) << "\n" << indent() << "(" << key << " " << (value ? "#t" : "#f") << ")";
}

void
SExprWriterImpl::write(std::string_view key, int value)
{
  (*out) << "\n" << indent() << "(" << key << " " << value << ")";
}

void
SExprWriterImpl::write(std::string_view key, float value)
{
  (*out) << "\n" << indent() << "(" << key << " " << value << ")";
}

void
SExprWriterImpl::write(std::string_view key, char const* value)
{
  write(key, std::string_view(value));
}

void
SExprWriterImpl::write(std::string_view key, std::string_view value)
{
  (*out) << "\n" << indent() << "(" << key << " ";
  write_escaped(*out, value);
  (*out) << ")";
}

void
SExprWriterImpl::write(std::string_view key, std::span<bool const> values)
{
  (*out) << "\n" << indent() << "(" << key;
  for (bool const value : values) {
    (*out) << ' ' << (value ? "#t" : "#f");
  }
  (*out) << ")";
}

void
SExprWriterImpl::write(std::string_view key, std::span<int const> values)
{
  (*out) << "\n" << indent() << "(" << key;
  for (int const value : values) {
    (*out) << ' ' << value;
  }
  (*out) << ")";
}

void
SExprWriterImpl::write(std::string_view key, std::span<float const> values)
{
  (*out) << "\n" << indent() << "(" << key;
  for (float const value : values) {
    (*out) << ' ' << value;
  }
  (*out) << ")";
}

void
SExprWriterImpl::write(std::string_view key, std::span<std::string const> values)
{
  (*out) << "\n" << indent() << "(" << key;
  for (std::string const& value : values) {
    (*out) << ' ';
    write_escaped(*out, value);
  }
  (*out) << ")";
}

void
SExprWriterImpl::write(std::string_view key, std::vector<bool> const& values)
{
  (*out) << "\n" << indent() << "(" << key;
  for (bool const value : values) {
    (*out) << ' ' << (value ? "#t" : "#f");
  }
  (*out) << ")";
}

} // namespace prio

/* EOF */
