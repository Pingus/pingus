//  $Id: GameObj.cc,v 1.1 2000/02/04 23:45:18 mbn Exp $
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

#include <iostream>
#include <ClanLib/clanlib.h>
#include <ClanLib/clanlayer2.h>

#include "GameObj.hh"

//CL_ResourceManager* GameObj::resource_p;

GameObj::~GameObj()
{
}
/*
CL_ResourceManager*
GameObj::resource()
{
  //  cout << "GameObj: resource_p=" << resource_p << endl;
  return resource_p;
}

int
GameObj::SetResourceManager(CL_ResourceManager* res)
{
  resource_p = res;
  return true; //
}
*/
void 
GameObj::let_move()
{
  //  cout << "GameObj::let_move: doing nothing" << endl;
}

void 
GameObj::draw_offset(int x, int y)
{
  //  cout << "GameObj::let_move: doing nothing" << endl;
}


/* EOF */
