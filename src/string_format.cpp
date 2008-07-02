
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

#include "font.hpp"
#include "string_format.hpp"


std::string
StringFormat::break_line (std::string text, int length, const Font& font)
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

  size_t start_pos      = 0;
  size_t previous_space = 0;
  pos = 0;

  while ((pos = text.find(' ', pos + 1)) != std::string::npos)
    {
      if (font.get_width(text.substr(start_pos, pos - start_pos)) > length)
        {
	  text[previous_space] = '\n';
	  start_pos = previous_space + 1;
	}
      else if (font.get_width(text.substr(start_pos, text.length())) <= length)
        break;

      previous_space = pos;
    }

  if (font.get_width(text.substr(start_pos, text.length() - start_pos)) > length)
    text[text.rfind(' ')] = '\n';

  return text;
}


/* EOF */
