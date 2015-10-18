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

#ifndef HEADER_PINGUS_UTIL_READER_IMPL_HPP
#define HEADER_PINGUS_UTIL_READER_IMPL_HPP

#include <string>
#include <vector>

class Color;
class Rect;
class Size;
class Vector2i;
class Vector3f;

class ReaderCollection;
class ReaderMapping;
class ReaderObject;
class ResDescriptor;

class ReaderObjectImpl
{
public:
  virtual ~ReaderObjectImpl() {}

  virtual std::string get_name() const = 0;
  virtual ReaderMapping get_mapping() const = 0;
};

class ReaderCollectionImpl
{
public:
  virtual ~ReaderCollectionImpl() {}

  virtual std::vector<ReaderObject> get_objects() const = 0;
};

class ReaderMappingImpl
{
public:
  virtual ~ReaderMappingImpl() {}

  virtual std::vector<std::string> get_keys() const = 0;

  virtual bool read_int(const char* key, int&) const = 0;
  virtual bool read_float(const char* key, float&) const = 0;
  virtual bool read_bool(const char* key, bool&) const = 0;
  virtual bool read_string(const char* key, std::string&) const = 0;
  virtual bool read_vector(const char* key, Vector3f&) const = 0;
  virtual bool read_vectors(const char* key, std::vector<Vector3f>&) const = 0;
  virtual bool read_size(const char* key, Size&) const = 0;
  virtual bool read_vector2i(const char* key, Vector2i&) const = 0;
  virtual bool read_rect(const char* key, Rect&) const = 0;
  virtual bool read_colorf(const char* key, Color&) const = 0;
  virtual bool read_colori(const char* key, Color&) const = 0;

  virtual bool read_mapping(const char* key, ReaderMapping&) const = 0;
  virtual bool read_collection(const char* key, ReaderCollection&) const = 0;
  virtual bool read_object(const char* key, ReaderObject&) const = 0;
};

#endif

/* EOF */
