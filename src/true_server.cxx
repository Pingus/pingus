//  $Id: true_server.cxx,v 1.11 2002/09/28 18:34:21 grumbel Exp $
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
  client_needs_redraw = true;
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
TrueServer::update(float delta)
{
  if (enough_time_passed()) 
    {
      client_needs_redraw = true;

      if (fast_forward)
	{
	  // To let the game run faster we just update it multiple
	  // times
	  for (int i = 0; i < 4; ++i)
	    {
	      Server::update(delta);
	      world->update(delta);
	    }
	}
      else
	{
	  Server::update(delta);
	  world->update(delta);
	}
    }
  else
    {
      //std::cout << "Sleeping: " << time_till_next_update() << std::endl;
      //CL_System::sleep(time_till_next_update());
    }
}

bool
TrueServer::enough_time_passed(void)
{
  if (pause) {
    return false;
  }

  if (fast_forward) {
    // FIXME: This should skip some frames, so it also works on slower
    // machines
    last_time = CL_System::get_time();
    return true;
  } else {
    if (last_time + local_game_speed > CL_System::get_time()) {
      return false;
    } else {
      delta = (CL_System::get_time () - last_time)/1000.0f;
      last_time = CL_System::get_time();
      return true;
    }
  }
}

int
TrueServer::time_till_next_update()
{
  return last_time + local_game_speed - CL_System::get_time();
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

bool
TrueServer::needs_redraw()
{
  if (client_needs_redraw) {
    client_needs_redraw = false;
    return true;
  }
  return false;
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
