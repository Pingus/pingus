//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "gettext.h"
#include "pingu_enums.hpp"

// Pingu "globals".  Make [deadly_velocity = 20 * sqrt("normal gravity")] so
// that the "deadly distance" is the same and therefore doesn't break levels.
const float deadly_velocity = 10.0f;
const int pingu_height = 26;

namespace Actions {

std::string action_to_screenname (ActionName action)
{
  switch (action)
    {
    case Angel          : return _("Angel");
    case Basher         : return _("Basher");
    case Blocker        : return _("Blocker");
    case Boarder        : return _("Boarder");
    case Bomber         : return _("Bomber");
    case Bridger        : return _("Bridger");
    case Climber        : return _("Climber");
    case Digger         : return _("Digger");
    case Drown          : return _("Drown");
    case Exiter         : return _("Exiter");
    case Faller         : return _("Faller");
    case Floater        : return _("Floater");
    case Jumper         : return _("Jumper");
    case Laserkill      : return _("Laserkill");
    case Miner          : return _("Miner");
    case Rocketlauncher : return _("Rocketlauncher");
    case Slider         : return _("Slider");
    case Smashed        : return _("Smashed");
    case Splashed       : return _("Splashed");
    case Superman       : return _("Superman");
    case Teleported     : return _("Teleported");
    case Waiter         : return _("Waiter");
    case Walker         : return _("Walker");
    default             : return "Unknown ActionName";
    }
}

std::string action_to_string(ActionName action)
{
  switch (action)
    {
    case Angel          : return "angel";
    case Basher         : return "basher";
    case Blocker        : return "blocker";
    case Boarder        : return "boarder";
    case Bomber         : return "bomber";
    case Bridger        : return "bridger";
    case Climber        : return "climber";
    case Digger         : return "digger";
    case Drown          : return "drown";
    case Exiter         : return "exiter";
    case Faller         : return "faller";
    case Floater        : return "floater";
    case Jumper         : return "jumper";
    case Laserkill      : return "laserkill";
    case Miner          : return "miner";
    case Rocketlauncher : return "rocketlauncher";
    case Slider         : return "slider";
    case Smashed        : return "smashed";
    case Splashed       : return "splashed";
    case Superman       : return "superman";
    case Teleported     : return "teleported";
    case Waiter         : return "waiter";
    case Walker         : return "walker";
    default             : return "Unknown ActionName";
    }
}

ActionName action_from_string(const std::string& action)
{
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

} // namespace Actions

/* EOF */
