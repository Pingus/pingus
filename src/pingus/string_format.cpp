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

#include "pingus/string_format.hpp"

#include <sstream>

#include <strut/utf8.hpp>

#include "engine/display/font.hpp"

namespace pingus {

std::string
StringFormat::break_line(std::string const& text, int width, Font const& font)
{
  strut::utf8::iterator beg(text);
  float line_width = 0;
  std::ostringstream out;

  strut::utf8::iterator it(text);
  while(it.next())
  {
    std::string word = strut::utf8::substr(beg, it+1);
    float word_width = font.get_width(word);

    if (strut::utf8::is_linebreak_character(*it))
    {
      if ((line_width + word_width) > static_cast<float>(width))
      {
        out << "\n" << word;
        line_width = word_width;
      }
      else
      {
        out << word;
        line_width += word_width;
      }

      beg = it+1;
    }
    else if (*it == '\n')
    {
      line_width = 0;
    }
  }

  std::string word = text.substr(beg.get_index());
  float word_width = font.get_width(word);
  if ((line_width + word_width) > static_cast<float>(width))
  {
    out << "\n" << word;
  }
  else
  {
    out << word;
  }

  return out.str();
}

} // namespace pingus

/* EOF */
