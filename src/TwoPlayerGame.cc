//  $Id: TwoPlayerGame.cc,v 1.3 2001/04/14 14:37:04 grumbel Exp $
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

#include "DeltaManager.hh"
#include "boost/smart_ptr.hpp"
#include "boost/dummy_ptr.hpp"
#include "Server.hh"
#include "TrueServer.hh"
#include "MultiplayerClientChild.hh"
#include "MultiplayerClient.hh"
#include "TwoPlayerGame.hh"
#include "XMLPLF.hh"
#include "Controller.hh"
#include "MouseController.hh"
#include "GamepadController.hh"

using boost::dummy_ptr;
using boost::shared_ptr;

TwoPlayerGame::TwoPlayerGame ()
{
}

TwoPlayerGame::~TwoPlayerGame ()
{
}

void 
TwoPlayerGame::start ()
{
  std::cout << "Starting Multiplayer Game" << std::endl;
  try {
  shared_ptr<PLF>               plf (new XMLPLF ("../data/levels/multi1-grumbel.xml"));
  shared_ptr<Server>            server (new TrueServer (plf));
  shared_ptr<Controller>        controller1 (new GamepadController (CL_Input::joysticks[0], 0));
  shared_ptr<Controller>        controller2 (new MouseController (1));
  shared_ptr<MultiplayerClient> 
    client (new MultiplayerClient 
	    (server.get (),
	     shared_ptr<MultiplayerClientChild>(new MultiplayerClientChild (controller1,
									    server.get (),
									    CL_Rect (0,0, 
										     CL_Display::get_width ()/2-2,
										     CL_Display::get_height ()))),
	     shared_ptr<MultiplayerClientChild>(new MultiplayerClientChild (controller2,
									    server.get (),
									    CL_Rect (CL_Display::get_width ()/2, 0,
										     CL_Display::get_width (), 
										     CL_Display::get_height ())))));

  DeltaManager delta_manager;
  while (!server->is_finished ())
    {
      float delta = delta_manager.getset ();
      server->update (delta);
      client->update (delta);
      client->draw ();
      Display::flip_display ();
      CL_System::keep_alive ();
      CL_System::sleep (40);
    }
  } catch (...) {
    std::cout << "TruePlayerGame: Something went wrong" << std::endl;
  }
}

/* EOF */
