//  $Id: Trap.hh,v 1.17 2001/04/21 10:55:15 grumbel Exp $
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

#ifndef TRAP_HH
#define TRAP_HH

#include <string>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "GameCounter.hh"
#include "Pingu.hh"
#include "WorldObj.hh"
#include "TrapData.hh"

class Pingu;

/** An abstract class for all trap or moving objects in the pingu word
    which are not under controll of the player (i.e. fire traps,
    rolling stones, etc.) */
class Trap : public TrapData,
	     public WorldObj
{
protected:
  ///
  CL_Surface surface;
  ///
  GameCounter counter;

public:
  ///
  Trap();
  ///
  virtual ~Trap();
  ///
  virtual int  get_z_pos() const { return (int) pos.z; }
  ///
  virtual void draw_offset(int x, int y, float s = 1.0);
  ///
  virtual void update(float delta);
  /// return true if the pingu was catched
  virtual void catch_pingu(boost::shared_ptr<Pingu>);
};

#endif

/* EOF */
