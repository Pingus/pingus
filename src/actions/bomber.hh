//  $Id: bomber.hh,v 1.4 2000/05/24 15:45:02 grumbel Exp $
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

#ifndef BOMBER_HH
#define BOMBER_HH

#include "../PinguAction.hh"

class Bomber : public PinguAction
{
private:
  bool particle_thrown;
  bool sound_played;
  static CL_Surface* static_surface;
  static CL_Surface* bomber_radius;
  static CL_Surface* explo_surf;
public:
  Bomber();

  void   init(void);
  void   draw_offset(int x, int y, float s);
  void   let_move();
  bool   can_reset() { return false; };
  int    activation_time() { return 50; };
  PinguAction* allocate(void);
};

#endif

/* EOF */
