//  $Id: explosive_particle.hxx,v 1.1 2002/06/12 19:11:31 grumbel Exp $
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

#ifndef EXPLOSIVEPARTICLE_HH
#define EXPLOSIVEPARTICLE_HH

#include "../sprite.hxx"
#include "particle.hxx"

class ExplosiveParticle
  : public Particle
{
private:
  Sprite sprite;
  bool alive;
  
public:
  ExplosiveParticle (int x, int y, float x_a, float y_a);
  ~ExplosiveParticle ();
  ///
  void update(float delta);
  ///
  void draw_offset(int ofx, int ofy, float s);
  ///
  bool is_alive(void);
  void detonate ();
};

#endif

/* EOF */
