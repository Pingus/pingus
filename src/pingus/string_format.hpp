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

#ifndef HEADER_PINGUS_PINGUS_STRING_FORMAT_HPP
#define HEADER_PINGUS_PINGUS_STRING_FORMAT_HPP

#include <string>

class Font;

class StringFormat {

public:
  /** Takes a string \a text and wraps it into multiple lines, each
      less then \a width long. Line wrappings happens TeX style, i.e.
      a double newline marks a newline, while other whitespace is
      joined to a single space. */
  static std::string break_line(const std::string& text, int width, const Font& font);
};

#endif

/* EOF */
