//  $Id: pingu_enums.hxx,v 1.7 2002/08/25 09:08:48 torangan Exp $
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

#ifndef HEADER_PINGUS_PINGU_ENUMS_HXX
#define HEADER_PINGUS_PINGU_ENUMS_HXX

#include "pingus.hxx"
#include <string>

/** Haven't yet experimented much with pingu status, but maybe it is a
    good idea. Maybe that should be changed to a bitmask. */
enum PinguStatus { PS_ALIVE, PS_EXITED, PS_DEAD };

enum PingusGameMode { 
  INTERACTIVE_MODE,
  DEMO_MODE 
};

namespace Actions {
    
  enum ActionName { Angel,
                    Basher,
                    Blocker,
                    Boarder,
                    Bomber,
                    Bridger,
                    Climber,
                    Digger,
                    Drown,
                    Exiter,
                    Faller,
                    Floater,
                    Jumper,
                    Laserkill,
                    Miner,
                    Rocketlauncher,
                    Slider,
                    Smashed,
                    Splashed,
                    Superman,
                    Teleported,
                    Waiter,
                    Walker
               };
     
  std::string action_to_string (ActionName action);
  ActionName  action_from_string (const std::string& action);
}


#endif


/* EOF */
