//  $Id: bridger.hh,v 1.7 2000/06/18 17:01:50 grumbel Exp $
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

#ifndef BRIDGER_HH
#define BRIDGER_HH

#include "../GameCounter.hh"
#include "../PinguAction.hh"

///
class Bridger : public PinguAction
{
private:
  ///
  GameCounter counter;

  ///
  static CL_Surface* static_surface;
  ///
  static CL_Surface* waiter;
  ///
  static CL_Surface* brick_l;
  ///
  static CL_Surface* brick_r;

  ///
  int bricks;
  ///
  int step;
  ///
  int do_steps;

public:
  ///
  Bridger();

  ///
  PinguAction* allocate(void);

  ///
  int    x_offset();
  ///
  int    y_offset();
  ///
  void   init();
  ///
  void   let_move();
  ///
  void   draw_offset(int, int, float s);
  ///
  bool   way_is_free();
  ///
  void   place_a_brick();
  ///
  void   walk_one_step_up();
}///
;

#endif

/* EOF */
