//  $Id: action_holder.cxx,v 1.8 2002/11/03 13:29:09 grumbel Exp $
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
#include "globals.hxx"
#include "plf.hxx"

#include "pingu_action_factory.hxx"
#include "action_holder.hxx"

ActionHolder::ActionHolder (PLF* plf)
{
  std::vector<ActionData> action_data = plf->get_actions();
  
  if (action_data.size() == 0)
    {
      std::cout << "Error: ActionHolder: No actions given in this level! Using defaults" << std::endl;
      action_data = default_actions;
    }
  
  for (std::vector<ActionData>::iterator i = action_data.begin(); i != action_data.end(); ++i) {
    set_actions(i->name, i->number_of);
  }
}

ActionHolder::~ActionHolder ()
{
  //FIXME: This is not really a good place to free the actions
  //FIXME: But not otherwise to possible at the moment due to the
  //FIXME: singleton thing.
  PinguActionFactory::instance()->delete_actions();
}

void
ActionHolder::set_actions (ActionName name, int available)
{
  available_actions[name] = available;
}

void
ActionHolder::push_action (ActionName name)
{
  available_actions[name]++;
}

bool
ActionHolder::pop_action (ActionName name)
{
  int& avail = available_actions[name];

  if (avail > 0)
    {
      --avail;
      return true;
    }
  else
    {
      return false;    
    }
}

int
ActionHolder::get_available (ActionName name)
{
  return available_actions[name];
}

/* EOF */
