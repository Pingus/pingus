//  $Id: action_data.cxx,v 1.4 2002/07/29 10:44:12 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include "pingu_enums.hxx"
#include "action_data.hxx"

using namespace Pingus::Actions;

std::vector<ActionData> default_actions; 

void init_default_actions ()
{
  //default_actions.push_back (ActionData (Boarder, 20));
  //default_actions.push_back (ActionData (Rocketlauncher, 20));
  //default_actions.push_back (ActionData (Superman, 0));

  default_actions.push_back (ActionData (Basher, 20));
  default_actions.push_back (ActionData (Blocker, 20));
  default_actions.push_back (ActionData (Bomber, 20));

  default_actions.push_back (ActionData (Bridger, 20));
  default_actions.push_back (ActionData (Climber, 20));

  default_actions.push_back (ActionData (Digger, 20));
  default_actions.push_back (ActionData (Floater, 20));
  default_actions.push_back (ActionData (Jumper, 20));

  default_actions.push_back (ActionData (Miner, 20));
  default_actions.push_back (ActionData (Slider, 20));
}

/* EOF */
