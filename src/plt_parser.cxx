//  $Id: plt_parser.cxx,v 1.7 2002/12/29 23:29:00 torangan Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#include <stdio.h>
#include "pingus_error.hxx"
#include "plt_parser.hxx"
#include "my_gettext.hxx"

PLTParser::PLTParser()
{
  lineno = 0;
  theme_name = "-";
  in_header = true;
}
 
PLTParser::~PLTParser()
{
  
}

void
PLTParser::parse(std::string filename)
{
  std::string keyword;
  std::string value;

  in.open(filename.c_str());

  if (!in) 
    PingusError::raise("PLTParser: Cannot open: " + filename);
  
  try 
    {
      while (in_header) 
	{
	  keyword = get_string();
	  // cout << "Keyword: " << keyword << endl;
	  expect(':');
	  value = get_line();
	  // cout << "Value: " << value << endl;
	  add_pair(keyword, value);
	  check_newline();
	}
      
      // cout << " ---- " << endl;
      
      while (true) 
	{
	  std::string str = get_line();
	  levels.push_back(str);
	}
    }
  
  catch (PLTEOF) 
    {
      // cout << "PLT: EOF found" << endl;
    }
}

void
PLTParser::add_pair(std::string keyword, std::string value)
{
  if (keyword == "Name") {
    theme_name = value;
  } else if (keyword == "Image") {
    theme_image = value;
  } else if (keyword == "Description") {
    theme_description = value;
  } else if (keyword == "Background") {
    background_image = value;
  } else {
    char str[128];
    snprintf(str, 128, _("Unexpected keyword: '%s' at line: %d"), keyword.c_str(), lineno);
    PingusError::raise(str);
  }
}

char
PLTParser::get_char()
{
  char c;
  c = in.get();
  
  if (in.eof()) {
    throw PLTEOF();
  }

  if (c == '\n') {
    ++lineno;
  }

  return c;
}

void
PLTParser::jump_spaces()
{
  char c;
  while ((c = get_char()) == ' ' || c == '\t') {}
  
  in.putback(c);
}

void
PLTParser::expect(char ch)
{
  char c;
  
  jump_spaces();
  c = get_char();

  if (c != ch) {
    char str[48];
    snprintf(str, 48, "Unexpected '%c' at line: %d", c, lineno);
    PingusError::raise(str);
  }
  jump_spaces();
}

std::string
PLTParser::get_string()
{
  char c;
  std::string str;
  
  while (isalnum(c = get_char())) {
    str += c;
  }  
  in.putback(c);

  return str;
}

std::string
PLTParser::get_line()
{
  char c;
  std::string str;

  while ((c = get_char()) != '\n') {
    str += c;
  }

  return str;
}

std::vector<std::string>
PLTParser::get_levels()
{
  return levels;
}

std::string 
PLTParser::get_name()
{
  return theme_name;
}

std::string
PLTParser::get_description()
{
  return theme_description;
}

std::string 
PLTParser::get_surface()
{
  return theme_image;
}

ResDescriptor
PLTParser::get_background()
{
  return background_image;
}

void
PLTParser::check_newline()
{
  char c;
  c = get_char();
  if (c == '\n') {
    in_header = false;
  } else {
    in.putback(c);
  }
}

/* EOF */
