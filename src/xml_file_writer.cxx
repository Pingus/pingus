//  $Id: xml_file_writer.cxx,v 1.1 2002/12/20 18:45:41 grumbel Exp $
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

#include "xml_file_writer.hxx"

XMLFileWriter::XMLFileWriter(std::ostream& out_)
  : out(&out_)
{
}

XMLFileWriter::~XMLFileWriter()
{
  
}

void
XMLFileWriter::begin_section (const char* name)
{
  (*out) << "<" << name << ">\n";
  section_stack.push(name);
}

void
XMLFileWriter::end_section ()
{
  const std::string& section_name = section_stack.top();

  (*out) << "</" << section_name << ">\n";

  section_stack.pop();
}

void
XMLFileWriter::write_int    (const char* name, int value)
{
  (*out) << "<" << name << ">" << value << "</" << name << ">\n";
}

void
XMLFileWriter::write_float  (const char* name, float value)
{
  (*out) << "<" << name << ">" << value << "</" << name << ">\n";
}

void
XMLFileWriter::write_bool   (const char* name, bool value)
{
  (*out) << "<" << name << ">" << value << "</" << name << ">\n";
}

void
XMLFileWriter::write_string (const char* name, const std::string& value)
{
  // FIXME: We don't handle escaping of characters here, might end
  // FIXME: ugly if people enter '<>' in there strings.
  (*out) << "<" << name << ">" << value << "</" << name << ">\n";
}

void
XMLFileWriter::write_vector (const char* name, const Vector& value)
{
  (*out) << "<" << name << ">\n"
         << "  <x-pos>" << value.x << "</x-pos>\n"
         << "  <y-pos>" << value.y << "</y-pos>\n"
         << "  <z-pos>" << value.z << "</z-pos>\n"
         << "</" << name << ">\n";
}

/* EOF */
