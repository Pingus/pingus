//  $Id: string_reader.cxx,v 1.8 2003/04/19 10:23:18 torangan Exp $
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

#include <ClanLib/Core/System/system.h>
#include <ClanLib/Display/Input/inputbuffer.h>
#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/Font/font.h>
#include "../console.hxx"
#include "../pingus_resource.hxx"
#include "string_reader.hxx"

StringReader::StringReader()
{
  strings = 0;
  font = PingusResource::load_font("Fonts/courier_small", "fonts");
}

StringReader::StringReader(const std::string & d, const std::string & def)
{
  strings = 0;
  description = d;
  default_string = def;
  font =  PingusResource::load_font("Fonts/courier_small", "fonts");
}

StringReader::~StringReader()
{
}

void
StringReader::set_strings(std::list<std::string>* s)
{
  strings = s;
}

std::string
StringReader::read_string()
{
  finished = false;
  CL_InputBuffer keys;
  CL_Key key;

  CL_System::keep_alive();

  current_string = default_string;

  keys.clear();

  draw();

  finished = false;
  while (!finished)
    {
      CL_System::keep_alive();

      if (keys.peek_key().state != CL_Key::NoKey)
	{
	  key = keys.get_key();

	  if (key.state == CL_Key::Pressed)
	    {
	      switch (key.id)
		{
		case CL_KEY_ENTER:
		  finished = true;
		  break;
		case CL_KEY_ESCAPE:
		  finished = true;
		  current_string = "";
		  break;
		case CL_KEY_DELETE:
		case CL_KEY_BACKSPACE:
		  if (!current_string.empty())
		    current_string = current_string.substr(0, current_string.size() - 1);
		  break;
		case CL_KEY_TAB:
		case CL_KEY_SPACE:
		  complete_string();
		  break;
		default:
		  if (key.ascii > 0)
		    current_string += key.ascii;
		}
	      draw();
	    }
	}
    }
  keys.clear();

  return current_string;
}

void
StringReader::complete_string()
{
  int completions_counter = 0;
  std::string* completion;

  completions.clear();

  console << "\nCompletions:\n" <<   "~~~~~~~~~~~~" << std::endl;

  for(std::list<std::string>::iterator i = strings->begin(); i != strings->end(); ++i)
    {
      if (i->find(current_string) == 0)
	{
	  console << *i << std::endl;
	  ++completions_counter;
	  completion = &(*i);
	  completions.push_back(&(*i));
	}
    }

  if (completions_counter >= 1)
    {
      current_string = find_uniq();
    }
  // std::cout << "Searching finished" << std::endl;
}

std::string
StringReader::find_uniq()
{
  std::list<std::string*>::iterator i = completions.begin();
  std::string ret_string = **(completions.begin());

  while (i != completions.end())
    {
      ret_string = while_eq(ret_string, **i);
      ++i;
    }

  return ret_string;
}

std::string
StringReader::while_eq(const std::string& a, const std::string& b)
{
  std::string ret_string;

  for(std::string::size_type i = 0;
      i < a.size() && i < b.size() && a[i] == b[i];
      ++i)
    {
      ret_string += a[i];
    }

  return ret_string;
}

void
StringReader::draw()
{
  CL_Display::clear_display();
  font->print_left(20, 20, description.c_str());
  font->print_left(20, 40, current_string.c_str());
  Display::flip_display();
}


/* EOF */
