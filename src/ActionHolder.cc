//  $Id: ActionHolder.cc,v 1.12 2000/05/24 18:48:33 grumbel Exp $
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

#include "ActionHolder.hh"

#define MAX_ACTION_BUFFER_SIZE 0

std::vector<PinguAction*> ActionHolder::uactions;
std::map<std::string, ActionCounter> ActionHolder::uaction_buffer;

ActionHolder::ActionHolder()
{
  if (verbose) std::cout << "ActionHolder: Init uactios" << std::endl;
  init_uactions();
}

ActionHolder::~ActionHolder()
{
  if (verbose) std::cout << "ActionHolder: Destructing ActionHolder" << std::endl;

  for (std::vector<std::string>::iterator i = action_name.begin(); i != action_name.end(); i++)
    {
      for (std::vector<PinguAction*>::iterator j = action[(*i)].action.begin(); j != action[(*i)].action.end(); j++)
	{
	  if (verbose) std::cout << "ActionHolder: Deleting action from: " << *i << std::endl;
	  delete *j;
	}
    }
  clear_uactions();
}

void
ActionHolder::add_action(const std::string& name, int available)
{
  if (verbose > 1)
    std::cout << "ActionHolder::add_action(" << name << ", " << available << ");" << std::endl;
  action[name].number = available;
  
  for (int i = 0; i < available; i++)
    {
      // FIXME: We are 'newing' memory here, we need to release it somewhere again
      action[name].action.push_back(translate_action(name));
    }
}

int
ActionHolder::get_available(const std::string& name)
{
  if (verbose > 1)
    std::cout << "ActionHolder::get_available: " << name << ":" << action[name].number << std::endl;
  return action[name].number;
}

PinguAction*
ActionHolder::get_action(const std::string& name)
{
  ActionCounter* count;
  
  count = &action[name];

  if (count->number > 0) 
    {
      return count->action[(count->number--) - 1];
    }
  else
    {
      if (unlimited_actions) 
	{
	  PinguAction* tmp_action = translate_action(name);
	  count->action.push_back(tmp_action);
	  return tmp_action;
	} 
      else 
	{
	  return 0;
	}
    }
}

void
ActionHolder::push_action(const std::string& name)
{
  action[name].number++;
  
  if (verbose > 1)
    std::cout << "ActionHolder: " << name << ": " << action[name].number << std::endl;
}

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
  } else {
    throw PingusError(std::string("ActionHolder:Action `") + name +"' unkown");
  }  
}

PinguAction* 
ActionHolder::get_uaction(const std::string& name)
{
  PinguAction* tmp_action;
  
  if (name == "bomber")
    {
      if (uaction_buffer["bomber"].number >= MAX_ACTION_BUFFER_SIZE)
	{
	  uaction_buffer["bomber"].action.push_back(translate_action("bomber"));
	}
      else 
	{
	  tmp_action = uaction_buffer["bomber"].action[uaction_buffer["bomber"].number++];
	}
    }
  else
    {
      tmp_action = translate_action(name);
      
      uactions.push_back(tmp_action);
    }

  return tmp_action;
}

void 
ActionHolder::init_uactions()
{
  Timer timer;

  timer.start();
  // FIXME: Test hack, should be replaced
  std::cout << "ActionHoler:init_uaction(): Generating actions..." << std::flush;

  for (int i=0; i < MAX_ACTION_BUFFER_SIZE; i++)
    {
      uaction_buffer["bomber"].action.push_back(translate_action("bomber"));
      uaction_buffer["bomber"].number = 0;
    }

  std::cout << "done " << timer.stop()  << std::endl;
}

void
ActionHolder::clear_uactions()
{
  if (verbose) std::cout << "ActionHolder: Clearing Uactions" << std::endl;

  for(std::vector<PinguAction*>::iterator act = uactions.begin(); act != uactions.end(); act++)
    {
      delete *act;
    }
  uactions.clear();
}

/* EOF */
