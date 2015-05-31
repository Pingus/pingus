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

#include "json_file_writer_impl.hpp"

#include <sstream>
#include <ostream>
#include <assert.h>

#include "math/color.hpp"
#include "math/size.hpp"
#include "math/vector2i.hpp"
#include "util/pathname.hpp"

JsonFileWriterImpl::JsonFileWriterImpl(std::ostream& out) :
  m_out(out),
  m_root(Json::objectValue),
  m_stack()
{
  // the root element is always an anonymous mapping
  m_stack.push_back(m_root);
}

JsonFileWriterImpl::~JsonFileWriterImpl()
{
  assert(m_stack.size() == 1);
}

void
JsonFileWriterImpl::begin_collection(const char* name)
{
  assert(!m_stack.empty());
  assert(m_stack.back().get().type() == Json::objectValue);

  m_stack.push_back((m_stack.back().get()[name] = Json::Value(Json::arrayValue)));
}

void
JsonFileWriterImpl::end_collection()
{
  assert(!m_stack.empty());
  m_stack.pop_back();
}

void
JsonFileWriterImpl::begin_mapping(const char* name)
{
  assert(!m_stack.empty());

  if (m_stack.back().get().type() == Json::objectValue)
  {
    m_stack.push_back((m_stack.back().get()[name] = Json::Value(Json::objectValue)));
  }
  else if (m_stack.back().get().type() == Json::arrayValue)
  {
    m_stack.back().get().append(Json::Value(Json::objectValue));
    m_stack.push_back(m_stack.back().get()[m_stack.back().get().size()-1][name] = Json::Value(Json::objectValue));
  }
  else
  {
    assert(!"never reached");
  }
}

void
JsonFileWriterImpl::end_mapping()
{
  assert(!m_stack.empty());
  m_stack.pop_back();

  if (m_stack.size() == 1)
  {
    flush();
  }
}

void
JsonFileWriterImpl::write_int(const char* name, int value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().get().type() == Json::objectValue);

  m_stack.back().get()[name] = Json::Value(value);
}

void
JsonFileWriterImpl::write_float(const char* name, float value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().get().type() == Json::objectValue);

  m_stack.back().get()[name] = Json::Value(value);
}

void
JsonFileWriterImpl::write_colorf(const char* name, const Color& value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().get().type() == Json::objectValue);

  Json::Value array(Json::arrayValue);
  array.append(Json::Value(value.r / 255.0f));
  array.append(Json::Value(value.g / 255.0f));
  array.append(Json::Value(value.b / 255.0f));
  array.append(Json::Value(value.a / 255.0f));
  m_stack.back().get()[name] = array;
}

void
JsonFileWriterImpl::write_colori(const char* name, const Color& value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().get().type() == Json::objectValue);

  Json::Value array(Json::arrayValue);
  array.append(Json::Value(value.r));
  array.append(Json::Value(value.g));
  array.append(Json::Value(value.b));
  array.append(Json::Value(value.a));
  m_stack.back().get()[name] = array;
}

void
JsonFileWriterImpl::write_bool(const char* name, bool value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().get().type() == Json::objectValue);

  m_stack.back().get()[name] = Json::Value(value);
}

void
JsonFileWriterImpl::write_string(const char* name, const std::string& value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().get().type() == Json::objectValue);

  m_stack.back().get()[name] = Json::Value(value);
}

void
JsonFileWriterImpl::write_vector(const char* name, const Vector3f& value)
{
}

void
JsonFileWriterImpl::write_size(const char* name, const Size& value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().get().type() == Json::objectValue);

  Json::Value array(Json::arrayValue);
  array.append(Json::Value(value.width));
  array.append(Json::Value(value.height));
  m_stack.back().get()[name] = array;
}

void
JsonFileWriterImpl::write_vector2i(const char* name, const Vector2i& value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().get().type() == Json::objectValue);

  Json::Value array(Json::arrayValue);
  array.append(Json::Value(value.x));
  array.append(Json::Value(value.y));
  m_stack.back().get()[name] = array;
}

void
JsonFileWriterImpl::write_path(const char* name, const Pathname& value)
{
  write_string(name, value.get_raw_path());
}

namespace {

void strip_trailing_whitespace(std::ostream& out, std::istream& in)
{
  std::string line;
  while(std::getline(in, line))
  {
    const auto p = line.find_last_not_of(' ');
    if (p == std::string::npos)
    {
      if (line.empty())
      {
        out << line << '\n';
      }
    }
    else
    {
      out.write(line.data(), p + 1);
      out << '\n';
    }
  }
}

} // namespace

void
JsonFileWriterImpl::flush()
{
  assert(m_stack.size() == 1);
  std::stringstream out;
  Json::StyledStreamWriter writer("  ");
  writer.write(out, m_stack.back());
  strip_trailing_whitespace(m_out, out);
}

/* EOF */
