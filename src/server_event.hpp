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

#ifndef HEADER_PINGUS_SERVER_EVENT_HPP
#define HEADER_PINGUS_SERVER_EVENT_HPP

#include <iosfwd>
#include "file_reader.hpp"
#include "pingu_enums.hpp"
#include "math/vector3f.hpp"

class Server;

/** This class represents an event that the Server can recieve from
    the client. Only actions and armageddon are here, the rest happens
    Client internal and the server knows nothing about it (scrolling,
    etc.) FIXME: this is only half true... */
class ServerEvent
{
public:
  enum Type { ARMAGEDDON_EVENT,
              FINISH_EVENT,
              END_EVENT,
	      PINGU_ACTION_EVENT };

  /** The type of event */
  Type type;

  /** the time at which the event should take place */
  int time_stamp;

  // stuff for pingu_action_event

  /** Id of the pingu which should get the actions */
  int pingu_id;

  Vector3f pos;

  /** action name */
  Actions::ActionName pingu_action;

  ServerEvent();

  /** Construct an server event from an xml subtree */
  ServerEvent(FileReader reader);

  void write(std::ostream& out) const;

  /** Send this event to the server */
  void send(Server*);

  // Pseudo constructors
  static ServerEvent make_end_event(int time);
  static ServerEvent make_finish_event(int time);
  static ServerEvent make_armageddon_event(int time);
  static ServerEvent make_pingu_action_event(int t, int id, const Vector3f& pos, Actions::ActionName action);
};


#endif

/* EOF */
