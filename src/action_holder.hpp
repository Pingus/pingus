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

#ifndef HEADER_PINGUS_ACTION_HOLDER_HXX
#define HEADER_PINGUS_ACTION_HOLDER_HXX

#include <map>
#include <vector>
#include "pingu_enums.hpp"


class PingusLevel;
class PinguAction;

/**
 * The ActionHolder is the backend of the ButtonPanel. It is responsible for
 * creating new PinguActions and (if necessary) restricting the number of
 * actions that can be created.
 *
 */
class ActionHolder
{
private:
  /** A map holding the number of available actions.
   *  For each action <i>name</i> there is a map (<i>name</i>, <i>n</i>)
   *  where the integer <i>n</i> indicates how much more actions
   *  called <i>name</i> can be created.
   */
  std::map<Actions::ActionName, int> available_actions;

public:
  /** Create an action holder from the number of actions given in a
      Pingus Level file */
  ActionHolder(const PingusLevel& plf);
  ~ActionHolder();

  /** @return a list of available actions in the current level */
  std::vector<Actions::ActionName> get_available_actions();

  /** Sets the number of actions, which are available in the pool.
   * @param name the name of the action
   * @param available the number of actions available
   */
  void set_actions (Actions::ActionName name, int available);

  /** Adds an action to the pool of actions, making one more action available
   * @param name the name of the action
   */
  void push_action (Actions::ActionName name);

  /** Removes an action from the pool of actions
      @return true if an action was available and removed, false if no
      actions where available */
  bool pop_action (Actions::ActionName name);

  /** Returns the number of actions of the specified name which are available
   *  thru get_action()
   * @return 0 if the name is unknown FIXME: should use .find instead of []
   */
  int get_available (Actions::ActionName name);

private:
  ActionHolder (const ActionHolder&);
  ActionHolder& operator= (const ActionHolder&);
};


#endif

/* EOF */
