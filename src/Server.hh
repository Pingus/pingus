//  $Id: Server.hh,v 1.3 2000/02/11 16:58:26 grumbel Exp $
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

#ifndef SERVER_HH
#define SERVER_HH

#include <string>
#include <fstream>
#include "World.hh"
#include "ActionHolder.hh"
#include "PingusDemo.hh"

class Server
{
protected:
  World* world;
  ActionHolder action_holder;
  bool demo_mode;
  std::string demo_file;
  bool get_next_event;
  PingusDemo demo_out;
  PingusDemo* demo_in;
  bool finished;
  
public:
  Server();
  virtual ~Server();

  virtual bool enough_time_passed() = 0;
  virtual void let_move();
  virtual void start(PLF* level_data) {};

  virtual void set_fast_forward(bool) = 0;
  virtual bool get_fast_forward() = 0;

  virtual bool get_pause() = 0;
  virtual void set_pause(bool) = 0;

  virtual bool needs_redraw() = 0;

  World* get_world();
  ActionHolder* get_action_holder();
  bool is_finished();
  void set_finished();
  void process_event(string);
  void send_event(string);
  void set_demo(string);
  void set_record_file(string);
};

#endif

/* EOF */
