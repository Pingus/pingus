//  $Id: Console.cc,v 1.2 2000/06/13 22:19:17 grumbel Exp $
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

#include <algorithm>
#include <cstdio>
#include "PingusResource.hh"
#include "Console.hh"

// Globale console
Console console;

Console::Console()
{
  is_init = false;
  is_visible = false;
  current_pos = 0;
  number_of_lines = 10;
}

Console::~Console()
{
}

// We are not initialising the console in the constructor, 'cause
// that doesn't work (ClanLib hasn't init the display at that point) void
void
Console::init()
{
  std::cout << "Console: Init..." << std::endl;
  font = CL_Font::load("Fonts/pingus_small", PingusResource::get("fonts.dat"));
  this->println("Console V0.2");
  is_init = true;
}

void
Console::draw()
{
  assert(is_init);

  CL_Display::fill_rect(0, 0, CL_Display::get_width(), font->get_height() * 11,
			1.0, 1.0, 1.0, 0.5);
  for(unsigned int i = max(0, current_pos - number_of_lines), j=1;
      i < output_buffer.size(); 
      i++, j++)
    {
      font->print_left(10, j * font->get_height(), output_buffer[i].c_str());
    }
  font->print_left(10, (number_of_lines + 1) * font->get_height(),
		   current_line.c_str());
}

void
Console::print(char* format, ...) 
{
  char str_buffer[128];
  va_list argp;

  va_start(argp, format);
  vsnprintf(str_buffer, 128, format, argp);
  va_end(argp);

  output_buffer.push_back(str_buffer);
  current_pos++;
}

void
Console::println(char* format, ...)
{
  char str_buffer[128];
  va_list argp;

  va_start(argp, format);
  vsnprintf(str_buffer, 128, format, argp);
  va_end(argp);

  output_buffer.push_back(str_buffer);
  current_pos++;
  newline();
}

void
Console::newline()
{
  output_buffer.push_back(current_line);
  current_pos++;
  current_line = "";
}

Console& 
Console::operator<<(string str)
{
  output_buffer.push_back(str);
  current_pos++;
  std::cout << str << std::endl;
  return *this;
}

Console&
Console::operator<<(int)
{
  return *this;
}

void
Console::on_event()
{
  draw();
}

/* EOF */
