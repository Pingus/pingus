//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include <sstream>
#include "font.hpp"
#include "string_format.hpp"
#include "utf8_iterator.hpp"

std::string
StringFormat::break_line (std::string text, int width, const Font& font)
{
  std::string::size_type pos = 0;
  while ((pos = text.find('\t', pos)) != std::string::npos)
    text.replace(pos, 1, 1, ' ');

  pos = 0;
  while ((pos = text.find("  ", pos)) != std::string::npos)
    text.replace(pos, 2, 1, ' ');

  pos = 0;
  while ((pos = text.find('\n', pos)) != std::string::npos)
    {
      if (pos < text.length() && text[pos + 1] == '\n')   // double enter marks paragraph
      	{
          text.replace(pos, 2, 1, '\n');  		  // replace the two \n by one
	}
      else if (pos < text.length() - 1 && text[pos + 1] == ' ' && text[pos + 2] == '\n')
        {
          text.replace(pos, 3, 1, '\n');		  // whitespace between the two \n doesn't matter
	}
      else
        {
          text.replace(pos, 1, 1, ' ');
	  continue;					  // no \n here anymore, so continue searching
        }

      if (pos && text[pos - 1] == ' ')
        text.replace(pos - 1, 2, 1, '\n');		  // no whitespace in front

      if (pos < text.length() && text[pos + 1] == ' ')
        text.replace(pos, 2, 1, '\n');			  // no whitespace behind

      ++pos;						  // we don't want to find it again
    }

  pos = 0;
  while ((pos = text.find("  ", pos)) != std::string::npos)
    text.replace(pos, 2, 1, ' ');

  // Text is now normalized, time to start breaking the lines
  std::string::const_iterator beg = text.begin();
  int line_width = 0;
  std::ostringstream out;
  for(std::string::const_iterator it = beg; it != text.end(); it = UTF8::advance(it))
    {
      std::string word(beg, UTF8::advance(it));
      int word_width = font.get_width(word);
      
      if (UTF8::is_linebreak_character(UTF8Iterator::decode_utf8(std::string(it, const_cast<const std::string&>(text).end()))))
        {
          if ((line_width + word_width) > width)
            {
              out << "\n" << word;
              line_width = 0;
            }
          else
            {
              out << word;
              line_width += word_width;
            }
          
          beg = UTF8::advance(it);
        }
    }
  
  std::string::const_iterator end = text.end();
  out << std::string(beg, end);

  return out.str();
}


/* EOF */
