//  $Id: TrueServer.cc,v 1.22 2001/04/13 13:45:09 grumbel Exp $
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

#include "GameTime.hh"
#include "Timer.hh"
#include "globals.hh"

#include "TrueServer.hh"

TrueServer::TrueServer(boost::shared_ptr<PLF> arg_plf)
  : plf (arg_plf)
{
  filename = plf->get_filename();
  local_game_speed = game_speed;
  world = 0;
  finished = false;
  client_needs_redraw = true;
  start(plf);
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
      GameTime::increase();
      Server::update(delta);
      world->update(delta);
    }
}

void
TrueServer::start(boost::shared_ptr<PLF> level_data)
{
  Timer timer;

  filename = level_data->get_filename();

  std::vector<ActionData> bdata;

  timer.start();
  
  std::cout << "TrueServer: Generating actions..." << std::flush;

  bdata = level_data->get_actions();

  for(std::vector<ActionData>::iterator b = bdata.begin(); b != bdata.end(); ++b) {
    action_holder.set_actions(b->name, b->number_of);
  }
  std::cout << "done " << timer.stop() << std::endl;

  fast_forward = false;
  pause = false;
  last_time = 0;
  local_game_speed = game_speed;

  world = new World;

  // FIXME: this is complete trash, delete it and place it in world
  // object or so...
  world->set_action_holder(&action_holder);

  world->init(level_data);

  GameTime::reset();
}

bool
TrueServer::enough_time_passed(void)
{
  if (pause) {
    return false;
  }

  if (fast_forward) {
    // BUG: This should skip some frames, so it also works on slower
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

void
TrueServer::set_fast_forward(bool value)
{
  fast_forward = value;
  if (fast_forward)
    local_game_speed = 5;
  else 
    local_game_speed = game_speed;
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
TrueServer::get_fast_forward()
{
  return fast_forward;
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

boost::shared_ptr<PLF> 
TrueServer::get_plf ()
{
  return plf;
}

/* EOF */
