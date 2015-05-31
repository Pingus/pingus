// Pingus - A free Lemmings clone
// Copyright (C) 1998-2009 Ingo Ruhnke <grumbel@gmail.com>
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

#include "action_name.hpp"

#include "gettext.h"

namespace ActionName {

std::string to_screenname(Enum action)
{
  switch (action)
  {
    case ANGEL          : return _("Angel");
    case BASHER         : return _("Basher");
    case BLOCKER        : return _("Blocker");
    case BOARDER        : return _("Boarder");
    case BOMBER         : return _("Bomber");
    case BRIDGER        : return _("Bridger");
    case CLIMBER        : return _("Climber");
    case DIGGER         : return _("Digger");
    case DROWN          : return _("Drown");
    case EXITER         : return _("Exiter");
    case FALLER         : return _("Faller");
    case FLOATER        : return _("Floater");
    case JUMPER         : return _("Jumper");
    case LASERKILL      : return _("Laserkill");
    case MINER          : return _("Miner");
    case SLIDER         : return _("Slider");
    case SMASHED        : return _("Smashed");
    case SPLASHED       : return _("Splashed");
    case SUPERMAN       : return _("Superman");
    case TELEPORTED     : return _("Teleported");
    case WAITER         : return _("Waiter");
    case WALKER         : return _("Walker");
    default             : return "Unknown ActionName";
  }
}

std::string to_string(Enum action)
{
  switch (action)
  {
    case ANGEL          : return "angel";
    case BASHER         : return "basher";
    case BLOCKER        : return "blocker";
    case BOARDER        : return "boarder";
    case BOMBER         : return "bomber";
    case BRIDGER        : return "bridger";
    case CLIMBER        : return "climber";
    case DIGGER         : return "digger";
    case DROWN          : return "drown";
    case EXITER         : return "exiter";
    case FALLER         : return "faller";
    case FLOATER        : return "floater";
    case JUMPER         : return "jumper";
    case LASERKILL      : return "laserkill";
    case MINER          : return "miner";
    case SLIDER         : return "slider";
    case SMASHED        : return "smashed";
    case SPLASHED       : return "splashed";
    case SUPERMAN       : return "superman";
    case TELEPORTED     : return "teleported";
    case WAITER         : return "waiter";
    case WALKER         : return "walker";
    default             : return "Unknown ActionName";
  }
}

Enum from_string(const std::string& action)
{
  if (action == "angel")          return ANGEL;
  if (action == "basher")         return BASHER;
  if (action == "blocker")        return BLOCKER;
  if (action == "boarder")        return BOARDER;
  if (action == "bomber")         return BOMBER;
  if (action == "bridger")        return BRIDGER;
  if (action == "climber")        return CLIMBER;
  if (action == "digger")         return DIGGER;
  if (action == "drown")          return DROWN;
  if (action == "exiter")         return EXITER;
  if (action == "faller")         return FALLER;
  if (action == "floater")        return FLOATER;
  if (action == "jumper")         return JUMPER;
  if (action == "laserkill")      return LASERKILL;
  if (action == "miner")          return MINER;
  if (action == "slider")         return SLIDER;
  if (action == "smashed")        return SMASHED;
  if (action == "splashed")       return SPLASHED;
  if (action == "superman")       return SUPERMAN;
  if (action == "teleported")     return TELEPORTED;
  if (action == "waiter")         return WAITER;
  if (action == "walker")         return WALKER;

  return WALKER;
}

} // namespace ActionName

/* EOF */
