//  $Id: server_event.cxx,v 1.9 2003/10/18 23:17:27 grumbel Exp $
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

#include <assert.h>
#include <iostream>
#include <ClanLib/Core/System/clanstring.h>
#include "pingus_error.hxx"
#include "server.hxx"
#include "world.hxx"
#include "pingu_holder.hxx"

namespace Pingus {

ServerEvent::ServerEvent() :
	type(PINGU_ACTION_EVENT),
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
  else if (reader.get_name() == "pingu-action")
    {
      type = PINGU_ACTION_EVENT;
      reader.read_int ("time",   time_stamp);
      reader.read_int ("id",     pingu_id);
      reader.read_enum("action", pingu_action, Actions::action_from_string);
    }
  else
    {
      PingusError::raise(std::string("ServerEvent: Parse error: Unknown event: ")
			 + reader.get_name());
    }
}

void
ServerEvent::write_xml(std::ostream& xml) const
{
  switch(type)
    {
    case ARMAGEDDON_EVENT:
      xml << "  <armageddon time=\"" << CL_String::to(time_stamp) << "\"/>" << std::endl;
      break;
    case PINGU_ACTION_EVENT:
      xml << "  <pingu-action time=\"" << CL_String::to(time_stamp) << "\" id=\"" << pingu_id
	  << "\" action=\"" << Actions::action_to_string(pingu_action) << "\"/>" << std::endl;
      break;
    default:
      assert(!"Unknown type");
    }
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
ServerEvent::make_pingu_action_event(int t, int id, Actions::ActionName action)
{
  ServerEvent event;
  event.type         = PINGU_ACTION_EVENT;
  event.time_stamp   = t;
  event.pingu_id     = id;
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
    case PINGU_ACTION_EVENT:
      {
	Pingu* pingu = server->get_world()->get_pingus()->get_pingu(pingu_id);
	if (pingu)
	  {
	    server->send_pingu_action_event(pingu,
					    pingu_action);
	  }
	else
	  {
	    std::cout << "ServerEvent: DemoFile inconsistent with world" << std::endl;
	  }
      }
      break;
    default:
      assert(!"Unknown type");
    }
}

} // namespace Pingus

/* EOF */
