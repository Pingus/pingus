//  $Id: ActionHolder.cc,v 1.19 2000/08/11 01:11:04 grumbel Exp $
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
#include "globals.hh"

#include "PingusError.hh"
#include "Timer.hh"
#include "globals.hh"

#include "actions/basher.hh"
#include "actions/blocker.hh"
#include "actions/bomber.hh"
#include "actions/bridger.hh"
#include "actions/climber.hh"
#include "actions/digger.hh"
#include "actions/exiter.hh"
#include "actions/floater.hh"
#include "actions/miner.hh"
#include "actions/Jumper.hh"
#include "actions/teleported.hh"
#include "actions/exiter.hh"
#include "actions/smashed.hh"
#include "actions/LaserKill.hh"
#include "actions/Splashed.hh"
#include "actions/Waiter.hh"
#include "actions/Drown.hh"

#include "ActionHolder.hh"

ActionHolder::ActionHolder()
{
}

ActionHolder::~ActionHolder()
{
  for (std::vector<PinguAction*>::iterator i = action_stack.begin(); 
       i != action_stack.end();
       i++)
    {
      delete *i;
    }
}

void
ActionHolder::set_actions(const std::string& name, int available)
{
  available_actions[name] = available;
}

void
ActionHolder::push_action(const std::string& name)
{
  available_actions[name]++;
}

int
ActionHolder::get_available(const std::string& name)
{
  return available_actions[name];
}

PinguAction*
ActionHolder::get_action(const std::string& name)
{
  int* count;
  
  count = &(available_actions[name]);


  if (*count > 0) 
    {
      (*count)--;
      return get_uaction(name);
    }
  else
    {
      if (unlimited_actions) {
      	return get_uaction(name);
      } else {
	return 0;
      }
    }
}

// Allocates an action from the given name
PinguAction*
ActionHolder::translate_action(const std::string& name)
{
    if (name == "climber") {
    return new Climber;
  } else if (name == "blocker") {
    return new Blocker;
  } else if (name == "bomber") {
    return new Bomber;
  } else if (name == "bridger") {
      return new Bridger;
  } else if (name == "digger") {
    return new Digger;
  } else if (name == "miner") {
    return new Miner;
  } else if (name == "floater") {
    return new Floater;
  } else if (name == "basher") {
    return new Basher;
  } else if (name == "jumper") {
    return new Jumper;
  } else if (name == "exiter") {
    return new Exiter;
  } else if (name == "teleported") {
    return new Teleported;
  } else if (name == "laserkill") {
    return new LaserKill;
  } else if (name == "smashed") {
    return new Smashed;
  } else if (name == "splashed") {
    return new Splashed;
  } else if (name == "waiter") {
    return new Waiter;
  } else if (name == "drown") {
    return new Drown;
  } else {
    throw PingusError(std::string("ActionHolder:Action `") + name +"' unkown");
  }  
}

// Returns an newly allocated action and adds it to the action_stack
// to ensure a later cleanup.
PinguAction* 
ActionHolder::get_uaction(const std::string& name)
{
  PinguAction* tmp_action;

  tmp_action = translate_action(name);
  
  action_stack.push_back(tmp_action);

  return tmp_action;
}

/* EOF */
