//  $Id: SnowParticle.hh,v 1.5 2000/12/14 21:35:56 grumbel Exp $
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

#ifndef SNOWPARTICLE_HH
#define SNOWPARTICLE_HH

#include <ClanLib/core.h>
#include "Particle.hh"

///
class SnowParticle : public Particle
{
private:

public:
  SnowParticle();
  ///
  SnowParticle(int x, int y);
  ///
  void init(int x, int y);
  ///
  virtual ~SnowParticle();

  virtual void let_move();
  virtual bool is_alive();
};

class CollidingSnowParticle : public SnowParticle
{
private:
  bool alive;
  static CL_Surface ground_snow;
public:
  CollidingSnowParticle();
  CollidingSnowParticle(int x, int y);
  virtual ~CollidingSnowParticle();

  virtual void let_move();
  virtual bool is_alive();  
};

#endif

/* EOF */
