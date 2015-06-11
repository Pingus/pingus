// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_SERVER_EVENT_HPP
#define HEADER_PINGUS_PINGUS_SERVER_EVENT_HPP

#include "math/vector3f.hpp"
#include "pingus/action_name.hpp"

class ReaderMapping;
class ReaderObject;
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
  unsigned int pingu_id;

  Vector3f pos;

  /** action name */
  ActionName::Enum pingu_action;

  ServerEvent();

  /** Construct an server event from an xml subtree */
  ServerEvent(const ReaderObject& reader);

  void write(std::ostream& out) const;

  /** Send this event to the server */
  void send(Server*);

  // Pseudo constructors

  /** The end event marks the very end of the demo, i.e. the time the
      server did run its last update() */
  static ServerEvent make_end_event(int time);

  /** The finish event is triggered when the user leave the level by
      pressing escape */
  static ServerEvent make_finish_event(int time);

  /** The armageddon event is triggered when the user presses the
      armageddon button */
  static ServerEvent make_armageddon_event(int time);

  /** The pingu action event is triggered whenever the user applies an
      action to a Pingu */
  static ServerEvent make_pingu_action_event(int t, unsigned int id, const Vector3f& pos, ActionName::Enum action);
};

#endif

/* EOF */
