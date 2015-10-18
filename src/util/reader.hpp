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

#ifndef HEADER_PINGUS_UTIL__READER_HPP
#define HEADER_PINGUS_UTIL__READER_HPP

#include <memory>
#include <vector>
#include <string>

class Size;
class Color;
class Vector3f;
class Vector2i;
class Rect;

class ResDescriptor;
class Pathname;

class ReaderObject;
class ReaderMapping;
class ReaderCollection;

class ReaderObjectImpl;
class ReaderMappingImpl;
class ReaderCollectionImpl;

class ReaderObject final
{
public:
  ReaderObject();
  ReaderObject(std::shared_ptr<ReaderObjectImpl> impl);

  std::string get_name() const;
  ReaderMapping get_mapping() const;

private:
  std::shared_ptr<ReaderObjectImpl> m_impl;
};

class ReaderCollection final
{
public:
  ReaderCollection();
  ReaderCollection(std::shared_ptr<ReaderCollectionImpl> impl);

  std::vector<ReaderObject> get_objects() const;

private:
  std::shared_ptr<ReaderCollectionImpl> m_impl;
};

class ReaderMapping final
{
public:
  ReaderMapping();
  ReaderMapping(std::shared_ptr<ReaderMappingImpl> impl);

  std::vector<std::string> get_keys() const;

  bool read_int(const char* key, int& value) const;
  bool read_float(const char* key, float& value) const;
  bool read_bool(const char* key, bool& value) const;
  bool read_string(const char* key, std::string& value) const;
  bool read_path(const char* key, Pathname& value) const;
  bool read_vector(const char* key, Vector3f& value) const;
  bool read_vectors(const char* key, std::vector<Vector3f>& value) const;
  bool read_vector2i(const char* key, Vector2i& value) const;
  bool read_rect(const char* key, Rect& value) const;
  bool read_size(const char* key, Size& value) const;
  bool read_colorf(const char* key, Color& value) const;
  bool read_colori(const char* key, Color& value) const;
  bool read_desc(const char* key, ResDescriptor& value) const;

  bool read_mapping(const char* key, ReaderMapping&) const;
  bool read_collection(const char* key, ReaderCollection&) const;
  bool read_object(const char* key, ReaderObject&) const;

  ReaderMapping read_mapping(const char* key) const;
  ReaderCollection read_collection(const char* key) const;
  ReaderObject read_object(const char* key) const;

  template<class E, class T>
  bool read_enum (const char* key, E& value, T enum2string) const
  {
    std::string str;
    if(read_string(key, str))
    {
      value = enum2string(str);
      return true;
    }

    return false;
  }

private:
  std::shared_ptr<ReaderMappingImpl> m_impl;
};

class Reader final
{
public:
  static ReaderObject parse(std::istream& stream);
  static ReaderObject parse(const std::string& filename);
  static ReaderObject parse(const Pathname& pathname);

  /** Reads multiple trees from a file, for use with files that don't
      contain a root element */
  static std::vector<ReaderObject> parse_many(const Pathname& pathname);

private:
  Reader() = delete;
};

#endif

/* EOF */
