//  $Id: ActionHolder.hh,v 1.9 2000/06/18 17:01:49 grumbel Exp $
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

#ifndef ACTIONHOLDER_HH
#define ACTIONHOLDER_HH

#include <string>
#include <map>
#include <vector>

#include "PinguAction.hh"

///
class PinguAction;

class ActionHolder
{
private:
  /// A map holding the number of available actions.
  std::map<std::string, int> available_actions;

  // A stack of all the allocated actions, they will be deleted on
  /// destruction. 
  std::vector<PinguAction*> action_stack;

  /// Returns a newed action coresponding to the given name.
  static PinguAction* translate_action(const std::string&);

public:
  ///
  ActionHolder();
  ///
  ~ActionHolder();

  /// Sets the number of actions, which are available in the pool.
  void ActionHolder::set_actions(const std::string& name, int available);
  
  /// Adds an action to the pool of actions.
  void ActionHolder::push_action(const std::string& name);

  /// Sets a given number of actions to the pool.
  void set_action(const std::string& name, int available);

  // Returns the number of actions which are available thru
  /// get_action() 
  int  get_available(const std::string&);

  // Returns a newly allocated or cached action by a given name. It
  // returns it from a pool of action, if the actions are out, it
  /// returns 0. The deletion of the action is handled by this class.  
  PinguAction* get_action(const std::string&);

  // Returns a newly allocated action, an unlimited number of actions
  // can be returned, so this will never return 0, but it throws an
  /// exception if the given action name is unknown.
  PinguAction* get_uaction(const std::string&);
}///
;

#endif

/* EOF */
