//  $Id: Console.hh,v 1.12 2000/10/09 19:17:30 grumbel Exp $
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

#ifndef CONSOLE_HH
#define CONSOLE_HH

#include <string>
#include <vector>
#include <ClanLib/core.h>
#include "Display.hh"

/** A "Quake" like console, but it can just handle output, you can't
    type anything. */
class Console : public DisplayHook
{
private:
  /// 
  struct Endl {};
  ///
  CL_Font* font;
  ///
  bool is_init;
  ///
  int  current_pos;
  ///
  std::vector<std::string> output_buffer;
  ///
  std::string current_line;
  ///
  int number_of_lines;

  ///
  void draw();
  ///
  void add_line(std::string);
public:
  /** A incarnation of the Endl class, just there to make the
      operator<<() behave like the standard std::cout stuff. */
  static Endl endl;

  ///
  Console();
  ///
  virtual ~Console();

  ///
  void init();
  ///
  virtual void on_event();

  /** Sets the number of lines, which are displayed
      @param lines The number of lines which are displayed */
  void set_lines(int lines);

  /// Returns the height in number of lines, {\em not} in pixels.
  int  get_lines();

  /** Increase the number of lines, which are displayed */
  void increase_lines();

  /** Decrease the number of lines, which are displayed */
  void decrease_lines();

  /** Scroll up */
  void scroll_up();

  /** Scroll up */
  void scroll_down();

  /** @name Output operators, similar to std::cout */
  //@{
  /// Wrapper around the end-of-line operator, its just insert a newline in the console.
  Console& operator<<(const Console::Endl&);

  /// Prints the given string to the console.
  Console& operator<<(std::string);

  /// Converts the given int to a string and prints it to the console.
  Console& operator<<(int);
  //@}

  /** Print to the console, just like a normal printf(). All format
      specifies available in printf() are also available here, since
      this is just a wrapper around vsnprintf(). This function prints
      only up to 1023 characters at a time, the rest is lost. */
  void printf(char* format, ...);
  ///
  void puts(std::string);
  ///
  void newline();
}///
;

///
extern Console console;

#endif

/* EOF */
