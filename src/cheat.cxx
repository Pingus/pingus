//  $Id: cheat.cxx,v 1.3 2003/04/19 10:23:17 torangan Exp $
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

#include <iostream>
#include "pingus_error.hxx"
#include "cheat.hxx"

bool Cheat::unlimited_actions = false;
bool Cheat::no_time_limit     = false;
bool Cheat::all_actions       = false;

void
Cheat::activate(const std::string& cheat)
{
  if (cheat == "unlimited-actions" || cheat == "ua")
    {
      unlimited_actions = true;
    }
  else if (cheat == "all-actions" || cheat == "aa")
    {
      all_actions = true;
      unlimited_actions = true;
    }
  else if (cheat == "no-time-limit" || cheat == "nt")
    {
      no_time_limit = true;
    }
  else if (cheat == "help")
    {
      print_help();
    }
  else
    {
      PingusError::raise("Cheat: Unknown cheat '" + cheat + "'");
    }
}

void
Cheat::print_help()
{
  std::cout
    << "Pingus Cheats\n"
    << "-------------\n"
    << "  unlimited-actions(ua) - the number of available actions is not limited\n"
    << "  all-actions(aa)       - all actions are available in all levels and unlimited\n"
    << "  no-time-limit(nt)     - time limit is disabled everywhere\n"
    << std::endl;

  exit(EXIT_SUCCESS);
}

/* EOF */
