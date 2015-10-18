// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_UTIL_FILE_WRITER_HPP
#define HEADER_PINGUS_UTIL_FILE_WRITER_HPP

#include <string>
#include <memory>

class Vector2i;
class Vector3f;
class Size;
class Color;
class Pathname;
class WriterImpl;

class Writer final
{
public:
  static Writer json(std::ostream& out);
  static Writer fastjson(std::ostream& out);
  static Writer sexpr(std::ostream& out);

public:
  Writer(std::ostream& out);
  Writer(std::unique_ptr<WriterImpl> impl);
  Writer(Writer&& other);
  ~Writer();

  /** collections contain an ordered sequence of objects */
  void begin_collection(const char* name);
  void end_collection();

  /** write an object into a collection, objects start a mapping for
      object properties */
  void begin_object(const char* type);
  void end_object();

  /** mappings contain name/value pairs */
  void begin_mapping(const char* name);
  void end_mapping();

  /** write a name/value pair inside a mapping */
  void write_int(const char* name, int);
  void write_float(const char* name, float);
  void write_colorf(const char* name, const Color&);
  void write_colori(const char* name, const Color&);
  void write_bool(const char* name, bool);
  void write_string(const char* name, const std::string&);
  void write_vector(const char* name, const Vector3f&);
  void write_size(const char* name, const Size&);
  void write_vector2i(const char* name, const Vector2i&);
  void write_path(const char* name, const Pathname&);

  template<class E, class T>
  void write_enum(const char* name, E& value, T string2enum)
  {
    write_string(name, string2enum(value));
  }

private:
  std::unique_ptr<WriterImpl> m_impl;
};

#endif

/* EOF */
