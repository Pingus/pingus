//  $Id: fire_hole.hh,v 1.5 2001/04/01 18:00:43 grumbel Exp $
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

#ifndef FIRE_HOLE_HH
#define FIRE_HOLE_HH

#include "../plf_datatypes.hh"
#include "../Trap.hh"

///
class FireHole : public Trap
{
private:
  
public:
  ///
  FireHole(trap_data data);
  ///
  virtual ~FireHole();
  ///
  void update(float delta);
  ///
  void catch_pingu(boost::shared_ptr<Pingu>);
};

#endif

/* EOF */
