//  $Id: console.cxx,v 1.19 2003/10/21 21:37:06 grumbel Exp $
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

#include <config.h>
#include <assert.h>
#include <ClanLib/Display/display.h>
#include <ClanLib/Display/font.h>
#include "fonts.hxx"
#include "console.hxx"
#include "math.hxx"

using std::ostream;

namespace Pingus {

// Globale console
Console console;

ConsoleBuffer::ConsoleBuffer () : buffer(NUM_LINES)
{
  // Set the output buffer
  setp (char_buffer, char_buffer + CONSOLE_BUFFER_SIZE - 1);

  // Switch of input buffer
  setg(0, 0, 0);
}

ConsoleBuffer::~ConsoleBuffer ()
{
  sync ();
}

int
ConsoleBuffer::overflow (int c)
{
  std::string str = fill_buffer(true);

  str += c;
  buffer.push_back(str);
  buffer.pop_front();

  setp (char_buffer, char_buffer + CONSOLE_BUFFER_SIZE - 1);
  return 0;
}

int
ConsoleBuffer::sync ()
{
  std::string str = fill_buffer(false);

  if (!str.empty())
    {
      buffer.push_back(str);
      buffer.pop_front();
    }

  setp(char_buffer, char_buffer + CONSOLE_BUFFER_SIZE - 1);
  return 0;
}

std::string
ConsoleBuffer::fill_buffer (bool append)
{
  std::string str;
  if (append)
    {
      str = *(--buffer.end());
      buffer.pop_back();
      buffer.push_front("");
    }

  for (char* c = pbase (); c != pptr (); ++c)
    {
      if (*c != '\n')
	str += *c;
      else
	{
	  if (str.size() > MAX_LINE_LENGTH)
	    {
	      std::string::size_type pos = str.rfind(' ');
	      if (pos == std::string::npos)
	        pos = MAX_LINE_LENGTH;

	      buffer.push_back(str.substr(0, pos));
	      buffer.pop_front();

	      str = str.substr(pos, str.size());
	    }

          buffer.push_back(str);
          buffer.pop_front();
          str = "";
	}
    }

  return str;
}

const std::list<std::string>&
ConsoleBuffer::get_buffer () {
  return buffer;
}

Console::Console() : ostream (&streambuf) // std:: is missing here since Win32 doesn't like it
{
  is_init = false;
  is_visible = false;
  current_pos = 0;
  number_of_lines = 12;

  *this << "================================" << std::endl;
  *this << "Welcome to Pingus " << VERSION << std::endl;
  *this << "================================" << std::endl;
  newline ();
  *this << "This is the output and debug console, you can toggle it with ^" << std::endl;
  newline ();
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
  font = Fonts::xterm;

  //  (*this) << "Pingus Output Console (hide/show it with F1)\n"
  //	  << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

  is_init = true;
}

void
Console::draw()
{
  assert(is_init);

  /** Callculate the position of the first line on the screen */
  int start_y_pos =
    CL_Display::get_height() - (font.get_height() * (number_of_lines + 3));

  // The background of the console
  CL_Display::fill_rect(CL_Rect(0, start_y_pos - 15,
                                CL_Display::get_width(),
                                CL_Display::get_height()),
			Display::to_color(0.0, 0.0, 0.0, 0.5));

  const std::list<std::string>& buffer = streambuf.get_buffer ();

  unsigned int window_start = Math::max(0, int(buffer.size() - number_of_lines - current_pos));

  std::list<std::string>::const_iterator it = buffer.begin();

  // move iterator to the first line to be displayed
  for (unsigned int i = 0; i < window_start; ++i)
    ++it;

  for (unsigned int i = 0;
       i < number_of_lines && i + window_start < buffer.size();
       ++it, ++i)
    {
      font.draw(10,
                start_y_pos + (i * (font.get_height() + 2)),
                it->c_str()
                );
    }
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
  if (current_pos + number_of_lines < streambuf.get_buffer().size())
    ++current_pos;
}

void
Console::scroll_down()
{
  if (current_pos)
    --current_pos;
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

} // namespace Pingus

/* EOF */
