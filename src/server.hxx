//  $Id: server.hxx,v 1.9 2002/10/02 19:20:19 grumbel Exp $
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

#ifndef HEADER_PINGUS_SERVER_HXX
#define HEADER_PINGUS_SERVER_HXX

#include "action_holder.hxx"

class Pingu;
class PLF;
class World;

/** An Event in the Pingus World

    Possible events are the applying of an action, an armageddon,
    scrolling, etc.

    FIXME: The concept is ok, but implementation sucks 
*/
class PingusEvent {
public:
  /// The GameTime at which the event happend
  int          game_time;
  /// A string describing the event
  std::string  str;

public:
  PingusEvent ();
  PingusEvent (const std::string& event_str);
  
  PingusEvent (const PingusEvent& old);
  PingusEvent& operator= (const PingusEvent&);
};

/** A abstract server-like class */
class Server
{
protected:
  World* world;
  ActionHolder action_holder;
  bool demo_mode;
  std::string demo_file;

  bool get_next_event;
  bool finished;

public:
  Server(PLF*);
  virtual ~Server();

  virtual void update();
  
  virtual void set_fast_forward(bool) = 0;
  virtual bool get_fast_forward() = 0;

  virtual bool get_pause() = 0;
  virtual void set_pause(bool) = 0;

  virtual void set_armageddon () =0;
  virtual bool get_armageddon () =0;

  virtual PLF* get_plf () =0;

  World* get_world();
  ActionHolder* get_action_holder();
  bool is_finished();
  void set_finished();

  /* Event handling stuff */
  void send_armageddon_event();
  void send_pingu_action_event(Pingu* pingu, Actions::ActionName action);
  
private:
  Server (const Server&);
  Server& operator= (const Server&);
};

#endif

/* EOF */
