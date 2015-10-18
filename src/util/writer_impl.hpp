//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_UTIL_FILE_WRITER_IMPL_HPP
#define HEADER_PINGUS_UTIL_FILE_WRITER_IMPL_HPP

#include <string>

class Vector2i;
class Vector3f;
class Size;
class Color;
class Pathname;

/** Interface to write out name/value pairs out of some kind of file or
    structure */
class WriterImpl
{
private:
public:
  virtual ~WriterImpl() {}

  virtual void begin_collection(const char* name) = 0;
  virtual void end_collection() = 0;

  virtual void begin_object(const char* type) = 0;
  virtual void end_object() = 0;

  virtual void begin_mapping(const char* name) = 0;
  virtual void end_mapping() = 0;

  virtual void write_int    (const char* name, int) = 0;
  virtual void write_float  (const char* name, float) = 0;
  virtual void write_colorf (const char* name, const Color&) = 0;
  virtual void write_colori (const char* name, const Color&) = 0;
  virtual void write_bool   (const char* name, bool) = 0;
  virtual void write_string (const char* name, const std::string&) = 0;
  virtual void write_vector (const char* name, const Vector3f&) = 0;
  virtual void write_size   (const char* name, const Size&) = 0;
  virtual void write_vector2i(const char* name, const Vector2i&) = 0;
  virtual void write_path   (const char* name, const Pathname&) = 0;
};

#endif

/* EOF */
