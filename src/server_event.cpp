
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

#include <assert.h>
#include <iostream>
#include <boost/format.hpp>

#include "pingus_error.hpp"
#include "server.hpp"
#include "world.hpp"
#include "pingu_holder.hpp"
#include "pingu.hpp"
#include "string_util.hpp"

static char num2hex[] = "0123456789abcdef";

/** Write out the raw bits of a float as hex */
static std::string float2string(float value)
{
  std::string str(2*sizeof(float), '0');

  for(size_t i = 0; i < sizeof(float); ++i)
    {
      char v = reinterpret_cast<char*>(&value)[i];
      str[2*i + 0] = num2hex[(v & 0xf0) >> 4];
      str[2*i + 1] = num2hex[v & 0x0f];
    }
  return str;
}

static char hex2int(char c)
{
  if (c >= '0' && c <= '9')
    return c - '0';
  else if (c >= 'a' && c <= 'f')
    return c - 'a' + 0xa;
  else
    return 0;    
}

/** Restore the raw bits of a float from a string */
static float string2float(const std::string& str)
{
  assert(str.size() == 2*sizeof(float));

  float value;
  for(size_t i = 0; i < sizeof(float); ++i)
    {
      char& v = reinterpret_cast<char*>(&value)[i];
      v = (hex2int(str[2*i+0]) << 4) | hex2int(str[2*i+1]);
    }

  return value;
}

ServerEvent::ServerEvent() 
  : type(PINGU_ACTION_EVENT),
    time_stamp(0),
    pingu_id(0),
    pingu_action(Actions::Walker)
{
}

ServerEvent::ServerEvent(FileReader reader)
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
      reader.read_string("raw-x", raw_x);
      reader.read_string("raw-y", raw_y);
      reader.read_enum("action", pingu_action, Actions::action_from_string);

      pos.x = string2float(raw_x);
      pos.y = string2float(raw_y);
    }
  else
    {
      PingusError::raise(std::string("ServerEvent: Parse error: Unknown event: ")
			 + reader.get_name());
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
            << "(raw-x \"" << float2string(pos.x) << "\") "
            << "(raw-y \"" << float2string(pos.y) << "\") "
            << "(action \"" << Actions::action_to_string(pingu_action) << "\"))"
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
ServerEvent::make_pingu_action_event(int t, int id, const Vector3f& pos, Actions::ActionName action)
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
                  std::cout << "ServerEvent: DemoFile inconsistent with world, pingu " << pingu_id << " is at the wrong position" << std::endl;
                }

              server->send_pingu_action_event(pingu, pingu_action);
            }
          else
            {
              std::cout << "ServerEvent: DemoFile inconsistent with world, pingu " << pingu_id << " missing" << std::endl;
            }
        }
        break;

      default:
        assert(!"Unknown type");
    }
}

/* EOF */
