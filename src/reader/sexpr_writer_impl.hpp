// Pingus - A free Lemmings clone
// Copyright (C) 2007 Jimmy Salmon
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

#ifndef HEADER_PINGUS_UTIL_SEXPR_FILE_WRITER_HPP
#define HEADER_PINGUS_UTIL_SEXPR_FILE_WRITER_HPP

#include <ostream>

#include "math/color.hpp"
#include "math/size.hpp"
#include "math/vector2f.hpp"
#include "reader/writer_impl.hpp"

class SExprWriterImpl : public WriterImpl
{
private:
  /** A reference to the output stream */
  std::ostream* out;
  size_t level;
  std::string indent() const;

public:
  SExprWriterImpl(std::ostream& out_);
  ~SExprWriterImpl() override;

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
  void write_vector(const char* name, const Vector2f&, float) override;
  void write_size(const char* name, const Size&) override;
  void write_vector2i(const char* name, const Vector2i&) override;
  void write_path(const char* name, const Pathname&) override;

  template<class E, class F>
  void write_enum(const char* name, E value, F enum2string)
  {
    (*out) << "\n" << indent() << "(" << name << " \"" << enum2string(value) << "\")";
  }

private:
  SExprWriterImpl(const SExprWriterImpl&);
  SExprWriterImpl& operator= (const SExprWriterImpl&);
};

#endif

/* EOF */
