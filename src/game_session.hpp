//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_GAME_SESSION_HPP
#define HEADER_PINGUS_GAME_SESSION_HPP

#include <string>
#include <memory>
#include "screen/gui_screen.hpp"
#include "pingus_level.hpp"
#include "pingu_enums.hpp"

namespace Input {
class ScrollEvent;
class AxisEvent;
class Event;
}

class ButtonPanel;
class PingusCounter;
class Playfield;
class TimeDisplay;
class SmallMap;
class Server;

/** You can use this class to start up a game session, which consist
    of a single level. */
class GameSession : public GUIScreen
{
private:
  /// The level data
  PingusLevel plf;

  bool show_result_screen;

  /// The server
  std::auto_ptr<Server> server;

  int world_delay; ///< how many milliseconds is the world behind the actual time

  // -- Client stuff
  bool is_finished;

  ButtonPanel*   button_panel;
  PingusCounter* pcounter;
  Playfield*     playfield;
  TimeDisplay*   time_display;
  SmallMap*      small_map;

  bool pause;
  bool fast_forward;

public:
  GameSession(const PingusLevel& arg_plf, bool arg_show_result_screen);
  ~GameSession ();

  /** Pass a delta to the screen */
  void update_server(float delta);

  Server*    get_server() { return server.get(); }
  Playfield* get_playfield() { return playfield; }

  void do_restart();
  bool finished();
  void set_finished();

  /** Update all parts of the world */
  void update (float delta);
  void update (const Input::Event& event);
  void draw_background (DrawingContext& gc);

  ButtonPanel* get_button_panel () { return button_panel; }

  // Overloaded GUIScreen stuff
  void on_startup ();
  void on_shutdown ();

  void on_pause_press ();
  void on_fast_forward_press ();
  void on_armageddon_press ();
  void on_escape_press ();
  void on_action_axis_move (float);

  Actions::ActionName get_action_name() const;

  void set_fast_forward(bool value);
  bool get_fast_forward() const;

  void set_pause(bool value);
  bool get_pause() const;

private:
  void process_scroll_event (const Input::ScrollEvent&);
  void process_axis_event (const Input::AxisEvent&);

  GameSession (const GameSession&);
  GameSession& operator= (const GameSession&);
};


#endif

/* EOF */
