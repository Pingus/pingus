//  $Id: GameObj.hh,v 1.1 2000/02/04 23:45:18 mbn Exp $
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

#ifndef GAMEOBJ_HH
#define GAMEOBJ_HH

#include <ClanLib/clanlayer2.h>

// Class for most of the objects in the game, it provides and easy way 
// to load the surfaces from the datafile and avoids global objects.
class GameObj
{
public:
  virtual ~GameObj();
  //  static CL_ResourceManager* resource_p;

  //static int SetResourceManager( CL_ResourceManager* );
  //  static CL_ResourceManager* resource();
  virtual void let_move();
  virtual void draw_offset(int x, int y);
};

#endif

/* EOF */
