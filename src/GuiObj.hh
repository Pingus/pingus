//  $Id: GuiObj.hh,v 1.4 2000/06/18 18:06:56 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef GUI_OBJ_HH
#define GUI_OBJ_HH

/** A GuiObj is an abstract interface for seperate parts of the screen, 
    the complete screen is representated by a window obtect. A GuiObj
    must provide a methode to react on input, update the screen and
    redraw it. */
class GuiObj 
{
private:
  ///
  int x1, y1, x2, y2;
public:
  ///
  GuiObj();
  ///
  virtual ~GuiObj();

  ///
  void    set_clip_rect(int, int, int, int);
  ///
  virtual void draw_clipped();
  ///
  virtual void draw() = 0;
  ///
  virtual void let_move();
};

#endif

/* EOF */
