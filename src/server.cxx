//  $Id: server.cxx,v 1.24 2002/10/12 00:24:26 grumbel Exp $
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

#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "system.hxx"
#include "pingu.hxx"
#include "server.hxx"
#include "pingu_holder.hxx"
#include "pingus_error.hxx"
#include "string_converter.hxx"
#include "game_time.hxx"
#include "world.hxx"
#include "demo_recorder.hxx"
#include "goal_manager.hxx"

using namespace std;
using Actions::action_from_string;

Server::Server (PLF* arg_plf)
  : plf(arg_plf),
    world(new World (plf)),
    action_holder (plf),
    goal_manager(new GoalManager(this)),
    demo_recorder(new DemoRecorder(this))
{
}

Server::~Server ()
{
  // Demo Server is exited and writes down its log
  delete goal_manager;
  delete demo_recorder;
  delete world;
}

World*
Server::get_world()
{
  return world;
}

void
Server::update()
{
  world->update();
  goal_manager->update();
}

void
Server::send_armageddon_event ()
{
  world->armageddon();

  if (demo_recorder)
    demo_recorder->record_event(ServerEvent::make_armageddon_event(get_time()));
}

void
Server::send_pingu_action_event (Pingu* pingu, Actions::ActionName action)
{
  if (action_holder.pop_action(action))
    {
      if (!(pingu->request_set_action(action)))
	{
	  action_holder.push_action(action);
	}
    }

  if (demo_recorder)
    demo_recorder->record_event(ServerEvent::make_pingu_action_event(get_time(), 
                                                                     pingu->get_id(), 
                                                                     action));
}

bool
Server::is_finished ()
{
  return goal_manager->is_finished();
}

void
Server::set_finished ()
{
  goal_manager->set_abort_goal();
}

ActionHolder*
Server::get_action_holder ()
{
  return &action_holder;
}

int
Server::get_time ()
{
  return get_world()->get_game_time()->get_ticks();
}

/* EOF */
