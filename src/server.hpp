//  $Id$
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

#ifndef HEADER_PINGUS_SERVER_HPP
#define HEADER_PINGUS_SERVER_HPP

#include "server_event.hpp"
#include "action_holder.hpp"
#include "pingus_level.hpp"


class Pingu;
class World;
class DemoRecorder;
class GoalManager;

/** A abstract server-like class */
class Server
{
protected:
  PingusLevel plf;
  World* world;

  /** Manager class for the number of available actions */
  ActionHolder action_holder;

  GoalManager* goal_manager;

  DemoRecorder* demo_recorder;

  bool fast_forward;
  bool pause;

public:
  Server(const PingusLevel& arg_plf);
  ~Server();

  void update();

  PingusLevel get_plf () { return plf; }

  int get_time();

  World* get_world();
  ActionHolder* get_action_holder();

  /** @return true if the server is finished and the game can be
      exited */
  bool is_finished();

  /** set the server into the finshed state, this is used when you
      press ESCAPE inside a game */
  void set_finished();

  /* Event handling stuff */
  void send_armageddon_event();
  void send_pingu_action_event(Pingu* pingu, Actions::ActionName action);

  void set_fast_forward(bool value);
  bool get_fast_forward();

  void set_pause(bool);
  bool get_pause();

private:
  Server (const Server&);
  Server& operator= (const Server&);
};


#endif

/* EOF */
