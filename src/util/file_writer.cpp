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

#include "util/file_writer.hpp"

#include "util/sexpr_file_writer_impl.hpp"
#include "util/json_file_writer_impl.hpp"

FileWriter::FileWriter(std::ostream& out) :
  //m_impl(std::make_unique<SExprFileWriterImpl>(out))
  m_impl(std::make_unique<JsonFileWriterImpl>(out))
{
}

FileWriter::FileWriter(std::unique_ptr<FileWriterImpl> impl) :
  m_impl(std::move(impl))
{
}

FileWriter::~FileWriter()
{
}

void
FileWriter::begin_collection(const char* name)
{
  m_impl->begin_collection(name);
}

void
FileWriter::end_collection()
{
  m_impl->end_collection();
}

void
FileWriter::begin_object(const char* type)
{
  m_impl->begin_object(type);
}

void
FileWriter::end_object()
{
  m_impl->end_object();
}

void
FileWriter::begin_mapping(const char* name)
{
  m_impl->begin_mapping(name);
}

void
FileWriter::end_mapping()
{
  m_impl->end_mapping();
}


void
FileWriter::write_int(const char* name, int value)
{
  m_impl->write_int(name, value);
}

void
FileWriter::write_float(const char* name, float value)
{
  m_impl->write_float(name, value);
}

void
FileWriter::write_colorf(const char* name, const Color& value)
{
  m_impl->write_colorf(name, value);
}

void
FileWriter::write_colori(const char* name, const Color& value)
{
  m_impl->write_colori(name, value);
}

void
FileWriter::write_bool(const char* name, bool value)
{
  m_impl->write_bool(name, value);
}

void
FileWriter::write_string(const char* name, const std::string& value)
{
  m_impl->write_string(name, value);
}

void
FileWriter::write_vector(const char* name, const Vector3f& value)
{
  m_impl->write_vector(name, value);
}

void
FileWriter::write_size(const char* name, const Size& value)
{
  m_impl->write_size(name, value);
}

void
FileWriter::write_vector2i(const char* name, const Vector2i& value)
{
  m_impl->write_vector2i(name, value);
}

void
FileWriter::write_path(const char* name, const Pathname& value)
{
  m_impl->write_path(name, value);
}

/* EOF */
