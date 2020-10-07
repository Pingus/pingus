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

#include "reader/json_reader_impl.hpp"

#include <logmich/log.hpp>

#include "math/color.hpp"
#include "math/rect.hpp"
#include "math/vector2f.hpp"
#include "pingus/res_descriptor.hpp"
#include "reader/reader.hpp"
#include "reader/reader_impl.hpp"

namespace {

void syntax_error(Json::Value const& value, const char* message)
{
  log_error("syntax error: {}", message);
}

} // namespace

JsonReaderObjectImpl::JsonReaderObjectImpl(Json::Value const& json) :
  m_json(json)
{
  if (!m_json.isObject() || m_json.size() != 1)
  {
    syntax_error(json, "expected hash with one element");
  }
}

JsonReaderObjectImpl::~JsonReaderObjectImpl()
{
}

std::string
JsonReaderObjectImpl::get_name() const
{
  auto it = m_json.begin();
  return it.key().asString();
}

ReaderMapping
JsonReaderObjectImpl::get_mapping() const
{
  auto it = m_json.begin();
  return ReaderMapping(std::make_shared<JsonReaderMappingImpl>(*it));
}


JsonReaderCollectionImpl::JsonReaderCollectionImpl(Json::Value const& json) :
  m_json(json)
{
  if (!m_json.isArray())
  {
    syntax_error(json, "expected array");
  }
}

JsonReaderCollectionImpl::~JsonReaderCollectionImpl()
{
}

std::vector<ReaderObject>
JsonReaderCollectionImpl::get_objects() const
{
  std::vector<ReaderObject> result;
  for(Json::ArrayIndex i = 0; i < m_json.size(); ++i)
  {
    result.push_back(ReaderObject(std::make_shared<JsonReaderObjectImpl>(m_json[i])));
  }
  return result;
}


JsonReaderMappingImpl::JsonReaderMappingImpl(Json::Value const& json) :
  m_json(json)
{
}

JsonReaderMappingImpl::~JsonReaderMappingImpl()
{
}

std::vector<std::string>
JsonReaderMappingImpl::get_keys() const
{
  std::vector<std::string> result;
  for(auto it = m_json.begin(); it != m_json.end(); ++it)
  {
    result.push_back(it.key().asString());
  }
  return result;
}

bool
JsonReaderMappingImpl::read_int(const char* key, int& value) const
{
  const Json::Value& element = m_json[key];
  if (element.isInt())
  {
    value = element.asInt();
    return true;
  }
  else
  {
    return false;
  }
}

bool
JsonReaderMappingImpl::read_float(const char* key, float& value) const
{
  const Json::Value& element = m_json[key];
  if (element.isDouble())
  {
    value = element.asFloat();
    return true;
  }
  else
  {
    return false;
  }
}

bool
JsonReaderMappingImpl::read_bool(const char* key, bool& value) const
{
  const Json::Value& element = m_json[key];
  if (element.isBool())
  {
    value = element.asBool();
    return true;
  }
  else
  {
    return false;
  }
}

bool
JsonReaderMappingImpl::read_string(const char* key, std::string& value) const
{
  const Json::Value& element = m_json[key];
  if (element.isString())
  {
    value = element.asString();
    return true;
  }
  else
  {
    return false;
  }
}

bool
JsonReaderMappingImpl::read_vectors(const char* key, std::vector<Vector2f>& values,
                                    std::vector<float>& z_indexes) const
{
  const Json::Value& element = m_json[key];
  if (element.isArray())
  {
    for(Json::ArrayIndex i = 0; i < element.size(); ++i)
    {
      const Json::Value& vec = element[i];
      if (vec.isArray() && vec.size() >= 3)
      {
        values.emplace_back(vec[0u].asFloat(),
                            vec[1u].asFloat());
        z_indexes.emplace_back(vec[2u].asFloat());
      }
      else
      {
        log_warn("ignoring element of '{}'", key);
      }
    }
    return true;
  }
  else
  {
    return false;
  }
}

bool
JsonReaderMappingImpl::read_vector(const char* key, Vector2f& value, float& z_index) const
{
  const Json::Value& element = m_json[key];
  if (element.isArray() && element.size() >= 3)
  {
    value = Vector2f(element[0u].asFloat(),
                     element[1u].asFloat());
    z_index = element[2u].asFloat();
    return true;
  }
  else
  {
    return false;
  }
}

bool
JsonReaderMappingImpl::read_size(const char* key, Size& value) const
{
  const Json::Value& element = m_json[key];
  if (element.isArray() && element.size() >= 2)
  {
    value = Size(element[0u].asInt(),
                 element[1u].asInt());
    return true;
  }
  else
  {
    return false;
  }
}

bool
JsonReaderMappingImpl::read_vector2i(const char* key, Vector2i& value) const
{
  const Json::Value& element = m_json[key];
  if (element.isArray() && element.size() >= 2)
  {
    value = Vector2i(element[0u].asInt(),
                     element[1u].asInt());
    return true;
  }
  else
  {
    return false;
  }
}

bool
JsonReaderMappingImpl::read_rect(const char* key, Rect& value) const
{
  const Json::Value& element = m_json[key];
  if (element.isArray() && element.size() >= 4)
  {
    value = Rect(element[0u].asInt(),
                 element[1u].asInt(),
                 element[2u].asInt(),
                 element[3u].asInt());
    return true;
  }
  else
  {
    return false;
  }
}

bool
JsonReaderMappingImpl::read_colorf(const char* key, Color& value) const
{
  const Json::Value& element = m_json[key];
  if (element.isArray() && element.size() >= 3)
  {
    value.r = static_cast<uint8_t>(255 * element[0u].asFloat());
    value.g = static_cast<uint8_t>(255 * element[1u].asFloat());
    value.b = static_cast<uint8_t>(255 * element[2u].asFloat());
    value.a = static_cast<uint8_t>(255 * element[3u].asFloat());
    return true;
  }
  else
  {
    return false;
  }
}

bool
JsonReaderMappingImpl::read_colori(const char* key, Color& value) const
{
  const Json::Value& element = m_json[key];
  if (element.isArray() && element.size() >= 3)
  {
    value.r = static_cast<uint8_t>(element[0u].asInt());
    value.g = static_cast<uint8_t>(element[1u].asInt());
    value.b = static_cast<uint8_t>(element[2u].asInt());
    value.a = static_cast<uint8_t>(element[3u].asInt());
    return true;
  }
  else
  {
    return false;
  }
}

bool
JsonReaderMappingImpl::read_mapping(const char* key, ReaderMapping& value) const
{
  const Json::Value& element = m_json[key];
  if (element.isObject())
  {
    value = ReaderMapping(std::make_shared<JsonReaderMappingImpl>(element));
    return true;
  }
  else
  {
    return false;
  }
}

bool
JsonReaderMappingImpl::read_collection(const char* key, ReaderCollection& value) const
{
  const Json::Value& element = m_json[key];
  if (element.isArray())
  {
    value = ReaderCollection(std::make_shared<JsonReaderCollectionImpl>(element));
    return true;
  }
  else
  {
    return false;
  }
}

bool
JsonReaderMappingImpl::read_object(const char* key, ReaderObject& value) const
{
  const Json::Value& element = m_json[key];
  if (element.isObject())
  {
    value = ReaderObject(std::make_shared<JsonReaderObjectImpl>(element));
    return true;
  }
  else
  {
    return false;
  }
}

/* EOF */
