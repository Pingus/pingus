//  $Id: game_session.cxx,v 1.12 2002/10/01 19:53:44 grumbel Exp $
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

#include <iostream>
#include "client.hxx"
#include "true_server.hxx"
#include "game_session.hxx"
#include "game_session_result.hxx"
#include "timer.hxx"
#include "plf.hxx"

PingusGameSession::PingusGameSession (std::string arg_filename)
  : filename (arg_filename)
{
  Timer timer;
  
  timer.start();
  plf    = PLF::create(filename);
  std::cout << "Timer: Level loading took: " << timer.stop() << std::endl;

  timer.start();
  server = new TrueServer(plf);
  std::cout << "Timer: TrueServer creation took: " << timer.stop() << std::endl;

  timer.start();
  client = new Client(server);
  std::cout << "Timer: Client creation took: " << timer.stop() << std::endl;
}

PingusGameSession::~PingusGameSession ()
{
  delete client;
  delete server;
  delete plf;
}

PingusGameSessionResult
PingusGameSession::get_result ()
{
  return PingusGameSessionResult ();
}

void
PingusGameSession::draw (GraphicContext& gc)
{
  client->draw (gc);
}

void
PingusGameSession::update (const GameDelta& delta)
{
  server->update ();
  client->update (delta);
}

void
PingusGameSession::on_pause_press ()
{
  client->on_pause_press (); 
}

void
PingusGameSession::on_fast_forward_press ()
{
  client->on_fast_forward_press (); 
}

void
PingusGameSession::on_armageddon_press () 
{
  client->on_armageddon_press (); 
}

void
PingusGameSession::on_escape_press () 
{ 
  client->on_escape_press (); 
}

/* EOF */
