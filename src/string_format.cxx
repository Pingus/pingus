//  $Id: string_format.cxx,v 1.1 2003/04/16 18:02:27 torangan Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <ClanLib/Display/Font/font.h>
#include "string_format.hxx"

std::string
StringFormat::break_line (std::string text, int length, CL_Font* font)
{
  unsigned int pos = 0;
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


  int start_pos      = 0;
  int previous_space = 0;
  pos = 0;
  while ((pos = text.find(' ', pos + 1)) != std::string::npos)
    {
      if (font->get_text_width(text.substr(start_pos, pos - start_pos)) > length)
        {
	  text[previous_space] = '\n';
	  start_pos = previous_space + 1;
	}
      else if (font->get_text_width(text.substr(start_pos, text.length())) <= length)
        break;

      previous_space = pos;
    }

  return text;
}

/* EOF */
