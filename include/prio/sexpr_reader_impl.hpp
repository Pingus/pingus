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

#ifndef HEADER_PRIO_SEXPR_READER_HPP
#define HEADER_PRIO_SEXPR_READER_HPP

#include <assert.h>
#include <sexp/value.hpp>

#include "error_handler.hpp"
#include "reader_impl.hpp"

namespace prio {

class SExprReaderDocumentImpl final : public ReaderDocumentImpl
{
public:
  SExprReaderDocumentImpl(sexp::Value sx, ErrorHandler error_handler,
                          std::optional<std::string> filename);

  ReaderObject get_root() const override;
  std::optional<std::string> get_filename() const override { return m_filename; }
  void set_parent(ReaderDocument const* parent) override { m_parent = parent; }
  ReaderDocument const& get_parent() const { assert(m_parent != nullptr); return *m_parent; }

  void error(sexp::Value const& sx, std::string_view message) const;
  void error(ErrorHandler error_handler, sexp::Value const& sx, std::string_view message) const;

  sexp::Value const& get_sx() const { return m_sx; }

private:
  sexp::Value m_sx;
  ErrorHandler m_error_handler;
  std::optional<std::string> m_filename;
  ReaderDocument const* m_parent;
};

class SExprReaderObjectImpl final : public ReaderObjectImpl
{
public:
  SExprReaderObjectImpl(SExprReaderDocumentImpl const& doc, sexp::Value const& sx);
  ~SExprReaderObjectImpl() override;

  SExprReaderDocumentImpl const& get_document() const override { return m_doc; }
  std::string get_name() const override;
  ReaderMapping get_mapping() const override;

  sexp::Value const& get_sx() const { return m_sx; }

private:
  SExprReaderDocumentImpl const& m_doc;
  sexp::Value const& m_sx;
};

class SExprReaderCollectionImpl final : public ReaderCollectionImpl
{
public:
  SExprReaderCollectionImpl(SExprReaderDocumentImpl const& doc, sexp::Value const& m_sx);
  ~SExprReaderCollectionImpl() override;

  SExprReaderDocumentImpl const& get_document() const override { return m_doc; }
  std::vector<ReaderObject> get_objects() const override;

  sexp::Value const& get_sx() const { return m_sx; }

private:
  SExprReaderDocumentImpl const& m_doc;
  sexp::Value const& m_sx;
};

class SExprReaderMappingImpl final : public ReaderMappingImpl
{
public:
  SExprReaderMappingImpl(SExprReaderDocumentImpl const& doc, sexp::Value const& m_sx);
  ~SExprReaderMappingImpl() override;

  SExprReaderDocumentImpl const& get_document() const override { return m_doc; }
  std::vector<std::string> get_keys() const override;

  bool read(std::string_view key, bool& value) const override;
  bool read(std::string_view key, int& value) const override;
  bool read(std::string_view key, float& value) const override;
  bool read(std::string_view key, std::string& value) const override;

  bool read(std::string_view key, std::vector<bool>& v) const override;
  bool read(std::string_view key, std::vector<int>& v) const override;
  bool read(std::string_view key, std::vector<float>& v) const override;
  bool read(std::string_view key, std::vector<std::string>& v) const override;

  bool read(std::string_view key, ReaderMapping& value) const override;
  bool read(std::string_view key, ReaderCollection& value) const override;
  bool read(std::string_view key, ReaderObject& value) const override;

  void error(std::string_view key, std::string_view message) const override;
  void missing_key_error(std::string_view key) const override;

  sexp::Value const& get_sx() const { return m_sx; }

private:
  sexp::Value const* get_subsection_item(std::string_view key) const;
  sexp::Value const* get_subsection_items(std::string_view key) const;
  sexp::Value const* get_subsection(std::string_view key) const;

private:
  SExprReaderDocumentImpl const& m_doc;
  sexp::Value const& m_sx;
};

} // namespace prio

#endif

/* EOF */
