// Pingus - A free Lemmings clone
// Copyright (C) 2007 Jimmy Salmon
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

#include "util/sexpr_writer_impl.hpp"

#include <map>

#include "util/pathname.hpp"

SExprWriterImpl::SExprWriterImpl(std::ostream& out_) :
  out(&out_),
  level(0)
{
}

SExprWriterImpl::~SExprWriterImpl()
{
}

std::string
SExprWriterImpl::indent()
{
  return std::string(level*2, ' ');
}

void
SExprWriterImpl::begin_collection(const char* name)
{
  begin_mapping(name);
}

void
SExprWriterImpl::end_collection()
{
  end_mapping();
}

void
SExprWriterImpl::begin_object(const char* type)
{
  begin_mapping(type);
}

void
SExprWriterImpl::end_object()
{
  end_mapping();
}

void
SExprWriterImpl::begin_mapping(const char* name)
{
  if (level != 0)
    (*out) << std::endl;

  (*out) << indent() << "(" << name;
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
SExprWriterImpl::write_int(const char* name, int value)
{
  (*out) << "\n" << indent() << "(" << name << " " << value << ")";
}

void
SExprWriterImpl::write_float(const char* name, float value)
{
  (*out) << "\n" << indent() << "(" << name << " " << value << ")";
}

void
SExprWriterImpl::write_colorf(const char* name, const Color& color)
{
  (*out) << "\n" << indent() << "(" << name << " "
         << static_cast<float>(color.r)/255.0f << " "
         << static_cast<float>(color.g)/255.0f << " "
         << static_cast<float>(color.b)/255.0f << " "
         << static_cast<float>(color.a)/255.0f << ")";
}

void
SExprWriterImpl::write_colori(const char* name, const Color& color)
{
  (*out) << "\n" << indent() << "(" << name << " "
         << static_cast<int>(color.r) << " "
         << static_cast<int>(color.g) << " "
         << static_cast<int>(color.b) << " "
         << static_cast<int>(color.a) << ")";
}

void
SExprWriterImpl::write_bool(const char* name, bool value)
{
  (*out) << "\n" << indent() << "(" << name << " " << (value ? "#t" : "#f") << ")";
}

void
SExprWriterImpl::write_string(const char* name, const std::string& value)
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
SExprWriterImpl::write_vector(const char* name, const Vector3f& value)
{
  (*out) << "\n" << indent() << "(" << name << " "
         << value.x << " " << value.y << " " << value.z << ")";
}

void
SExprWriterImpl::write_size(const char* name, const Size& size)
{
  (*out) << "\n" << indent() << "(" << name << " " << size.width << " " << size.height << ")";
}

void
SExprWriterImpl::write_vector2i(const char* name, const Vector2i& v)
{
  (*out) << "\n" << indent() << "(" << name << " " << v.x << " " << v.y << ")";
}

void
SExprWriterImpl::write_path(const char* name, const Pathname& path)
{
  write_string(name, path.get_raw_path());
}

/* EOF */
