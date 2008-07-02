//  $Id$
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
#include <fstream>
#include "system.hpp"
#include "pingu.hpp"
#include "globals.hpp"
#include "server.hpp"
#include "game_time.hpp"
#include "world.hpp"
#include "goal_manager.hpp"

using Actions::action_from_string;

static std::string get_date_string ()
{
  char buffer[32];
  time_t curtime;
  struct tm *loctime;
  curtime = time (NULL);
  loctime = localtime(&curtime);
  strftime(buffer, 32, "%Y%m%d-%H%M%S", loctime);

  return std::string(buffer);
}

static std::auto_ptr<std::ostream> get_demostream(const std::string& levelname)
{
  std::string flat_levelname = levelname;

  // 'Flatten' the levelname so that we don't need directories
  for (std::string::iterator i = flat_levelname.begin(); i != flat_levelname.end(); ++i)
    if (*i == '/')
      *i = '_';

  std::string filename = System::get_userdir() + "demos/" + get_date_string() + "-" + flat_levelname + ".pingus-demo";

  std::auto_ptr<std::ofstream> out(new std::ofstream(filename.c_str()));
  
  if (!(*out.get()))
    {
      std::cout << "DemoRecorder: Error: Couldn't write DemoFile '" << filename
                << "', demo recording will be disabled" << std::endl;
      return std::auto_ptr<std::ostream>();
    }
  else
    {
      std::cout << "DemoRecorder: Writing demo to: " << filename << std::endl;

      // Write file header
      *out << "(level (name \"" << levelname << "\"))\n";
      return std::auto_ptr<std::ostream>(out.release());
    }
}

Server::Server(const PingusLevel& arg_plf,
               bool record_demo)
  : plf(arg_plf),
    world(new World (plf)),
    action_holder (plf),
    goal_manager(new GoalManager(this))
{
  if (record_demo)
    {
      demostream = get_demostream(plf.get_resname());
    }
}

Server::~Server ()
{
}

World*
Server::get_world()
{
  return world.get();
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
  record(ServerEvent::make_armageddon_event(get_time()));

  world->armageddon();
}

void
Server::send_pingu_action_event (Pingu* pingu, Actions::ActionName action)
{
  record(ServerEvent::make_pingu_action_event(get_time(), pingu->get_id(), pingu->get_pos(), action));

  if (action_holder.pop_action(action))
    {
      if (!(pingu->request_set_action(action)))
	{
	  action_holder.push_action(action);
	}
    }
}

void
Server::record(const ServerEvent& event)
{
  if (demostream.get())
  event.write(*demostream);
}

bool
Server::is_finished ()
{
  return goal_manager->is_finished();
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

void
Server::send_finish_event()
{
  record(ServerEvent::make_finish_event(get_time()));
  goal_manager->set_abort_goal();
}

/* EOF */
