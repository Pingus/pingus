//  $Id: credits.hxx,v 1.2 2002/06/24 22:52:54 grumbel Exp $
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

#ifndef HEADER_PINGUS_CREDITS_HXX
#define HEADER_PINGUS_CREDITS_HXX

#include <string>
#include <vector>
#include <ClanLib/Display/Display/surface.h>

class CL_Font;

///
class Credits
{
private:
  ///
  CL_Surface surface;
  ///
  CL_Font*    font;
  ///
  CL_Font*    font_small;
  ///
  bool is_init;

  float offset;

  /** The string's which are shown in the scrolling text, the first
      character of the string can be used to apply a special format.
      
      'n' - Inserts a newline
      '_' - Use small font
      '-' - Use large font
  */
  std::vector<std::string> credits;
public:
  ///
  Credits();
  ///
  ~Credits();
  ///
  void init ();
  void update (float delta);
  void draw ();
  ///
  void display();
};

extern Credits credit_screen;

#endif

/* EOF */

