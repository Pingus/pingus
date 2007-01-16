//  $Id: multiline_text.cxx,v 1.10 2003/10/21 21:37:06 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include <assert.h>
#include "font.hpp"
#include "multiline_text.hxx"
#include "math.hxx"


MultiLineText::MultiLineText()
{
  width = -1;
  height = -1;
}

void
MultiLineText::set_font(Font f)
{
  font = f;
}

void
MultiLineText::set_text(const std::string& str, int text_width)
{
#ifdef CLANLIB_0_6
  int last_pos;

  assert(font);

  for(unsigned int j=0; j < str.length(); )
    {
      last_pos = str.length();

      for(unsigned int i=j; /* i < (text_width + j) */
	  font->get_text_width(str.substr(j, i-j).c_str()) < text_width;
	  ++i)
	{
	  if (i >= str.length())
	    {
	      last_pos = str.length();
	      break;
	    }
	  else if (str[i] == ' ')
	    {
	      last_pos = i;
	    }
	  else if (str[i] == '\n')
	    {
	      last_pos = i;
	      break;
	    }
	}
      text.push_back(str.substr(j, last_pos - j));
      j = last_pos + 1;
    }

  //cout << "MultiLineText: " << str << endl;
  /*
  cout << "MultiLineText: " << endl;
  for(std::vector<std::string>::iterator i = text.begin(); i != text.end(); ++i)
    cout << "  " << *i << endl;
  */
  width = 0;
  for(std::vector<std::string>::iterator i = text.begin(); i != text.end(); ++i)
	  width = Math::max(width, font->get_text_width(i->c_str()));

  height = text.size() * font->get_height();
#endif
}

void
MultiLineText::print_left(int x_pos, int y_pos)
{
  int y_inc = 0;

  for(std::vector<std::string>::iterator i = text.begin();
      i != text.end();
      ++i)
    {
      font.draw(x_pos, y_pos + y_inc, i->c_str());
      y_inc += font.get_height();
    }
}

void
MultiLineText::print_right(int x_pos, int y_pos)
{
#ifdef CLANLIB_0_6
  int y_inc = 0;

  for(std::vector<std::string>::iterator i = text.begin();
      i != text.end();
      ++i)
    {
      font->print_right(x_pos, y_pos + y_inc, i->c_str());
      y_inc += font->get_height();
    }
#endif
}

void
MultiLineText::print_center(int x_pos, int y_pos)
{
  int y_inc = 0;

  Font myfont = font;
  myfont.set_alignment(origin_top_center);

  for(std::vector<std::string>::iterator i = text.begin();
      i != text.end();
      ++i)
    {
      myfont.draw(x_pos, y_pos + y_inc, i->c_str());
      y_inc += font.get_height();
    }
}

int
MultiLineText::get_width()
{
  return width;
}

int
MultiLineText::get_height()
{
  return height;
}


/* EOF */

