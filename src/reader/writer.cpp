//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2015 Ingo Ruhnke <grumbel@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "reader/writer.hpp"

#include "reader/sexpr_writer_impl.hpp"
#include "reader/json_writer_impl.hpp"
#include "reader/jsonpretty_writer_impl.hpp"

Writer
Writer::fastjson(std::ostream& out)
{
  return Writer(std::make_unique<JsonWriterImpl>(out));
}

Writer
Writer::json(std::ostream& out)
{
  return Writer(std::make_unique<JsonPrettyWriterImpl>(out));
}

Writer
Writer::sexpr(std::ostream& out)
{
  return Writer(std::make_unique<SExprWriterImpl>(out));
}

Writer::Writer(std::ostream& out) :
  m_impl(std::make_unique<SExprWriterImpl>(out))
{
}

Writer::Writer(std::unique_ptr<WriterImpl> impl) :
  m_impl(std::move(impl))
{
}

Writer::~Writer()
{
}

void
Writer::begin_collection(const char* name)
{
  m_impl->begin_collection(name);
}

void
Writer::end_collection()
{
  m_impl->end_collection();
}

void
Writer::begin_object(const char* type)
{
  m_impl->begin_object(type);
}

void
Writer::end_object()
{
  m_impl->end_object();
}

void
Writer::begin_mapping(const char* name)
{
  m_impl->begin_mapping(name);
}

void
Writer::end_mapping()
{
  m_impl->end_mapping();
}


void
Writer::write_int(const char* name, int value)
{
  m_impl->write_int(name, value);
}

void
Writer::write_float(const char* name, float value)
{
  m_impl->write_float(name, value);
}

void
Writer::write_colorf(const char* name, const Color& value)
{
  m_impl->write_colorf(name, value);
}

void
Writer::write_colori(const char* name, const Color& value)
{
  m_impl->write_colori(name, value);
}

void
Writer::write_bool(const char* name, bool value)
{
  m_impl->write_bool(name, value);
}

void
Writer::write_string(const char* name, const std::string& value)
{
  m_impl->write_string(name, value);
}

void
Writer::write_vector(const char* name, const Vector2f& value, float z_index)
{
  m_impl->write_vector(name, value, z_index);
}

void
Writer::write_size(const char* name, const Size& value)
{
  m_impl->write_size(name, value);
}

void
Writer::write_vector2i(const char* name, const Vector2i& value)
{
  m_impl->write_vector2i(name, value);
}

void
Writer::write_path(const char* name, const Pathname& value)
{
  m_impl->write_path(name, value);
}

/* EOF */
