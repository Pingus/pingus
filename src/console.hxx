//  $Id: console.hxx,v 1.10 2002/09/05 17:28:33 grumbel Exp $
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

#ifndef HEADER_PINGUS_CONSOLE_HXX
#define HEADER_PINGUS_CONSOLE_HXX

#include <string>
#include <iostream>
#include "display.hxx"

class CL_Font;

class ConsoleBuffer :
  public std::streambuf
{
private:

  enum { CONSOLE_BUFFER_SIZE = 200 };
  enum { NUM_LINES           = 100 };
  enum { MAX_LINE_LENGTH     = 130  }; // FIXME: this should be dynamic arcording to the current screen size

  std::list<std::string> buffer;
  char char_buffer[CONSOLE_BUFFER_SIZE];
  
public:
  ConsoleBuffer ();  
  virtual ~ConsoleBuffer ();
  int overflow (int c);  
  int sync ();
  const std::list<std::string>& get_buffer ();
  
private:
  /// helper function used by overflow and sync
  std::string fill_buffer (bool append);
  
  ConsoleBuffer (const ConsoleBuffer&);
  ConsoleBuffer operator= (const ConsoleBuffer&);
};

/** A "Quake" like console, but it can just handle output, you can't
    type anything. */
class Console : 
  public std::ostream,
  public DisplayHook
{
private:
  ConsoleBuffer streambuf;

public:
  CL_Font* font;
  bool is_init;
  unsigned int current_pos;

  /** number of lines which will get displayed on the screen */
  unsigned int number_of_lines;
  void draw();
public:
  Console ();
  virtual ~Console();

  void init();
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

  void puts(const std::string&);
  void newline();
  
private:
  Console (const Console&);
  Console operator= (const Console&);
};

/** The globale console object to which you can send messages */
extern Console console;

#endif

/* EOF */
