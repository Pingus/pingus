//  $Id: true_server.cxx,v 1.12 2002/10/01 19:53:44 grumbel Exp $
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
#include <ClanLib/Core/System/system.h>
#include "globals.hxx"
#include "plf.hxx"
#include "true_server.hxx"
#include "world.hxx"

using namespace std;

TrueServer::TrueServer(PLF* arg_plf)
  : Server (arg_plf),
    plf (arg_plf)
{
  armageddon = false;
  world = 0;
  finished = false;
  fast_forward = false;
  pause = false;
  last_time = 0;
  local_game_speed = game_speed;

  world = new World (plf);
}

TrueServer::~TrueServer()
{
  if (world) {
    std::cout << "TrueServer: Deleting World" << std::endl;
    delete world;
  }
}

void
TrueServer::update()
{
  if (fast_forward)
    {
      // To let the game run faster we just update it multiple
      // times
      for (int i = 0; i < 4; ++i)
	{
	  Server::update();
	  world->update();
	}
    }
  else
    {
      Server::update();
      world->update();
    }
}

void
TrueServer::set_fast_forward(bool value)
{
  fast_forward = value;
}

bool
TrueServer::get_fast_forward()
{
  return fast_forward;
}

void 
TrueServer::set_pause(bool value)
{
  pause = value;
}

bool
TrueServer::get_pause()
{
  return pause;
}

PLF*
TrueServer::get_plf ()
{
  return plf;
}

void
TrueServer::set_armageddon ()
{
  armageddon = true;
  world->armageddon ();
}

bool
TrueServer::get_armageddon ()
{
  return armageddon;
}

/* EOF */
