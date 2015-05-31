// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_PINGUS_GOAL_MANAGER_HPP
#define HEADER_PINGUS_PINGUS_GOAL_MANAGER_HPP

class Server;

/** Class that looks at the server and searches for goal conditions,
    such no pingus in the world, time up or Escape pressed. */
class GoalManager
{
private:
  Server* server;

  enum GoalType { GT_NONE,        // No goal is reached
                  GT_OUT_OF_TIME, // if the timelimit has passed
                  GT_NO_PINGUS_IN_WORLD, // if all pingus are released and exited/killed
                  GT_ARMAGEDDON, // if armageddon as destroyed all pingus
                  GT_GAME_ABORTED }; // if the user pressed Escape to exit the level };
  GoalType goal;

  /** time at which is_finished() will return true */
  int exit_time;

public:
  GoalManager(Server* plf);

  /** @return true if a goal is reached and the server can shutdown */
  bool is_finished();

  /** Abort the level */
  void set_abort_goal();

  /** Check for goal conditions and set finished accordingly */
  void update();

private:
  GoalManager (const GoalManager&);
  GoalManager& operator= (const GoalManager&);
};

#endif

/* EOF */
