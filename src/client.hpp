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

#ifndef HEADER_PINGUS_CLIENT_HPP
#define HEADER_PINGUS_CLIENT_HPP

#include "screen/gui_screen.hpp"

namespace Input {
struct ButtonEvent;
struct PointerEvent;
struct ScrollEvent;
struct AxisEvent;
struct KeyboardEvent;
} // namespace Input

namespace GUI {
class RootGUIManager;
} // namespace GUI

class Vector;
class ButtonPanel;
class Cursor;
class HurryUp;
class PingusCounter;
class Playfield;
class Server;
class SmallMap;
class TimeDisplay;

/** FIXME: This whole Server/Client concept is a bit screwed */
class Client : public GUIScreen
{
private:
  Server* server;

  int  skip_frame;
  bool do_replay;
  bool is_finished;

  ButtonPanel*   button_panel;
  PingusCounter* pcounter;
  Playfield*     playfield;
  TimeDisplay*   time_display;
  SmallMap*      small_map;
  bool enabled;

public:
  Client(Server* s);
  virtual ~Client();

  Server* get_server() { return server; }
  Playfield* get_playfield() { return playfield; }

  bool replay();
  void do_restart();
  bool finished();
  void set_finished();

  /** Update all parts of the world */
  void update (const GameDelta&);
  void draw_background (DrawingContext& gc);
  //void process_events ();

  ButtonPanel* get_button_panel () { return button_panel; }

  // Overloaded GUIScreen stuff
  void on_startup ();
  void on_shutdown ();

  void on_pause_press ();
  void on_fast_forward_press ();
  void on_armageddon_press ();
  void on_escape_press ();
  void on_action_axis_move (float);

private:
  void process_events (const GameDelta& events);
  void process_scroll_event (const Input::ScrollEvent&);
  void process_axis_event (const Input::AxisEvent&);

  Client (const Client&);
  Client& operator= (const Client&);
};


#endif

/* EOF */
