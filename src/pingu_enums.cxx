//  $Id: pingu_enums.cxx,v 1.5 2003/04/18 09:22:28 torangan Exp $
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

#include "my_gettext.hxx"
#include "pingu_enums.hxx"

// Pingu "globals".  Make [deadly_velocity = 20 * sqrt("normal gravity")] so
// that the "deadly distance" is the same and therefore doesn't break levels.
const float deadly_velocity = 10.0f;
const int pingu_height = 26;

namespace Actions {

  std::string action_to_string (ActionName action, NameType choice) {

    switch (action) {

      case Angel          : return (choice == ENGLISH) ? "angel"          : _("Angel");
      case Basher         : return (choice == ENGLISH) ? "basher"         : _("Basher");
      case Blocker        : return (choice == ENGLISH) ? "blocker"        : _("Blocker");
      case Boarder        : return (choice == ENGLISH) ? "boarder"        : _("Boarder");
      case Bomber         : return (choice == ENGLISH) ? "bomber"         : _("Bomber");
      case Bridger        : return (choice == ENGLISH) ? "bridger"        : _("Bridger");
      case Climber        : return (choice == ENGLISH) ? "climber"        : _("Climber");
      case Digger         : return (choice == ENGLISH) ? "digger"         : _("Digger");
      case Drown          : return (choice == ENGLISH) ? "drown"          : _("Drown");
      case Exiter         : return (choice == ENGLISH) ? "exiter"         : _("Exiter");
      case Faller         : return (choice == ENGLISH) ? "faller"         : _("Faller");
      case Floater        : return (choice == ENGLISH) ? "floater"        : _("Floater");
      case Jumper         : return (choice == ENGLISH) ? "jumper"         : _("Jumper");
      case Laserkill      : return (choice == ENGLISH) ? "laserkill"      : _("Laserkill");
      case Miner          : return (choice == ENGLISH) ? "miner"          : _("Miner");
      case Rocketlauncher : return (choice == ENGLISH) ? "rocketlauncher" : _("Rocketlauncher");
      case Slider         : return (choice == ENGLISH) ? "slider"         : _("Slider");
      case Smashed        : return (choice == ENGLISH) ? "smashed"        : _("Smashed");
      case Splashed       : return (choice == ENGLISH) ? "splashed"       : _("Splashed");
      case Superman       : return (choice == ENGLISH) ? "superman"       : _("Superman");
      case Teleported     : return (choice == ENGLISH) ? "teleported"     : _("Teleported");
      case Waiter         : return (choice == ENGLISH) ? "waiter"         : _("Waiter");
      case Walker         : return (choice == ENGLISH) ? "walker"         : _("Walker");
      default             : return "Unknown ActionName";
   }
  }

  ActionName action_from_string(const std::string& action) {

    if (action == "angel")          return Angel;
    if (action == "basher")         return Basher;
    if (action == "blocker")        return Blocker;
    if (action == "boarder")        return Boarder;
    if (action == "bomber")         return Bomber;
    if (action == "bridger")        return Bridger;
    if (action == "climber")        return Climber;
    if (action == "digger")         return Digger;
    if (action == "drown")          return Drown;
    if (action == "exiter")         return Exiter;
    if (action == "faller")         return Faller;
    if (action == "floater")        return Floater;
    if (action == "jumper")         return Jumper;
    if (action == "laserkill")      return Laserkill;
    if (action == "miner")          return Miner;
    if (action == "rocketlauncher") return Rocketlauncher;
    if (action == "slider")         return Slider;
    if (action == "smashed")        return Smashed;
    if (action == "splashed")       return Splashed;
    if (action == "superman")       return Superman;
    if (action == "teleported")     return Teleported;
    if (action == "waiter")         return Waiter;
    if (action == "walker")         return Walker;

    return Walker;
  }

}

/* EOF */
