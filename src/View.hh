//  $Id: View.hh,v 1.1 2000/02/04 23:45:19 mbn Exp $
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
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef VIEW_HH
#define VIEW_HH

#include "Range.hh"
#include "World.hh"
#include "CaptureRectangle.hh"

// A class to controll the rentering of the playfield, each display on 
// the screen, which displays the pingus world is an view object.
class View
{
private:
  CL_ClipRect clip_rect;
  Range x_offset, y_offset; // The position of the view in the world
  int x1_pos, y1_pos;       // The upper/left corner of the view
  int x2_pos, y2_pos;       // The buttom/right corner of the view
  bool mouse_over;
  double size;              // The zoom of the View, 1 is default
  CaptureRectangle cap;
  Pingu* current_pingu;
  // Static objects which are equal for all Views
  static World* world;

  void make_range();
public:
  View(int, int, int, int, float s = 1.0);
  ~View();
  void draw();
  bool is_over(int x, int y);
  bool is_current();
  int  get_x_offset() const;
  int  get_y_offset() const;
  int  get_x_pos() const;
  int  get_y_pos() const; 
  void set_x_offset(int);
  void set_y_offset(int);
  void set_zoom(double);
  double get_zoom();
  void set_pingu(Pingu* p);
  static void set_world(World*);
};

#endif

/* EOF */
