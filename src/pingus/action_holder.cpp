// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/action_holder.hpp"

#include "pingus/globals.hpp"
#include "pingus/pingus_level.hpp"

ActionHolder::ActionHolder(const PingusLevel& plf) :
  m_actions()
{
  const std::map<std::string, int>& actions = plf.get_actions();

  for(std::map<std::string, int>::const_iterator i = actions.begin(); i != actions.end(); ++i)
  {
    set_actions(ActionName::from_string(i->first), i->second);
  }
}

ActionHolder::~ActionHolder ()
{
}

std::vector<ActionName::Enum>
ActionHolder::get_available_actions()
{
  std::vector<ActionName::Enum> ret;

  for(auto i= m_actions.begin(); i != m_actions.end(); ++i)
  {
    ret.push_back(i->first);
  }

  return ret;
}

void
ActionHolder::set_actions (ActionName::Enum name, int available)
{
  m_actions[name] = ActionCount{available, 0};
}

void
ActionHolder::push_action (ActionName::Enum name)
{
  m_actions[name].available += 1;
  m_actions[name].used -= 1;
}

bool
ActionHolder::pop_action (ActionName::Enum name)
{
  auto it = m_actions.find(name);
  if (it == m_actions.end())
  {
    return false;
  }
  else
  {
    if (it->second.available > 0 || globals::developer_mode)
    {
      it->second.available -= 1;
      it->second.used      += 1;
      return true;
    }
    else
    {
      return false;
    }
  }
}

int
ActionHolder::get_available(ActionName::Enum name)
{
  auto it = m_actions.find(name);
  if (it != m_actions.end())
  {
    return it->second.available;
  }
  else
  {
    return 0;
  }
}

int
ActionHolder::get_used(ActionName::Enum name)
{
  auto it = m_actions.find(name);
  if (it != m_actions.end())
  {
    return it->second.used;
  }
  else
  {
    return 0;
  }
}

/* EOF */
