//  $Id: Spike.hh,v 1.5 2000/09/18 12:22:16 grumbel Exp $
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

#ifndef SPIKE_HH
#define SPIKE_HH

#include <ClanLib/core.h>

#include "../Trap.hh"
#include "../TrapData.hh"

///
class Spike : public Trap
{
private:
  ///
  bool killing;
public:
  ///
  Spike(TrapData data);
  ///
  virtual ~Spike();
  
  ///
  void draw_offset(int x_of, int y_of, float s = 1.0);
  ///
  void let_move(void);
  ///
  void catch_pingu(Pingu*);
};

#endif

/* EOF */
