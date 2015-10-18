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

#ifndef HEADER_PINGUS_UTIL_JSON_READER_IMPL_HPP
#define HEADER_PINGUS_UTIL_JSON_READER_IMPL_HPP

#include "util/reader_impl.hpp"

#include <json/value.h>

class JsonReaderObjectImpl final : public ReaderObjectImpl
{
public:
  JsonReaderObjectImpl(Json::Value const& value);
  virtual ~JsonReaderObjectImpl();

  std::string get_name() const override;
  ReaderMapping get_mapping() const override;

private:
  Json::Value m_json;
};

class JsonReaderCollectionImpl final : public ReaderCollectionImpl
{
public:
  JsonReaderCollectionImpl(Json::Value const& value);
  virtual ~JsonReaderCollectionImpl();

  std::vector<ReaderObject> get_objects() const override;

private:
  Json::Value m_json;
};

class JsonReaderMappingImpl final : public ReaderMappingImpl
{
public:
  JsonReaderMappingImpl(Json::Value const& value);
  virtual ~JsonReaderMappingImpl();

  std::vector<std::string> get_keys() const override;

  bool read_int(const char* key, int&) const override;
  bool read_float(const char* key, float&) const override;
  bool read_bool(const char* key, bool&) const override;
  bool read_string(const char* key, std::string&) const override;
  bool read_vector(const char* key, Vector3f&) const override;
  bool read_vectors(const char* key, std::vector<Vector3f>&) const override;
  bool read_size(const char* key, Size&) const override;
  bool read_vector2i(const char* key, Vector2i&) const override;
  bool read_rect(const char* key, Rect&) const override;
  bool read_colorf(const char* key, Color&) const override;
  bool read_colori(const char* key, Color&) const override;

  bool read_mapping(const char* key, ReaderMapping&) const override;
  bool read_collection(const char* key, ReaderCollection&) const override;
  bool read_object(const char* key, ReaderObject&) const override;

private:
  Json::Value m_json;
};

#endif

/* EOF */
