//  $Id: game_session.hxx,v 1.11 2002/10/03 01:02:12 grumbel Exp $
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

#ifndef HEADER_PINGUS_GAME_SESSION_HXX
#define HEADER_PINGUS_GAME_SESSION_HXX

#include <string>
#include "screen.hxx"

class Client;
class Server;
class PLF;
class PingusGameSessionResult;
class DemoPlayer;

/** You can use this class to start up a game session, which consist
    of a single level. */
class PingusGameSession : public Screen
{
private:
  /// The filename of the level
  std::string filename;

  /// The level data
  PLF* plf;

  /// The server
  Server* server;

  /// The client
  Client* client;

  unsigned int last_redraw;
  unsigned int last_update;

  int left_over_time;

  /** Number of updates */
  unsigned int number_of_updates;

  /** Number of redraws (frameskip == number_of_updates - number_of_redraws) */
  unsigned int number_of_redraws;
  
public:
  /** Create a new game session which is launched on start ()
      @param arg_filename The filename of a Pingus Level file 
      FIXME: Absolute or relative filename? */
  PingusGameSession (std::string arg_filename);

  /** Clean up */
  ~PingusGameSession ();

  /** Get the results of the last gaming session */
  PingusGameSessionResult get_result ();

  // Overloaded Screen functions 
  /** Draw this screen */
  bool draw (GraphicContext& gc);

  /** Pass a delta to the screen */
  void update (const GameDelta& delta);

  void on_pause_press ();
  void on_fast_forward_press ();
  void on_armageddon_press ();
  void on_escape_press ();

private:  
  PingusGameSession (const PingusGameSession&);
  PingusGameSession& operator= (const PingusGameSession&);
};

#endif

/* EOF */
