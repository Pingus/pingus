//  $Id: Guillotine.hh,v 1.4 2000/07/30 01:47:38 grumbel Exp $
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

#ifndef GUILLOTINE_HH
#define GUILLOTINE_HH

#include <ClanLib/core.h>

#include "../Direction.hh"
#include "../Trap.hh"
#include "../TrapData.hh"

///
class Guillotine : public Trap
{
private:
  ///
  bool killing;
  ///
  CL_Surface* idle_surf;
  ///
  Direction  direction;
  ///
  GameCounter idle_counter;
public:
  ///
  Guillotine(TrapData data);
  ///
  virtual ~Guillotine();
  
  ///
  void let_move(void);
  ///
  void draw_offset(int x, int y, float s);
  ///
  bool catch_pingu(Pingu*);
}///
;

#endif

/* EOF */
