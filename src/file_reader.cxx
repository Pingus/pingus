//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include "file_reader.hxx"
#include "file_reader_impl.hxx"

namespace Pingus {

FileReader::FileReader(CL_SharedPtr<FileReaderImpl> impl_)
  : impl(impl_)
{
}

FileReader::FileReader()
  : impl(0)
{
}

std::string
FileReader::get_name() const
{
  return impl->get_name();
}

bool
FileReader::read_int(const char* name, int& value) const
{
  return impl->read_int(name, value);
}

bool
FileReader::read_float (const char* name, float& value) const
{
  return impl->read_float(name, value);
}

bool
FileReader::read_bool  (const char* name, bool& value) const
{
  return impl->read_bool(name, value);
}

bool
FileReader::read_string(const char* name, std::string& value) const
{
  return impl->read_string(name, value);
}

bool
FileReader::read_vector(const char* name, Vector& value) const
{
  return impl->read_vector(name, value);
}

bool
FileReader::read_color(const char* name, CL_Colorf& value) const
{
  return impl->read_color(name, value);
}

bool
FileReader::read_desc(const char* name, ResDescriptor& desc) const
{
  return impl->read_desc(name, desc);
}

bool
FileReader::read_size  (const char* name, CL_Size& value) const
{
  return impl->read_size(name, value);
}

bool
FileReader::read_section(const char* name, FileReader& reader) const
{
  return impl->read_section(name, reader);
}

std::vector<std::string>
FileReader::get_section_names() const
{
  return impl->get_section_names();
}

std::vector<FileReader>
FileReader::get_sections() const
{
  return impl->get_sections();
}

FileReader
FileReader::read_section(const char* name)   const
{
  FileReader reader;
  read_section(name, reader);
  return reader;
}

} // namespace Pingus

/* EOF */
