//  $Id: goal_manager.cxx,v 1.1 2002/10/04 16:54:04 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include "server.hxx"
#include "world.hxx"
#include "plf.hxx"
#include "pingu_holder.hxx"
#include "goal_manager.hxx"

GoalManager::GoalManager(Server* s)
  : server(s), goal(GT_NONE)
{
}

bool
GoalManager::is_finished()
{
  if (goal == GT_NONE)
    {
      return false;
    }
  else
    {
      std::cout << "XXXX goal reached: " << goal << std::endl;
      return true;
    }
}

void
GoalManager::update()
{
  World*       world  = server->get_world();
  PinguHolder* pingus = world->get_pingus();
  PLF*         plf    = server->get_plf();

  if (pingus->get_number_of_allowed() == pingus->get_number_of_released()
      && pingus->get_number_of_alive() == 0)
    {
      goal = GT_NO_PINGUS_IN_WORLD;
    }
  else if (pingus->get_number_of_alive() == 0 && world->check_armageddon())
    {
      goal = GT_ARMAGEDDON;
    }
  else if (plf->get_time() != -1
           && plf->get_time() <= server->get_time())
    {
      goal = GT_OUT_OF_TIME;
    }
}

void
GoalManager::set_abort_goal()
{
  goal = GT_GAME_ABORTED;
}

/* EOF */
