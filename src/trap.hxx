//  $Id: trap.hxx,v 1.3 2002/08/23 15:49:51 torangan Exp $
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

#ifndef HEADER_PINGUS_TRAP_HXX
#define HEADER_PINGUS_TRAP_HXX

#include <ClanLib/Display/Display/surface.h>
#include "game_counter.hxx"
#include "worldobj.hxx"
#include "trap_data.hxx"

class Pingu;

/** An abstract class for all trap or moving objects in the pingu word
    which are not under controll of the player (i.e. fire traps,
    rolling stones, etc.) */
class Trap : public TrapData,
	     public WorldObj
{
protected:
  CL_Surface surface;
  GameCounter counter;

public:
  Trap();
  virtual ~Trap();

  virtual float get_z_pos() const { return pos.z; }
  virtual void draw_offset(int x, int y, float s = 1.0);
  virtual void update(float delta);
  
protected:
  virtual void catch_pingu(Pingu*) =0;
  
private:
  Trap (const Trap&);
  Trap operator= (const Trap&);
};

#endif

/* EOF */

