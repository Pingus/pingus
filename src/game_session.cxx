//  $Id: game_session.cxx,v 1.13 2002/10/01 21:48:32 grumbel Exp $
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

#include <ClanLib/Core/System/system.h>
#include <iostream>
#include "client.hxx"
#include "true_server.hxx"
#include "game_session.hxx"
#include "game_session_result.hxx"
#include "timer.hxx"
#include "plf.hxx"
#include "globals.hxx"

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

  last_redraw = CL_System::get_time();
  last_update = CL_System::get_time();
  
  frames = 0;
  frame_skips = 0;
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

bool
PingusGameSession::draw (GraphicContext& gc)
{
  client->draw (gc);
  last_redraw = CL_System::get_time();
  return true;
}

void
PingusGameSession::update (const GameDelta& delta)
{
  int time_passed = (CL_System::get_time() - last_update);
  int update_time = game_speed;

  if (time_passed > update_time)
    {
      for (int i = 0;  i < time_passed; i += update_time)
	{
	  // This updates the world and all objects
	  server->update ();
	}
      // This updates something else... what?! Well, userinterface and
      // things like that...
      last_update = CL_System::get_time();
    }
  else
    {
      if (!max_cpu_usage)
	{
	  CL_System::sleep(update_time - time_passed);
	}
    }
  
  // Client is independend of the update limit, well, not completly...
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
