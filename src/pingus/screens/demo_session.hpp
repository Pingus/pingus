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

#ifndef HEADER_PINGUS_PINGUS_SCREENS_DEMO_SESSION_HPP
#define HEADER_PINGUS_PINGUS_SCREENS_DEMO_SESSION_HPP

#include <memory>
#include <vector>

#include "engine/screen/gui_screen.hpp"
#include "pingus/server_event.hpp"
#include "util/pathname.hpp"

namespace Input {
struct Event;
struct ScrollEvent;
} // namespace Input

class Server;
class XMLPDF;
class DemoPlayer;
class PingusCounter;
class PingusDemo;
class Playfield;
class SmallMap;
class BButton;
class ButtonPanel;

/** A DemoSession is analog to a GameSession, but instead of loading a
    level and letting the player play a game, a demo file will be
    loaded and the level will be played automatically. */
class DemoSession : public GUIScreen
{
private:
  Pathname pathname;

  std::unique_ptr<Server>     server;
  std::unique_ptr<PingusDemo> demo;
  std::vector<ServerEvent>  events;

  PingusCounter* pcounter;
  Playfield*     playfield;
  SmallMap*      small_map;

  BButton* fastforward_button;
  BButton* pause_button;
  BButton* restart_button;

  bool pause;
  bool fast_forward;

public:
  /** @param filename the complete filename of the demo file */
  DemoSession(const Pathname& pathname);
  ~DemoSession();

  void draw_background(DrawingContext& gc);

  /** Pass a delta to the screen */
  void update(float delta);
  void update_demo();

  void on_pause_press ();
  void on_fast_forward_press ();
  void on_escape_press ();

  void restart();

  void on_scroller_move(float x, float y);

  bool is_pause() { return pause; }
  bool is_fast_forward() { return fast_forward; }

  void resize(const Size& size);

private:
  DemoSession (const DemoSession&);
  DemoSession& operator= (const DemoSession&);
};

#endif

/* EOF */
