// Pingus - A free Lemmings clone
// Copyright (C) 2005 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_UTIL_SEXPR_READER_HPP
#define HEADER_PINGUS_UTIL_SEXPR_READER_HPP

#include <sexp/value.hpp>

#include "util/reader_impl.hpp"

class SExprReaderObjectImpl : public ReaderObjectImpl
{
public:
  SExprReaderObjectImpl(sexp::Value const& sx);
  virtual ~SExprReaderObjectImpl();

  std::string get_name() const override;
  ReaderMapping get_mapping() const override;

private:
  sexp::Value m_sx;
};

class SExprReaderCollectionImpl : public ReaderCollectionImpl
{
public:
  SExprReaderCollectionImpl(sexp::Value const& m_sx);
  virtual ~SExprReaderCollectionImpl();

  std::vector<ReaderObject> get_objects() const override;

private:
  sexp::Value m_sx;
};

class SExprReaderMappingImpl : public ReaderMappingImpl
{
public:
  SExprReaderMappingImpl(sexp::Value const& m_sx);
  virtual ~SExprReaderMappingImpl();

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
  sexp::Value const* get_subsection_item(const char* name) const;
  sexp::Value const* get_subsection(const char* name) const;

private:
  sexp::Value m_sx;
};

#endif

/* EOF */
