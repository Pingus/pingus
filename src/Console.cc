//  $Id: Console.cc,v 1.18 2001/08/04 12:46:22 grumbel Exp $
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
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include "PingusResource.hh"
#include "Console.hh"

using namespace std;

// Globale console
Console console;

ConsoleBuffer::ConsoleBuffer () 
{
  // Set the output buffer
  setp (char_buffer, char_buffer + CONSOLE_BUFFER_SIZE - 1);
  
    // Switch of input buffer
  setg(0, 0, 0);
}
  
ConsoleBuffer::~ConsoleBuffer () {
  sync ();
  int c = 1;
  std::cout << "----------- Debugging Output ------------" << std::endl;
  for (std::vector<std::string>::iterator i = buffer.begin ();
       i != buffer.end (); ++i)
    {
      std::cout << "pingus:" << c++ << ": " << *i << std::endl;
    }
}

int
ConsoleBuffer::overflow (int c) 
{
  std::string str;
    
  for (char* ptr = pbase (); ptr != pptr (); ++ptr)
    {
      if (*ptr != '\n') 
	str += *ptr;
      else
	{
	  std::cout << str << std::endl;
	  buffer.push_back (str);
	  str = "";
	}
    }
  str += c;
  buffer.push_back (str);    
    
  setp (char_buffer, char_buffer + CONSOLE_BUFFER_SIZE - 1);
  return 0;
}
  
int 
ConsoleBuffer::sync () 
{
  std::string str;
    
  for (char* c = pbase (); c != pptr (); ++c)
    {
      if (*c != '\n') 
	str += *c;
      else
	{
	  std::cout << str << std::endl;
	  buffer.push_back (str);
	  str = "";
	}
    }
    
  if (!str.empty ())
    buffer.push_back (str);

  setp (char_buffer, char_buffer + CONSOLE_BUFFER_SIZE - 1);
  return 0;
}

std::vector<std::string>* 
ConsoleBuffer::get_buffer () {
  return &buffer;
}

Console::Console()
  : ostream (&streambuf)
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
  // std::cout << "Console: Init..." << std::endl;
  font = PingusResource::load_font("Fonts/xterm","fonts");

  //  (*this) << "Pingus Output Console (hide/show it with F1)\n"
  //	  << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

  is_init = true;
}

void
Console::draw()
{
  assert(is_init);

  int i,j;
  int start_index;
  bool draw_current_line = !current_line.empty();
  int start_y_pos = 
    CL_Display::get_height() - (font->get_height() * (number_of_lines + 3));

  if (draw_current_line)
    start_index =  max(0, current_pos - number_of_lines);
  else
    start_index =  max(0, current_pos - number_of_lines-1);

  CL_Display::fill_rect(0, start_y_pos,
			CL_Display::get_width(),
			CL_Display::get_height(),
			0.0, 0.0, 0.0, 0.5);

  std::vector<std::string>* output_buffer = streambuf.get_buffer ();
  for(i = start_index, j=1; 
      i < (int)output_buffer->size(); 
      ++i, ++j)
    {
      font->print_left(10, 
		       start_y_pos + j * font->get_height(), 
		       (*output_buffer)[i].c_str());
    }
  if (draw_current_line)
    font->print_left(10, start_y_pos + j * font->get_height(),
		     current_line.c_str());
}

void
Console::increase_lines()
{
  ++number_of_lines;
}

void
Console::decrease_lines()
{
  if (number_of_lines > 0)
    --number_of_lines;
}

void 
Console::scroll_up()
{
  /*  if (current_pos - number_of_lines > 0)
      --current_pos;*/
}

void
Console::scroll_down()
{
  /*if (current_pos - number_of_lines < (int)output_buffer.size())
    ++current_pos;*/
}

void
Console::set_lines(int a)
{
  number_of_lines = a;
}

int
Console::get_lines()
{
  return number_of_lines;
}

void
Console::puts(const std::string& str)
{
  (*this) << str << std::endl;
}

void
Console::newline()
{
  (*this) <<  std::endl;
}

void
Console::on_event()
{
  draw();
}

/* EOF */
