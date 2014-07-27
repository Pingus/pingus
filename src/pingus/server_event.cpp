//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include <boost/format.hpp>

#include "pingus/pingu.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/server.hpp"
#include "pingus/world.hpp"
#include "util/file_reader.hpp"
#include "util/log.hpp"
#include "util/raise_exception.hpp"

ServerEvent::ServerEvent() :
  type(PINGU_ACTION_EVENT),
  time_stamp(0),
  pingu_id(0),
  pos(),
  pingu_action(ActionName::WALKER)
{
}

ServerEvent::ServerEvent(const FileReader& reader) :
  type(PINGU_ACTION_EVENT),
  time_stamp(0),
  pingu_id(0),
  pos(),
  pingu_action(ActionName::WALKER)
{
  if (reader.get_name() == "armageddon")
  {
    type = ARMAGEDDON_EVENT;
    reader.read_int("time", time_stamp);
  }
  else if (reader.get_name() == "end")
  {
    type = END_EVENT;
    reader.read_int("time", time_stamp);
  }
  else if (reader.get_name() == "finish")
  {
    type = FINISH_EVENT;
    reader.read_int("time", time_stamp);
  }
  else if (reader.get_name() == "pingu-action")
  {
    std::string raw_x;
    std::string raw_y;

    type = PINGU_ACTION_EVENT;
    reader.read_int ("time",   time_stamp);
    reader.read_int ("id",     pingu_id);

    if (reader.read_string("raw-x", raw_x))
      pos.x = Math::string2float(raw_x);

    if (reader.read_string("raw-y", raw_y))
      pos.y = Math::string2float(raw_y);

    reader.read_enum("action", pingu_action, &ActionName::from_string);

      
  }
  else
  {
    raise_exception(std::runtime_error, "ServerEvent: Parse error: Unknown event: " << reader.get_name());
  }
}

void
ServerEvent::write(std::ostream& out) const
{
  switch(type)
  {
    case ARMAGEDDON_EVENT:
      out << "(armageddon (time " << time_stamp << "))" << std::endl;
      break;

    case FINISH_EVENT:
      out << "(finish (time " << time_stamp << "))" << std::endl;
      break;

    case PINGU_ACTION_EVENT:
      out << "(pingu-action "
          << "(time " << time_stamp << ") "
          << "(id " << pingu_id << ") "
          << "(raw-x \"" << Math::float2string(pos.x) << "\") "
          << "(raw-y \"" << Math::float2string(pos.y) << "\") "
          << "(action \"" << ActionName::to_string(pingu_action) << "\"))"
          << std::endl;
      break;

    default:
      assert(!"Unknown type");
  }
}

ServerEvent
ServerEvent::make_finish_event(int t)
{
  ServerEvent event;
  event.type       = FINISH_EVENT;
  event.time_stamp = t;
  return event; 
}

ServerEvent
ServerEvent::make_end_event(int t)
{
  ServerEvent event;
  event.type       = END_EVENT;
  event.time_stamp = t;
  return event; 
}

ServerEvent
ServerEvent::make_armageddon_event(int t)
{
  ServerEvent event;
  event.type       = ARMAGEDDON_EVENT;
  event.time_stamp = t;
  return event;
}

ServerEvent
ServerEvent::make_pingu_action_event(int t, int id, const Vector3f& pos, ActionName::Enum action)
{
  ServerEvent event;
  event.type         = PINGU_ACTION_EVENT;
  event.time_stamp   = t;
  event.pingu_id     = id;
  event.pos          = pos;
  event.pingu_action = action;
  return event;
}

void
ServerEvent::send(Server* server)
{
  switch(type)
  {
    case ARMAGEDDON_EVENT:
      server->send_armageddon_event();
      break;

    case FINISH_EVENT:
      server->send_finish_event();      
      break;

    case END_EVENT:
      // do nothing
      break;

    case PINGU_ACTION_EVENT:
    {
      Pingu* pingu = server->get_world()->get_pingus()->get_pingu(pingu_id);
      if (pingu)
      {
        if (pos.x != pingu->get_pos().x ||
            pos.y != pingu->get_pos().y)
        {
          log_error("DemoFile inconsistent with world, pingu %1% is at the wrong position", pingu_id);
        }

        server->send_pingu_action_event(pingu, pingu_action);
      }
      else
      {
        log_error("DemoFile inconsistent with world, pingu %1% missing", pingu_id);
      }
    }
    break;

    default:
      assert(!"Unknown type");
  }
}

/* EOF */
