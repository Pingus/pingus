//  $Id: view.hxx,v 1.14 2003/10/19 12:25:47 grumbel Exp $
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

#ifndef HEADER_PINGUS_VIEW_HXX
#define HEADER_PINGUS_VIEW_HXX

#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include "range.hxx"
#include "capture_rectangle.hxx"
#include "gui/display_graphic_context.hxx"

namespace Pingus {

class Pingu;
class World;
class Controller;
class Client;

/** A class to controll the rentering of the playfield, each display on
    the screen, which displays the pingus world is an view object. */
class View
{
private:
  DisplayGraphicContext display_gc;

  CL_ClipRect clip_rect;

  /** The position of the view in the world */
  Range x_offset, y_offset;

  bool mouse_over;

  /** The zoom of the View, 1 is default */
  double size;
  int mouse_x;
  int mouse_y;

  CaptureRectangle cap;
  Pingu* current_pingu;
  /// Static objects which are equal for all Views
  static World* world;

  void make_range();

public:
  /** @name The position of the View.

      The View should reach from [x1_pos, x2_pos), x2_pos is not
      included in the View, just like in {\tt CL_Display::fill_rect()}. */
  //@{
  int x1_pos;
  int y1_pos;
  int x2_pos;
  int y2_pos;
  //@}

  View(Client*, int, int, int, int, float s = 1.0);
  ~View();

  void draw();
  void update(float);

  bool is_over(int x, int y);
  bool is_current();
  int  get_x_offset() const;
  int  get_y_offset() const;
  int  get_x_pos() const;
  int  get_y_pos() const;
  void set_x_offset(int);
  void set_y_offset(int);
  void shift_x_offset(int);
  void shift_y_offset(int);

  /** Returns the current center of the screen in World coordinates. */
  Vector get_center ();

  int get_width () { return x2_pos - x1_pos; }
  int get_height () { return y2_pos - y1_pos; }

  void set_zoom(double);
  double get_zoom();
  void set_pingu(Pingu* p);
  static void set_world(World*);

  void on_pointer_move (int x, int y);

private:
  View (const View&);
  View& operator= (const View&);
};

} // namespace Pingus

#endif

/* EOF */
