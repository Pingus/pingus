//  $Id: Entrance.hh,v 1.2 2000/02/09 21:43:39 grumbel Exp $
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

#ifndef ENTRANCE_HH
#define ENTRANCE_HH

#include <string>
#include <ClanLib/core.h>

#include "Pingu.hh"
#include "WorldObj.hh"
#include "particles/ParticleHolder.hh"
#include "entrance_data.hh"

class Entrance : public WorldObj
{
protected:
  static CL_Surface* surface;
  int last_release;
  int release_rate;
  entrance_data::Direction direction;
  
  int x_pos;
  int y_pos;

  static ParticleHolder* particle;
public:
  Entrance();
  Entrance(entrance_data data);

  virtual bool   pingu_ready(void);
  virtual Pingu* get_pingu(void);
  virtual void   let_move(void);
  virtual void   draw_offset(int x, int y, float s = 1.0);

  static void SetParticleHolder(ParticleHolder* p);
};

#endif

/* EOF */
