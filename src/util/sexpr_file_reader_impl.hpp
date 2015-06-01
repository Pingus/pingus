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

#ifndef HEADER_PINGUS_UTIL_SEXPR_FILE_READER_HPP
#define HEADER_PINGUS_UTIL_SEXPR_FILE_READER_HPP

#include "lisp/lisp.hpp"
#include "util/file_reader_impl.hpp"

class SExprFileReaderImpl final: public FileReaderImpl
{
private:
  std::shared_ptr<lisp::Lisp> sexpr;

public:
  SExprFileReaderImpl(std::shared_ptr<lisp::Lisp> sexpr_);
  ~SExprFileReaderImpl();

  std::string get_name() const;

  bool read_int   (const char* name, int& v) const;
  bool read_float (const char* name, float& v) const;
  bool read_bool  (const char* name, bool& v) const;
  bool read_string(const char* name, std::string& v) const;
  bool read_vector(const char* name, Vector3f& v) const;
  bool read_size(const char* name, Size& v) const;
  bool read_vector2i(const char* name, Vector2i& v) const;
  bool read_rect(const char* name, Rect& rect) const;
  bool read_colorf(const char* name, Color& v) const;
  bool read_colori(const char* name, Color& v) const;
  bool read_desc  (const char* name, ResDescriptor& v) const;
  bool read_section(const char* name, FileReader& v) const;
  std::vector<FileReader> get_sections() const;
  std::vector<std::string> get_section_names() const;

private:
  std::shared_ptr<lisp::Lisp> get_subsection_item(const char* name) const;
  std::shared_ptr<lisp::Lisp> get_subsection(const char* name) const;
};

#endif

/* EOF */
