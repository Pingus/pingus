//  $Id: WorldObj.hh,v 1.3 2000/04/10 21:14:14 grumbel Exp $
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

#ifndef WORLDOBJ_HH
#define WORLDOBJ_HH

#include "ColMap.hh"

class WorldObj
{
protected:
public:
  int z_pos;

  WorldObj();
  virtual ~WorldObj();
  bool operator< (const WorldObj& w);
  virtual void draw_offset(int, int, float s = 1.0) = 0;
  virtual void draw_colmap(ColMap*);
  virtual void let_move(void);
};

#endif

/* EOF */
