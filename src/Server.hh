//  $Id: Server.hh,v 1.13 2001/04/13 13:45:09 grumbel Exp $
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
#include "DemoRecorder.hh"

/** An Event in the Pingus World

    Possible events are the applying of an action, an armageddon,
    scrolling, etc. */
class PingusEvent {
public:
  ///
  PingusEvent();
  ///
  PingusEvent(std::string);

  /// The GameTime at which the event happend
  int          game_time;
  /// A string describing the event
  std::string  str;
};

/** A abstract server-like class */
class Server
{
protected:
  ///
  World* world;
  ///
  ActionHolder action_holder;
  ///
  bool demo_mode;
  ///
  std::string demo_file;

  /// The name of the current level file
  std::string filename;

  ///
  bool get_next_event;
  ///
  DemoRecorder recorder;
  ///
  bool finished;

public:
  ///
  Server();
  ///
  virtual ~Server();

  ///
  virtual bool enough_time_passed() = 0;
  ///
  virtual void update(float delta);
  ///
  virtual void start(boost::shared_ptr<PLF> level_data) {}

  ///
  virtual void set_fast_forward(bool) = 0;
  ///
  virtual bool get_fast_forward() = 0;

  ///
  virtual bool get_pause() = 0;
  ///
  virtual void set_pause(bool) = 0;

  ///
  virtual bool needs_redraw() = 0;

  ///
  virtual boost::shared_ptr<PLF> get_plf () =0;

  ///
  World* get_world();
  ///
  ActionHolder* get_action_holder();
  ///
  bool is_finished();
  ///
  void set_finished();
  ///
  void process_event(std::string);
  ///
  void send_event(std::string);
  ///
  void set_demo(std::string);
  ///
  void record_demo();
};

#endif

/* EOF */
