//  $Id: string_reader.cxx,v 1.12 2004/04/02 18:13:00 grumbel Exp $
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
#include <ClanLib/display.h>
#include "../console.hxx"
#include "../resource.hxx"
#include "../fonts.hxx"
#include "string_reader.hxx"

namespace Pingus {
namespace EditorNS {

StringReader::StringReader()
{
  font = Fonts::courier_small;
}

StringReader::StringReader(const std::string & d, const std::string & def)
{
  description = d;
  default_string = def;
  font = Fonts::courier_small;
}

StringReader::~StringReader()
{
}

void
StringReader::set_strings(const std::list<std::string>& s)
{
  strings = s;
}

std::string
StringReader::read_string()
{
  finished = false;
  CL_InputBuffer keys;
  keys.add_device(CL_Keyboard::get_device());
  CL_InputEvent  event;

  CL_System::keep_alive();

  current_string = default_string;

  keys.clear();

  draw();

  finished = false;
  while (!finished)
    {
      CL_System::keep_alive();

      if (keys.peek_key().type != CL_InputEvent::no_key)
	{
	  event = keys.pop_key();

	  if (event.type == CL_InputEvent::pressed)
	    {
	      switch (event.id)
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
		  if (!event.str.empty())
		    current_string += event.str;
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

  for(std::list<std::string>::iterator i = strings.begin(); i != strings.end(); ++i)
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
  CL_Display::clear();
  font.draw(20, 20, description.c_str());
  font.draw(20, 40, current_string.c_str());
  Display::flip_display();
}

} // namespace EditorNS
} // namespace Pingus

/* EOF */
