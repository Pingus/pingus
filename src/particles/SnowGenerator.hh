//  $Id: SnowGenerator.hh,v 1.1 2000/08/02 19:00:08 grumbel Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef SNOWGENERATOR_HH
#define SNOWGENERATOR_HH

#include "../WorldObj.hh"
#include "ParticleHolder.hh"

class SnowGenerator : public WorldObj
{
private:
  static ParticleHolder* particle;
  int strength;
public:
  ///
  SnowGenerator();
  ///
  SnowGenerator::SnowGenerator(int s);
  ///
  virtual ~SnowGenerator();
  ///
  virtual void let_move();
  int get_z_pos() const { return 0; };
  void draw_offset(int, int, float) {};
  ///
  static void SetParticleHolder(ParticleHolder* p);
};

#endif

/* EOF */
