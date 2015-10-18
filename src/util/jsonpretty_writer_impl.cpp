// Pingus - A free Lemmings clone
// Copyright (C) 1998-2015 Ingo Ruhnke <grumbel@gmail.com>
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

#include "jsonpretty_writer_impl.hpp"

#include <sstream>
#include <ostream>
#include <assert.h>

#include "math/color.hpp"
#include "math/size.hpp"
#include "math/vector2i.hpp"
#include "math/vector3f.hpp"
#include "util/pathname.hpp"

JsonPrettyWriterImpl::JsonPrettyWriterImpl(std::ostream& out) :
  m_out(out),
  m_depth(0),
  m_write_seperator( { false } ),
  m_context( { Context::Collection } )
{
}

JsonPrettyWriterImpl::~JsonPrettyWriterImpl()
{
}

void
JsonPrettyWriterImpl::begin_collection(const char* name)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(name);
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
JsonPrettyWriterImpl::begin_object(const char* type)
{
  assert(m_context.back() == Context::Collection);

  if (m_depth != 0)
  {
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
JsonPrettyWriterImpl::begin_mapping(const char* name)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(name);
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
JsonPrettyWriterImpl::write_int(const char* name, int value)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(name);
  m_out << ": " << value;
  write_separator();
}

void
JsonPrettyWriterImpl::write_float(const char* name, float value)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(name);
  m_out << ": " << value;
  write_separator();
}

void
JsonPrettyWriterImpl::write_colorf(const char* name, const Color& value)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(name);
  m_out << ": [ "
        << static_cast<float>(value.r) / 255.0f << ", "
        << static_cast<float>(value.g) / 255.0f << ", "
        << static_cast<float>(value.b) / 255.0f << ", "
        << static_cast<float>(value.a) / 255.0f << " ]";
  write_separator();
}

void
JsonPrettyWriterImpl::write_colori(const char* name, const Color& value)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(name);
  m_out << ": [ "
        << static_cast<int>(value.r) << ", "
        << static_cast<int>(value.g) << ", "
        << static_cast<int>(value.b) << ", "
        << static_cast<int>(value.a) << " ]";
  write_separator();
}

void
JsonPrettyWriterImpl::write_bool(const char* name, bool value)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(name);
  m_out << ": " << (value ? "true" : "false");
  write_separator();
}

void
JsonPrettyWriterImpl::write_string(const char* name, const std::string& value)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(name);
  m_out << ": ";
  write_quoted_string(value);
  write_separator();
}

void
JsonPrettyWriterImpl::write_vector(const char* name, const Vector3f& value)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(name);
  m_out << ": [ " << value.x << ", " << value.y << ", " << value.z << " ]";
  write_separator();
}

void
JsonPrettyWriterImpl::write_size(const char* name, const Size& value)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(name);
  m_out << ": [ " << value.width << ", " << value.height << " ]";
  write_separator();
}

void
JsonPrettyWriterImpl::write_vector2i(const char* name, const Vector2i& value)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(name);
  m_out << ": [ " << value.x << ", " << value.y << " ]";
  write_separator();
}

void
JsonPrettyWriterImpl::write_path(const char* name, const Pathname& value)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(name);
  m_out << ": ";
  write_quoted_string(value.get_raw_path());
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
JsonPrettyWriterImpl::write_separator()
{
  m_write_seperator.back() = true;
}

void
JsonPrettyWriterImpl::write_quoted_string(const char* str)
{
  // FIXME: obviously evil
  m_out << '"' << str << '"';
}

void
JsonPrettyWriterImpl::write_quoted_string(const std::string& str)
{
  write_quoted_string(str.c_str());
}

/* EOF */
