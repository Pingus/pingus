// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_SCREENS_GAME_SESSION_HPP
#define HEADER_PINGUS_PINGUS_SCREENS_GAME_SESSION_HPP

#include <memory>

#include "engine/screen/gui_screen.hpp"
#include "pingus/action_name.hpp"
#include "pingus/pingus_level.hpp"
#include "fwd.hpp"

namespace pingus {

/** You can use this class to start up a game session, which consist
    of a single level. */
class GameSession : public GUIScreen
{
private:
  /// The level data
  PingusLevel plf;

  bool show_result_screen;

  /// The server
  std::unique_ptr<Server> server;

  int world_delay; ///< how many milliseconds is the world behind the actual time

  // -- Client stuff
  ButtonPanel*   button_panel;
  PingusCounter* pcounter;
  Playfield*     playfield;
  SmallMap*      small_map;

  ArmageddonButton* armageddon_button;
  ForwardButton*    forward_button;
  PauseButton*      pause_button;

  bool pause;
  bool fast_forward;
  bool single_step;

public:
  GameSession(PingusLevel const& arg_plf, bool arg_show_result_screen);
  ~GameSession() override;

  /** Pass a delta to the screen */
  void update_server(float delta);

  Server*    get_server() { return server.get(); }
  Playfield* get_playfield() { return playfield; }

  bool finished();

  /** Update all parts of the world */
  void update (float delta) override;
  void update_input(pingus::input::Event const& event) override;
  void draw_background (DrawingContext& gc) override;

  ButtonPanel* get_button_panel() { return button_panel; }

  // Overloaded GUIScreen stuff
  void on_startup() override;

  void on_pause_press() override;
  void on_single_step_press() override;
  void on_fast_forward_press() override;
  void on_fast_forward_release() override;
  void on_armageddon_press() override;
  void on_escape_press() override;
  void on_action_axis_move (float) override;

  ActionName::Enum get_action_name() const;

  void set_fast_forward(bool value);
  bool get_fast_forward() const;

  void set_pause(bool value);
  bool get_pause() const;

  void resize(Size const&) override;

private:
  void process_scroll_event (pingus::input::ScrollEvent const&);
  void process_axis_event (pingus::input::AxisEvent const&);

private:
  GameSession (GameSession const&);
  GameSession& operator= (GameSession const&);
};

} // namespace pingus

#endif

/* EOF */
