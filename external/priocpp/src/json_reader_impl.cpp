// prio - Property I/O for C++
// Copyright (C) 2005-2020 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "json_reader_impl.hpp"

#include <stdexcept>

#include <fmt/ostream.h>
#include <json/writer.h>
#include <logmich/log.hpp>

#include "reader_collection.hpp"
#include "reader_error.hpp"
#include "reader_impl.hpp"
#include "reader_mapping.hpp"
#include "reader_object.hpp"

namespace prio {

JsonReaderDocumentImpl::JsonReaderDocumentImpl(Json::Value value, ErrorHandler error_handler,
                                               std::optional<std::string> filename) :
  m_value(std::move(value)),
  m_error_handler(error_handler),
  m_filename(std::move(filename)),
  m_parent(nullptr)
{
}

void
JsonReaderDocumentImpl::error(Json::Value const& json, std::string_view message) const
{
  error(m_error_handler, json, message);
}

void
JsonReaderDocumentImpl::error(ErrorHandler error_handler, Json::Value const& json, std::string_view message) const
{
  switch (error_handler) {
    case ErrorHandler::THROW:
      throw ReaderError(fmt::format("{}: {}: {}", m_filename ? *m_filename : "<unknown>", fmt::streamed(json), message));

    case ErrorHandler::LOG:
      log_error("{}: {}: {}", m_filename ? *m_filename : "<unknown>", fmt::streamed(json), message);
      break;

    case ErrorHandler::IGNORE:
      break;
  }
}

ReaderObject
JsonReaderDocumentImpl::get_root() const
{
  return ReaderObject(std::make_unique<JsonReaderObjectImpl>(*this, m_value));
}

JsonReaderObjectImpl::JsonReaderObjectImpl(JsonReaderDocumentImpl const& doc, Json::Value const& json) :
  m_doc(doc),
  m_json(json)
{
  if (!m_json.isObject() || m_json.size() != 1)
  {
    m_doc.error(m_json, "expected hash with one element");
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
  return ReaderMapping(std::make_unique<JsonReaderMappingImpl>(m_doc, *it));
}


JsonReaderCollectionImpl::JsonReaderCollectionImpl(JsonReaderDocumentImpl const& doc, Json::Value const& json) :
  m_doc(doc),
  m_json(json)
{
  if (!m_json.isArray())
  {
    m_doc.error(json, "expected array");
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
    result.push_back(ReaderObject(std::make_unique<JsonReaderObjectImpl>(m_doc, m_json[i])));
  }
  return result;
}


JsonReaderMappingImpl::JsonReaderMappingImpl(JsonReaderDocumentImpl const& doc, Json::Value const& json) :
  m_doc(doc),
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

#define GET_VALUE_MACRO(type, checker, getter)                  \
  const Json::Value& element = m_json[std::string(key)];        \
  if (element.isNull()) { return false; }                       \
  if (!element.checker()) {                                     \
    m_doc.error(element, "expected " type);                     \
    return false;                                               \
  }                                                             \
  value = element.getter();                                     \
  return true

  bool
  JsonReaderMappingImpl::read(std::string_view key, bool& value) const
  {
    GET_VALUE_MACRO("bool", isBool, asBool);
  }

bool
JsonReaderMappingImpl::read(std::string_view key, int& value) const
{
  GET_VALUE_MACRO("int", isInt, asInt);
}

bool
JsonReaderMappingImpl::read(std::string_view key, float& value) const
{
  GET_VALUE_MACRO("double", isDouble, asFloat);
}

bool
JsonReaderMappingImpl::read(std::string_view key, std::string& value) const
{
  GET_VALUE_MACRO("string", isString, asString);
}

#undef GET_VALUE_MACRO

#define GET_VALUES_MACRO(type_, checker_, getter_)              \
  const Json::Value& element = m_json[std::string(key)];        \
  if (element.isNull()) { return false; }                       \
  if (!element.isArray()) {                                     \
    m_doc.error(element, "expected array");                     \
    return false;                                               \
  }                                                             \
                                                                \
  for(Json::Value::ArrayIndex i = 0; i < element.size(); ++i) { \
    if (!element[i].checker_()) {                               \
      m_doc.error(element[i], "expected " type_);               \
      return false;                                             \
    }                                                           \
  }                                                             \
                                                                \
  values.resize(element.size());                                \
  for(Json::Value::ArrayIndex i = 0; i < element.size(); ++i) { \
    values[i] = element[i].getter_();                           \
  }                                                             \
  return true

  bool
  JsonReaderMappingImpl::read(std::string_view key, std::vector<bool>& values) const
  {
    GET_VALUES_MACRO("bool", isBool, asBool);
  }

bool
JsonReaderMappingImpl::read(std::string_view key, std::vector<int>& values) const
{
  GET_VALUES_MACRO("int", isInt, asInt);
}

bool
JsonReaderMappingImpl::read(std::string_view key, std::vector<float>& values) const
{
  GET_VALUES_MACRO("double", isDouble, asFloat);
}

bool
JsonReaderMappingImpl::read(std::string_view key, std::vector<std::string>& values) const
{
  GET_VALUES_MACRO("string", isString, asString);
}

#undef GET_VALUES_MACRO

bool
JsonReaderMappingImpl::read(std::string_view key, ReaderMapping& value) const
{
  const Json::Value& element = m_json[std::string(key)];
  if (element.isObject())
  {
    value = ReaderMapping(std::make_unique<JsonReaderMappingImpl>(m_doc, element));
    return true;
  }
  else
  {
    return false;
  }
}

bool
JsonReaderMappingImpl::read(std::string_view key, ReaderCollection& value) const
{
  const Json::Value& element = m_json[std::string(key)];
  if (element.isArray())
  {
    value = ReaderCollection(std::make_unique<JsonReaderCollectionImpl>(m_doc, element));
    return true;
  }
  else
  {
    return false;
  }
}

bool
JsonReaderMappingImpl::read(std::string_view key, ReaderObject& value) const
{
  const Json::Value& element = m_json[std::string(key)];
  if (element.isObject())
  {
    value = ReaderObject(std::make_unique<JsonReaderObjectImpl>(m_doc, element));
    return true;
  }
  else
  {
    return false;
  }
}

void
JsonReaderMappingImpl::error(std::string_view key, std::string_view message) const
{
  m_doc.error(m_json, message);
}

void
JsonReaderMappingImpl::missing_key_error(std::string_view key) const
{
  std::ostringstream oss;
  oss << "required key not found: " << key;
  m_doc.error(ErrorHandler::THROW, m_json, oss.str());
}

} // namespace prio

/* EOF */
