//  $Id: client.hxx,v 1.19 2002/09/28 11:52:21 torangan Exp $
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

#ifndef HEADER_PINGUS_CLIENT_HXX
#define HEADER_PINGUS_CLIENT_HXX

#include <ClanLib/Signals/slot.h>
#include <ClanLib/Display/Input/inputdevice.h>
#include <vector>
#include "result.hxx"
#include "gui_screen.hxx"
#include "sprite.hxx"

namespace Input {
class Controller;
class ButtonEvent;
class PointerEvent;
class AxisEvent;
} // namespace Input

namespace GUI {
class RootGUIManager;
} // namespace GUI

class CL_Key;
class Vector;
class ButtonPanel;
class Controller;
class Cursor;
class HurryUp;
class PLF;
class PingusCounter;
class Playfield;
class Server;
class SmallMap;
class TimeDisplay;

/** FIXME: This whole Server/Client concept is a bit screwed */
class Client : public GUIScreen
{
private:
  Result result;
  Server* server;

  int  skip_frame;
  bool do_replay;
  bool is_finished;

  Sprite unplayable;

  ButtonPanel*   button_panel;
  PingusCounter* pcounter;
  Playfield*     playfield;
  TimeDisplay*   time_display;
  SmallMap*      small_map;
  HurryUp*       hurry_up;

#if 0  
  void process_button_event (Input::ButtonEvent*);
  void process_pointer_event (Input::PointerEvent*);
  void process_axis_event (Input::AxisEvent*);
#endif 

  bool enabled;
public:
  Client(Server * s);
  virtual ~Client();

  Server* get_server() { return server; }
  Playfield* get_playfield() { return playfield; }

  bool replay();
  void do_restart();
  bool finished();
  void set_finished();

  /** Update all parts of the world */
  void update (float delta);
  void update (const GameDelta&);

  //void process_events ();

  ButtonPanel* get_button_panel () { return button_panel; }

  void on_primary_button_press(int x, int y);
  void on_primary_button_release(int x, int y);

  void on_butmouse_button_press(const CL_Key &key);
  void on_mouse_button_release(const CL_Key &key);

  // Overloaded GUIScreen stuff
  void draw (GraphicContext& gc);
  void on_startup ();
  void on_shutdown ();

  void on_pause_press ();
  void on_fast_forward_press ();
  void on_armageddon_press ();
  void on_escape_press ();
  void on_action_axis_move (float);

private:
  Client (const Client&);
  Client& operator= (const Client&);
};

#endif

/* EOF */
