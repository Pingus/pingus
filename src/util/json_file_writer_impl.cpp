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

#include <ostream>
#include <assert.h>

#include "math/color.hpp"
#include "math/size.hpp"
#include "math/vector2i.hpp"
#include "util/pathname.hpp"

JsonFileWriterImpl::JsonFileWriterImpl(std::ostream& out) :
  m_out(out),
  m_stack()
{
  // the root element is always a mapping
  Json::Value mapping(Json::objectValue);
  m_stack.push_back(mapping);
}

JsonFileWriterImpl::~JsonFileWriterImpl()
{
}

void
JsonFileWriterImpl::begin_collection(const char* name)
{
  assert(!m_stack.empty());
  assert(m_stack.back().type() == Json::objectValue);

  Json::Value child(Json::arrayValue);
  m_stack.back()[name] = child;
  m_stack.push_back(child);
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

  if (m_stack.back().type() == Json::objectValue)
  {
    Json::Value child(Json::objectValue);
    m_stack.back()[name] = child;
    m_stack.push_back(child);
  }
  else if (m_stack.back().type() == Json::arrayValue)
  {
    Json::Value parent(Json::objectValue);
    Json::Value child(Json::objectValue);
    parent[name] = child;
    m_stack.back()[name] = parent;
    m_stack.push_back(child);
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

  if (m_stack.size() == 1)
  {
    flush();
    m_stack.pop_back();
  }

  m_stack.pop_back();
}

void
JsonFileWriterImpl::write_int(const char* name, int value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().type() == Json::objectValue);

  m_stack.back()[name] = Json::Value(value);
}

void
JsonFileWriterImpl::write_float(const char* name, float value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().type() == Json::objectValue);

  m_stack.back()[name] = Json::Value(value);
}

void
JsonFileWriterImpl::write_colorf(const char* name, const Color& value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().type() == Json::objectValue);

  Json::Value array(Json::arrayValue);
  array[0] = Json::Value(value.r / 255.0f);
  array[1] = Json::Value(value.g / 255.0f);
  array[2] = Json::Value(value.b / 255.0f);
  array[3] = Json::Value(value.a / 255.0f);
  m_stack.back()[name] = array;
}

void
JsonFileWriterImpl::write_colori(const char* name, const Color& value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().type() == Json::objectValue);

  Json::Value array(Json::arrayValue);
  array[0] = Json::Value(value.r);
  array[1] = Json::Value(value.g);
  array[2] = Json::Value(value.b);
  array[3] = Json::Value(value.a);
  m_stack.back()[name] = array;
}

void
JsonFileWriterImpl::write_bool(const char* name, bool value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().type() == Json::objectValue);

  m_stack.back()[name] = Json::Value(value);
}

void
JsonFileWriterImpl::write_string(const char* name, const std::string& value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().type() == Json::objectValue);

  m_stack.back()[name] = Json::Value(value);
}

void
JsonFileWriterImpl::write_vector(const char* name, const Vector3f& value)
{
}

void
JsonFileWriterImpl::write_size(const char* name, const Size& value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().type() == Json::objectValue);

  Json::Value array(Json::arrayValue);
  array[0] = Json::Value(value.width);
  array[1] = Json::Value(value.height);
  m_stack.back()[name] = array;
}

void
JsonFileWriterImpl::write_vector2i(const char* name, const Vector2i& value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().type() == Json::objectValue);

  Json::Value array(Json::arrayValue);
  array[0] = Json::Value(value.x);
  array[1] = Json::Value(value.y);
  m_stack.back()[name] = array;
}

void
JsonFileWriterImpl::write_path(const char* name, const Pathname& value)
{
  write_string(name, value.get_raw_path());
}

void
JsonFileWriterImpl::flush()
{
  assert(!m_stack.empty());
  Json::StyledStreamWriter writer("  ");
  writer.write(m_out, m_stack.back());
}

/* EOF */
