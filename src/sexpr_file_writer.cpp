//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Jimmy Salmon
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

#include <iostream>
#include <string>
#include <map>
#include "sexpr_file_writer.hpp"

SExprFileWriter::SExprFileWriter(std::ostream& out_)
  : out(&out_), level(0)
{
}

SExprFileWriter::~SExprFileWriter()
{
}

std::string
SExprFileWriter::indent()
{
  return std::string(level*2, ' ');
}

void
SExprFileWriter::begin_section(const char* name)
{
  (*out) << "\n" << indent() << "(" << name << " ";
  ++level;
}

void
SExprFileWriter::end_section()
{
  --level;
  (*out) << ")";
}

void
SExprFileWriter::write_int(const char* name, int value)
{
  (*out) << "\n" << indent() << "(" << name << " " << value << ")";
}

void
SExprFileWriter::write_float(const char* name, float value)
{
  (*out) << "\n" << indent() << "(" << name << " " << value << ")";
}

void
SExprFileWriter::write_color(const char* name, const Color& color)
{
  (*out) << "\n" << indent() << "(" << name << " "
         << (int)color.r << " " << (int)color.g << " " << (int)color.b << " " << (int)color.a << ")";
}

void
SExprFileWriter::write_bool(const char* name, bool value)
{
  (*out) << "\n" << indent() << "(" << name << " " << (value ? "#t" : "#f") << ")";
}

void
SExprFileWriter::write_string(const char* name, const std::string& value)
{
	// Perform basic XML encoding (turns apostrophes into &apos;, etc.
	std::string new_value = value;
	std::string::size_type pos;

	std::map<std::string, std::string> replacements;
	
	replacements["\""] = "\\\"";
	replacements["\\"] = "\\\\";

	for (std::map<std::string, std::string>::iterator i = replacements.begin();
		i != replacements.end(); i++)
	{
		for (pos = new_value.find(i->first); pos != std::string::npos; pos = new_value.find(i->first))
		{
			// Replace character with encoding characters
			new_value.replace(pos, 1, i->second);
		}
	}
	
	(*out) << "\n" << indent() << "(" << name << " \"" << new_value << "\")";
}

void
SExprFileWriter::write_vector(const char* name, const Vector3f& value)
{
  (*out) << "\n" << indent() << "(" << name << " "
         << value.x << " " << value.y << " " << value.z << ")";
}

void
SExprFileWriter::write_size(const char* name, const Size& size)
{
  (*out) << "\n" << indent() << "(" << name << " " << size.width << " " << size.height << ")";
}

/* EOF */
