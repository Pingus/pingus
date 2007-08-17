//  $Id$
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

#ifndef HEADER_PINGUS_CHEAT_HXX
#define HEADER_PINGUS_CHEAT_HXX

#include <string>

/** */
namespace Cheat
{
/** unlimited actions are available */
extern bool unlimited_actions;

/** no timelimit for all levels */
extern bool no_time_limit;

/** all actions are available in all levels */
extern bool all_actions;

/** Activate the given cheat */
void activate(const std::string&);

/** Print a list off possible cheats to stdout */
void print_help();
}

#endif

/* EOF */
