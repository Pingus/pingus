//  $Id$
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmx.de>
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

#include <stdexcept>
#include <fstream>
#include "pathname.hpp"
#include "server_event.hpp"
#include "pingu_enums.hpp"
#include "pingus_demo.hpp"

PingusDemo::PingusDemo(const Pathname& pathname)
{
  std::ifstream in(pathname.get_sys_path().c_str());
  std::string line;
  
  while(std::getline(in, line))
    {
      char action[256];
      int  time;
      int  id;

      if (sscanf(line.c_str(), "(pingu-action (time %d) (id %d) (action \"%s\"))", &time, &id, action) == 3)
        {
          ServerEvent::make_pingu_action_event(time, id, Actions::action_from_string(action));
        }
      else if (sscanf(line.c_str(), "(armageddon (time %d))", &time) == 1)
        {
          ServerEvent::make_armageddon_event(time);
        }
      else if (sscanf(line.c_str(), "(finish (time %d))", &time) == 1)
        {
          ServerEvent::make_finish_event(time);
        }
      else
        {
          throw std::runtime_error("Couldn't open " + pathname.str());
        }
    }
}

/* EOF */
