//  $Id: GuiObj.hh,v 1.9 2001/04/13 22:17:46 grumbel Exp $
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

/** A GuiObj is an abstract interface for seperate parts of the
    screen, the complete screen is representated by a window obtect. A
    GuiObj must provide a methode to react on input, update the screen
    and redraw it. */
class GuiObj 
{
protected:
  ///
  int x1;
  ///
  int y1;
  ///
  int x2;
  ///
  int y2;
public:
  ///
  GuiObj();
  ///
  virtual ~GuiObj();

  ///
  void    set_clip_rect(int, int, int, int);
  
  /// Draw the gui element and do the clipping.
  virtual void draw_clipped();

  /// Draw the gui element
  virtual void draw() = 0;

  /// Move the world one step further
  virtual void update(float delta);

  /** Update all elements, which are indepented of the world speed
      (for example scrolling). */
  virtual void updateX();
};

#endif

/* EOF */
