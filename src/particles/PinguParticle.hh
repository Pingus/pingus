//  $Id: PinguParticle.hh,v 1.1 2000/02/04 23:45:19 mbn Exp $
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

#ifndef PINGUPARTICLE_HH
#define PINGUPARTICLE_HH

#include "Particle.hh"

class PinguParticle : public Particle
{
private:
  double size;
  double size_add;
public:
  PinguParticle();
  PinguParticle(int x, int y, double x_a, double y_a);

  void draw_offset(int ofx, int ofy, double s) const;
  void let_move(void);
  static void load_data(void);
};

#endif

/* EOF */
