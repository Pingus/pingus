//  $Id: console.hxx,v 1.1 2002/06/12 19:09:37 grumbel Exp $
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
#include <iostream>
#include "display.hxx"

class CL_Font;

#define CONSOLE_BUFFER_SIZE 100

class ConsoleBuffer :
  public std::streambuf
{
private:
  std::string current_line;
  bool continue_last;
  std::vector<std::string> buffer;
  char char_buffer[CONSOLE_BUFFER_SIZE];
  
public:
  ConsoleBuffer ();  
  virtual ~ConsoleBuffer ();
  int overflow (int c);  
  int sync ();
  std::vector<std::string>* get_buffer ();
};

/** A "Quake" like console, but it can just handle output, you can't
    type anything. */
class Console : 
  public std::ostream,
  public DisplayHook
{
private:
  ConsoleBuffer streambuf;
  std::vector<std::string>* buffer;

public:
  ///
  CL_Font* font;
  ///
  bool is_init;
  ///
  int  current_pos;
  ///
  std::string current_line;
  ///
  int number_of_lines;
  ///
  void draw();
public:
  ///
  Console ();
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

  /** Scroll down or up n lines, depending on the sign */
  void scroll (int n);

  ///
  void puts(const std::string&);
  ///
  void newline();
};

/** The globale console object to which you can send messages */
extern Console console;

#endif

/* EOF */




