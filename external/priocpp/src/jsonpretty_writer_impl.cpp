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

#include "jsonpretty_writer_impl.hpp"

#include <sstream>
#include <ostream>
#include <assert.h>

namespace prio {

JsonPrettyWriterImpl::JsonPrettyWriterImpl(std::ostream& out) :
  m_out(out),
  m_depth(0),
  m_write_seperator( { false } ),
  m_context()
{
}

JsonPrettyWriterImpl::~JsonPrettyWriterImpl()
{
  assert(m_context.empty());
}

void
JsonPrettyWriterImpl::begin_collection(std::string_view key)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(key);
  m_out << ": [";

  m_context.push_back(Context::Collection);
  m_write_seperator.push_back(false);
  m_depth += 1;
}

void
JsonPrettyWriterImpl::end_collection()
{
  assert(m_context.back() == Context::Collection);

  m_write_seperator.back() = false;
  m_depth -= 1;

  write_indent();
  m_out << "]";

  m_context.pop_back();
  m_write_seperator.pop_back();

  write_separator();
}

void
JsonPrettyWriterImpl::begin_object(std::string_view type)
{
  assert(m_context.empty() ||
         m_context.back() == Context::Collection ||
         m_context.back() == Context::KeyValue);

  if (m_depth != 0) {
    write_indent();
  }

  m_out << "{";
  m_depth += 1;
  write_indent();
  write_quoted_string(type);
  m_out << ": {";

  m_context.push_back(Context::Mapping);
  m_write_seperator.push_back(false);
  m_depth += 1;
}

void
JsonPrettyWriterImpl::end_object()
{
  m_write_seperator.back() = false;
  m_depth -= 1;

  write_indent();
  m_out << "}";
  m_depth -= 1;
  write_indent();
  m_out << "}";

  if (m_depth == 0)
  {
    m_out << "\n";
  }

  m_context.pop_back();
  m_write_seperator.pop_back();

  write_separator();
}

void
JsonPrettyWriterImpl::begin_mapping(std::string_view key)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(key);
  m_out << ": {";

  m_context.push_back(Context::Mapping);
  m_write_seperator.push_back(false);
  m_depth += 1;
}

void
JsonPrettyWriterImpl::end_mapping()
{
  assert(m_context.back() == Context::Mapping);

  m_write_seperator.back() = false;
  m_depth -= 1;

  write_indent();
  m_out << "}";

  m_context.pop_back();
  m_write_seperator.pop_back();

  write_separator();
}

void
JsonPrettyWriterImpl::begin_keyvalue(std::string_view key)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(key);
  m_out << ": {";

  m_context.push_back(Context::KeyValue);
  m_write_seperator.push_back(false);
  m_depth += 1;
}

void
JsonPrettyWriterImpl::end_keyvalue()
{
  assert(m_context.back() == Context::KeyValue);

  m_write_seperator.back() = false;
  m_depth -= 1;

  write_indent();
  m_out << "}";

  m_context.pop_back();
  m_write_seperator.pop_back();

  write_separator();
}

void
JsonPrettyWriterImpl::write(std::string_view key, bool value)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(key);
  m_out << ": " << (value ? "true" : "false");
  write_separator();
}

void
JsonPrettyWriterImpl::write(std::string_view key, int value)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(key);
  m_out << ": " << value;
  write_separator();
}

void
JsonPrettyWriterImpl::write(std::string_view key, float value)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(key);
  m_out << ": " << value;
  write_separator();
}

void
JsonPrettyWriterImpl::write(std::string_view key, char const* value)
{
  write(key, std::string_view(value));
}

void
JsonPrettyWriterImpl::write(std::string_view key, std::string_view value)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(key);
  m_out << ": ";
  write_quoted_string(value);
  write_separator();
}

void
JsonPrettyWriterImpl::write(std::string_view key, std::span<bool const> values)
{
  write_indent();
  write_quoted_string(key);
  m_out << ": [";
  size_t index = 0;
  for(auto const& value : values) {
    m_out << (value ? "true" : "false");
    if (index != values.size() - 1) {
      m_out << ", ";
    }
    index += 1;
  }
  m_out << "]";
  write_separator();
}

void
JsonPrettyWriterImpl::write(std::string_view key, std::span<int const> values)
{
  write_indent();
  write_quoted_string(key);
  m_out << ": [";
  for(auto const& value : values) {
    m_out << value;
    if (&value != &values.back()) {
      m_out << ", ";
    }
  }
  m_out << "]";
  write_separator();
}

void
JsonPrettyWriterImpl::write(std::string_view key, std::span<float const> values)
{
  write_indent();
  write_quoted_string(key);
  m_out << ": [";
  for(auto const& value : values) {
    m_out << value;
    if (&value != &values.back()) {
      m_out << ", ";
    }
  }
  m_out << "]";
  write_separator();
}

void
JsonPrettyWriterImpl::write(std::string_view key, std::span<std::string const> values)
{
  write_indent();
  write_quoted_string(key);
  m_out << ": [";
  for(auto const& value : values) {
    write_quoted_string(value);
    if (&value != &values.back()) {
      m_out << ", ";
    }
  }
  m_out << "]";
  write_separator();
}

void
JsonPrettyWriterImpl::write_indent()
{
  if (m_write_seperator.back())
  {
    m_out.write(",\n", 2);
    m_write_seperator.back() = false;
  }
  else
  {
    m_out.write("\n", 1);
  }

  for(int i = 0; i < m_depth; ++i)
  {
    m_out.write("  ", 2);
  }
}

void
JsonPrettyWriterImpl::write(std::string_view key, std::vector<bool> const& values)
{
  write_indent();
  write_quoted_string(key);
  m_out << ": [";
  size_t index = 0;
  for(auto const&& value : values) {
    m_out << (value ? "true" : "false");
    if (index != values.size() - 1) {
      m_out << ", ";
    }
    index += 1;
  }
  m_out << "]";
  write_separator();
}

void
JsonPrettyWriterImpl::write_separator()
{
  m_write_seperator.back() = true;
}

void
JsonPrettyWriterImpl::write_quoted_string(std::string_view text)
{
  m_out << '"';
  for(char const c : text) {
    switch(c) {
      case '"':
        m_out << "\\\"";
        break;

      case '\\':
        m_out << "\\\\";
        break;

      default:
        m_out << c;
        break;
    }
  }
  m_out << '"';
}

} // namespace prio

/* EOF */
