//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include "server.hpp"
#include "world.hpp"
#include "globals.hpp"
#include "pingu_holder.hpp"
#include "goal_manager.hpp"


GoalManager::GoalManager(Server* s)
  : server(s), goal(GT_NONE), exit_time(0)
{
}

bool
GoalManager::is_finished()
{
  if (goal == GT_NONE)
    {
      return false;
    }
  else if (goal == GT_GAME_ABORTED)
    {
      return true;
    }
  else if (exit_time == 0)
    {
      // we are finished, now wait a few second so that everybody can
      // see the particles, etc.
      if (maintainer_mode)
        std::cout << "XXXX goal reached: " << goal << std::endl;
      exit_time = server->get_time() + 125;
      return false;
    }
  else
    {
      return (exit_time < server->get_time());
    }
}

void
GoalManager::update()
{
  if (exit_time == 0)
    {
      World*       world  = server->get_world();
      PinguHolder* pingus = world->get_pingus();
      const PingusLevel& plf    = server->get_plf();

      if (pingus->get_number_of_allowed() == pingus->get_number_of_released()
          && pingus->get_number_of_alive() == 0)
        {
          goal = GT_NO_PINGUS_IN_WORLD;
        }
      else if (pingus->get_number_of_alive() == 0 && world->check_armageddon())
        {
          goal = GT_ARMAGEDDON;
        }
      else if (plf.get_time() != -1
               && plf.get_time() <= server->get_time())
        {
          goal = GT_OUT_OF_TIME;
        }
    }
}

void
GoalManager::set_abort_goal()
{
  if (exit_time == 0)
    {
      goal = GT_GAME_ABORTED;
      exit_time = server->get_time();
    }
}


/* EOF */
