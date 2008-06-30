//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_GAME_SESSION_HPP
#define HEADER_PINGUS_GAME_SESSION_HPP

#include <string>
#include <memory>
#include "screen/screen.hpp"
#include "pingus_level.hpp"

class Client;
class Server;

/** You can use this class to start up a game session, which consist
    of a single level. */
class GameSession : public Screen
{
private:
  /// The level data
  PingusLevel plf;

  bool show_result_screen;

  /// The server
  std::auto_ptr<Server> server;

  /// The client
  std::auto_ptr<Client> client;

  int world_delay; ///< how many milliseconds is the world behind the actual time

public:
  GameSession(const PingusLevel& arg_plf, bool arg_show_result_screen);

  /** Clean up */
  ~GameSession ();

  // Overloaded Screen functions
  /** Draw this screen */
  bool draw(DrawingContext& gc);

  /** Pass a delta to the screen */
  void update (const GameDelta& delta);

  void on_startup();
  void on_shutdown();

  void on_pause_press ();
  void on_fast_forward_press ();
  void on_armageddon_press ();
  void on_escape_press ();

private:
  GameSession (const GameSession&);
  GameSession& operator= (const GameSession&);
};


#endif

/* EOF */
