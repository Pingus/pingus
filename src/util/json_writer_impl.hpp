// Pingus - A free Lemmings clone
// Copyright (C) 1998-2015 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_UTIL_JSON_FILE_WRITER_HPP
#define HEADER_PINGUS_UTIL_JSON_FILE_WRITER_HPP

#include <functional>
#include <json/json.h>
#include <iosfwd>

#include "util/writer_impl.hpp"

class JsonWriterImpl final : public WriterImpl
{
private:
  std::ostream& m_out;
  Json::Value m_root;

  // jsoncpp does copy-by-value, even for arrays and objects, so we
  // have to use std::reference_wrapper<> instead of just Json::Value
  std::vector<std::reference_wrapper<Json::Value> > m_stack;

public:
  JsonWriterImpl(std::ostream& out);
  virtual ~JsonWriterImpl();

  void begin_collection(const char* name) override;
  void end_collection() override;

  void begin_object(const char* type) override;
  void end_object() override;

  void begin_mapping(const char* name) override;
  void end_mapping() override;

  void write_int(const char* name, int) override;
  void write_float(const char* name, float) override;
  void write_colorf(const char* name, const Color&) override;
  void write_colori(const char* name, const Color&) override;
  void write_bool(const char* name, bool) override;
  void write_string(const char* name, const std::string&) override;
  void write_vector(const char* name, const Vector3f&) override;
  void write_size(const char* name, const Size&) override;
  void write_vector2i(const char* name, const Vector2i&) override;
  void write_path(const char* name, const Pathname&) override;

private:
  void flush();

private:
  JsonWriterImpl(const JsonWriterImpl&) = delete;
  JsonWriterImpl& operator=(const JsonWriterImpl&) = delete;
};

#endif

/* EOF */
