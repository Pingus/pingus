//  $Id: SmallMap.hh,v 1.1 2000/02/25 02:35:27 grumbel Exp $
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

#ifndef SMALLMAP_HH
#define SMALLMAP_HH

#include "World.hh"
#include "GuiObj.hh"

class SmallMap : public GuiObj
{
private:
  CL_Surface* sur;
  World*      world;
public:
  SmallMap();
  ~SmallMap();
  
  void set_world(World* w);
  void init();
  void draw();
  void let_move();
};

#endif

/* EOF */
