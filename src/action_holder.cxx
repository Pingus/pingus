//  $Id: action_holder.cxx,v 1.14 2003/10/21 11:01:52 grumbel Exp $
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
#include "plf.hxx"
#include "pingu_action_factory.hxx"
#include "action_holder.hxx"
#include "cheat.hxx"

namespace Pingus {

using namespace Actions;

ActionHolder::ActionHolder (PLF* plf)
{
  std::vector<ActionData> action_data = plf->get_actions();

  if (action_data.size() == 0 || Cheat::all_actions)
    {
      std::cout << "Error: ActionHolder: No actions given in this level!" << std::endl;
    }

  for (std::vector<ActionData>::iterator i = action_data.begin(); i != action_data.end(); ++i) {
    set_actions(i->name, i->number_of);
  }
}

ActionHolder::~ActionHolder ()
{
}

std::vector<Actions::ActionName>
ActionHolder::get_available_actions()
{
  std::vector<ActionName> ret;

  for(std::map<ActionName, int>::iterator i= available_actions.begin();
      i != available_actions.end(); ++i)
    ret.push_back(i->first);

  return ret;
}

void
ActionHolder::set_actions (ActionName name, int available)
{
  available_actions[name] = available;
}

void
ActionHolder::push_action (ActionName name)
{
  if (!Cheat::unlimited_actions)
    available_actions[name]++;
}

bool
ActionHolder::pop_action (ActionName name)
{
  if (Cheat::unlimited_actions)
    {
      return true;
    }
  else
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
}

int
ActionHolder::get_available (ActionName name)
{
  return available_actions[name];
}

} // namespace Pingus

/* EOF */
