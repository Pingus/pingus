//  $Id: Client.hh,v 1.9 2000/04/24 13:15:41 grumbel Exp $
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

#ifndef CLIENT_HH
#define CLIENT_HH

#include <string>

#include "Server.hh"
#include "Result.hh"
#include "ClientEvent.hh"

#include "ButtonPanel.hh"
#include "TimeDisplay.hh"
#include "PinguInfo.hh"
#include "PingusCounter.hh"
#include "SmallMap.hh"

class ClientEvent;
class Playfield;
class ButtonPanel;
class PingusCounter;

class Client
{
private:
  friend ClientEvent;
  ClientEvent* event;

  PLF* plf;
  Result result;
  Server* server;
  PingusGameMode       mode;
  bool fast_forward;
  bool pause;
  int  skip_frame;
  int  current_fps;
  bool do_replay;
  bool is_finished;

  std::vector<GuiObj* > obj;
 
  ButtonPanel*   button_panel;
  PingusCounter* pcounter;
  Playfield*     playfield;
  TimeDisplay*   time_display;
  SmallMap*      small_map;

public:
  Client(Server* s);

  Server*    get_server() { return server; }
  Playfield* get_playfield() { return playfield; }

  void start(std::string filename, PingusGameMode m = (PingusGameMode)INTERACTIVE_MODE);
  void start(std::string plf_filename, std::string psm_filename);
  void play_level(std::string plf_filename, std::string psm_filename = "");

  void init_display();
  void deinit_display();
  void resize_display();

  void set_fast_forward(bool value);
  bool get_fast_forward();

  bool get_pause();
  void set_pause(bool value);

  void count_fps();
  int  get_fps();
  bool replay();
  void do_restart();
  bool finished();
  void set_finished();
  Result get_result();
};

#endif

/* EOF */
