//  $Id: MultiLineText.hh,v 1.3 2001/03/18 17:45:04 grumbel Exp $
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

#ifndef MULTILINETEXT_HH
#define MULTILINETEXT_HH

#include <string>
#include <vector>
#include <ClanLib/core.h>
#include <ClanLib/display.h>

///
class MultiLineText
{
private:
  ///
  CL_Font* font;
  ///
  std::vector<std::string> text;
  ///
  int width, height;
public:
  ///
  MultiLineText();

  ///
  void set_font(CL_Font*);
  ///
  void set_text(const std::string&, int width);
  ///
  void print_left(int, int);
  ///
  void print_right(int, int);
  ///
  void print_center(int, int);
  ///
  int  get_width();
  ///
  int  get_height();
}///
;

#endif

/* EOF */
