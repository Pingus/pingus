//  $Id$
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

#ifndef HEADER_PINGUS_GOAL_MANAGER_HXX
#define HEADER_PINGUS_GOAL_MANAGER_HXX


class Server;

/** Class that looks at the server and searches for goal conditions,
    such no pingus in the world, time up or Escape pressed. */
class GoalManager
{
private:
  Server* server;

public:
  enum GoalType { 
    GT_NONE,        // No goal is reached
    GT_OUT_OF_TIME, // if the timelimit has passed
    GT_NO_PINGUS_IN_WORLD, // if all pingus are released and exited/killed
    GT_ARMAGEDDON, // if armageddon as destroyed all pingus
    GT_GAME_ABORTED, // if the user pressed Escape to exit the level
    GT_GLOBAL_TIME_LIMIT
  };

private:
  GoalType goal;

  /** time at which is_finished() will return true */
  int exit_time;

  bool m_always_fail;

public:
  GoalManager(Server* plf);

  /** @return true if a goal is reached and the server can shutdown */
  bool is_finished();

  /** Abort the level */
  void set_abort_goal(bool always_fail = false);

  GoalType get_goal() const { return goal; }

  /** Check for goal conditions and set finished accordingly */
  void update();

  bool get_always_fail() const { return m_always_fail; }

private:
  GoalManager (const GoalManager&);
  GoalManager& operator= (const GoalManager&);
};


#endif

/* EOF */
