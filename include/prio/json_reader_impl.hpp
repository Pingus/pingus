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

#ifndef HEADER_PRIO_JSON_READER_IMPL_HPP
#define HEADER_PRIO_JSON_READER_IMPL_HPP

#include "reader_impl.hpp"

#include <assert.h>
#include <json/value.h>

#include "error_handler.hpp"

namespace prio {

class JsonReaderDocumentImpl final : public ReaderDocumentImpl
{
public:
  JsonReaderDocumentImpl(Json::Value value, ErrorHandler error_handler, std::optional<std::string> filename);

  ReaderObject get_root() const override;
  std::optional<std::string> get_filename() const override { return m_filename; }
  void set_parent(ReaderDocument const* parent) override { m_parent = parent; }
  ReaderDocument const& get_parent() const { assert(m_parent != nullptr); return *m_parent; }

  void error(Json::Value const& json, std::string_view message) const;
  void error(ErrorHandler error_handler, Json::Value const& json, std::string_view message) const;

private:
  Json::Value m_value;
  ErrorHandler m_error_handler;
  std::optional<std::string> m_filename;
  ReaderDocument const* m_parent;
};

class JsonReaderObjectImpl final : public ReaderObjectImpl
{
public:
  JsonReaderObjectImpl(JsonReaderDocumentImpl const& doc, Json::Value const& json);
  ~JsonReaderObjectImpl() override;

  JsonReaderDocumentImpl const& get_document() const override { return m_doc; }
  std::string get_name() const override;
  ReaderMapping get_mapping() const override;

private:
  JsonReaderDocumentImpl const& m_doc;
  Json::Value const& m_json;
};

class JsonReaderCollectionImpl final : public ReaderCollectionImpl
{
public:
  JsonReaderCollectionImpl(JsonReaderDocumentImpl const& doc, Json::Value const& json);
  ~JsonReaderCollectionImpl() override;

  JsonReaderDocumentImpl const& get_document() const override { return m_doc; }
  std::vector<ReaderObject> get_objects() const override;

private:
  JsonReaderDocumentImpl const& m_doc;
  Json::Value const& m_json;
};

class JsonReaderMappingImpl final : public ReaderMappingImpl
{
public:
  JsonReaderMappingImpl(JsonReaderDocumentImpl const& doc, Json::Value const& json);
  ~JsonReaderMappingImpl() override;

  JsonReaderDocumentImpl const& get_document() const override { return m_doc; }
  std::vector<std::string> get_keys() const override;

  bool read(std::string_view key, bool& value) const override;
  bool read(std::string_view key, int& value) const override;
  bool read(std::string_view key, float& value) const override;
  bool read(std::string_view key, std::string& value) const override;

  bool read(std::string_view key, std::vector<bool>& values) const override;
  bool read(std::string_view key, std::vector<int>& values) const override;
  bool read(std::string_view key, std::vector<float>& values) const override;
  bool read(std::string_view key, std::vector<std::string>& values) const override;

  bool read(std::string_view key, ReaderMapping& value) const override;
  bool read(std::string_view key, ReaderCollection& value) const override;
  bool read(std::string_view key, ReaderObject& value) const override;

  void error(std::string_view key, std::string_view message) const override;
  void missing_key_error(std::string_view key) const override;

private:
  JsonReaderDocumentImpl const& m_doc;
  Json::Value const& m_json;
};

} // namespace prio

#endif

/* EOF */
