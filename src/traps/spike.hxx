//  $Id: spike.hxx,v 1.1 2002/06/12 19:11:32 grumbel Exp $
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

#include "../trap.hxx"

class TrapData;

///
class Spike : public Trap
{
private:
  bool killing;

public:
  Spike(const TrapData& data);
  virtual ~Spike();
  
  void draw_offset(int x_of, int y_of, float s = 1.0);
  void update(float delta);
  void catch_pingu(Pingu*);
};

#endif

/* EOF */
