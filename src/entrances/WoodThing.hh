//  $Id: WoodThing.hh,v 1.4 2000/07/30 01:47:38 grumbel Exp $
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

#ifndef WOODTHING_HH
#define WOODTHING_HH

#include "../GameCounter.hh"
#include "../Entrance.hh"

///
class WoodThing : public Entrance
{
private:
  ///
  GameCounter counter;
  ///
  CL_Surface* surface2;
public:
  ///
  WoodThing(EntranceData);

  ///
  void let_move(void);
  ///
  void draw_offset(int x, int y, float s = 1.0);
}///
;

#endif

/* EOF */

