//  $Id: action_data.hxx,v 1.6 2002/11/02 19:03:40 grumbel Exp $
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

#ifndef HEADER_PINGUS_ACTION_DATA_HXX
#define HEADER_PINGUS_ACTION_DATA_HXX

#include <vector>
#include "pingu_enums.hxx"

class ActionData
{
public:
  ActionData() {}
  ActionData(Actions::ActionName name_, int i) : name(name_), number_of(i)
  {}

  Actions::ActionName   name;
  int number_of;
};

/** Actions that are available through the GUI in the level editor,
    only useable action are here, hidden actions like walker or drawn
    are not here */
extern std::vector<ActionData> default_actions; 
void init_default_actions ();

#endif

/* EOF */
