//  $Id: Console.cc,v 1.1 2000/06/13 17:50:46 grumbel Exp $
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
  for(unsigned int i = max(0, current_pos - 10), j=1;
      i < output_buffer.size(); 
      i++, j++) 
    {
      font->print_left(10, j * font->get_height(), output_buffer[i].c_str());
    }
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
Console::toggle_display()
{
  is_visible = !is_visible;
}

void
Console::on_event()
{
  if (is_visible)
    draw();
}

/* EOF */
