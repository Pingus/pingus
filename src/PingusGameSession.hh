//  $Id: PingusGameSession.hh,v 1.4 2001/04/15 17:01:51 grumbel Exp $
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

#ifndef PINGUSGAMESESSION_HH
#define PINGUSGAMESESSION_HH

#include <string>

#include "Server.hh"
#include "Client.hh"

/** With this class you can obtain information about the success or
    failure of a level, in addition you can get details like how many
    penguins where saved or killed, etc. */
class PingusGameSessionResults 
{
public:
  /** Gives information if the level was succeeded or not 
      @return true if the level was succeeded, otherwise false */
  bool finished ();

  /** @return The percentage of pingus which where saved in this level */
  float percentage_saved ();

  /** @return The percentage of time, which was used to finish this level */
  float percentage_time ();
};

/** You can use this class to start up a game session, which consist
    of a single level. */
class PingusGameSession
{
private:
  /// The filename of the level
  std::string filename;

  /// The level data
  boost::shared_ptr<PLF> plf;

  /// The server
  boost::shared_ptr<Server> server;

  /// The client
  boost::shared_ptr<Client> client;

public:
  /** Create a new game session which is launched on start ()
      @param filename The filename of a Pingus Level file */
  PingusGameSession (std::string arg_filename);

  /** Clean up */
  ~PingusGameSession ();

  /** Start a game session, create Server/Client objects as needed */
  void start ();

  /** Get the results of the last gaming session */
  PingusGameSessionResults get_results ();
};

#endif

/* EOF */
