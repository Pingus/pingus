//  $Id: MultiplayerGame.cc,v 1.10 2002/06/10 11:00:26 torangan Exp $
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

#include <ClanLib/Display/Input/input.h>
#include <ClanLib/Display/Display/display.h>
#include "PathManager.hh"
#include "TrueServer.hh"
#include "MultiplayerClient.hh"
#include "XMLPLF.hh"
#include "MouseController.hh"
#include "GamepadController.hh"
#include "KeyboardController.hh"
#include "MultiplayerGame.hh"

using boost::shared_ptr;

MultiplayerGame::MultiplayerGame ()
{
}

MultiplayerGame::~MultiplayerGame ()
{
}

void 
MultiplayerGame::start ()
{
  std::cout << "Starting Multiplayer Game" << std::endl;
  try {
    PLF* plf = new XMLPLF (path_manager.complete("levels/multi2-grumbel.xml"));
    shared_ptr<Server>     server (new TrueServer (plf));
    shared_ptr<Controller> controller1;
    shared_ptr<Controller> controller2;
    shared_ptr<Controller> controller3;
    shared_ptr<Controller> controller4;

    if (CL_Input::joysticks.size () > 0)
      controller1 = shared_ptr<Controller>(new GamepadController (CL_Input::joysticks[0], 0));
    else 
      controller1 = shared_ptr<Controller>(new KeyboardController (0));

    if (CL_Input::joysticks.size () > 1)
      controller2 = shared_ptr<Controller>(new GamepadController (CL_Input::joysticks[1], 0));
    else
      controller2 = shared_ptr<Controller>(new MouseController (1));

    if (CL_Input::joysticks.size () > 2)
      controller3 = shared_ptr<Controller>(new GamepadController (CL_Input::joysticks[2], 0));
    else
      controller3 = shared_ptr<Controller>(new KeyboardController (2));

    controller4 = shared_ptr<Controller>(new MouseController (3));

    shared_ptr<MultiplayerClient> client;
    int player = 2;
    if (player == 2)
      {
	client = shared_ptr<MultiplayerClient> 
	  (new MultiplayerClient 
	   (server.get (),
	    shared_ptr<GuiObj>(new MultiplayerClientChild (controller1.get(),
							   server.get (),
							   CL_Rect (0,0, 
								    CL_Display::get_width ()/2-2,
								    CL_Display::get_height ()))),
	    shared_ptr<GuiObj>(new MultiplayerClientChild (controller2.get(),
							   server.get (),
							   CL_Rect (CL_Display::get_width ()/2, 0,
								    CL_Display::get_width (), 
								    CL_Display::get_height ())))));
      }
    else
      {
	client = shared_ptr<MultiplayerClient> 
	  (new MultiplayerClient 
	   (server.get (),
	    shared_ptr<GuiObj>(new MultiplayerClientChild (controller1.get(),
							   server.get (),
							   CL_Rect (0,
								    0, 
								    CL_Display::get_width ()/2 - 4,
								    CL_Display::get_height ()/2 - 4))),
	    shared_ptr<GuiObj>(new MultiplayerClientChild (controller2.get(),
							   server.get (),
							   CL_Rect (CL_Display::get_width ()/2 + 4,
								    0,
								    CL_Display::get_width (), 
								    CL_Display::get_height ()/2 - 4))),
	    shared_ptr<GuiObj>(new MultiplayerClientChild (controller3.get(),
							   server.get (),
							   CL_Rect (0,
								    CL_Display::get_height ()/2 + 4,
								    CL_Display::get_width ()/2 - 4, 
								    CL_Display::get_height ()))),
	    shared_ptr<GuiObj>(new MultiplayerClientChild (controller4.get(),
							   server.get (),
							   CL_Rect (CL_Display::get_width ()/2 + 4,
								    CL_Display::get_height ()/2 + 4,
								    CL_Display::get_width (), 
								    CL_Display::get_height ())))
	    ));
      }

    DeltaManager delta_manager;
    while (!server->is_finished ())
      {
	float delta = delta_manager.getset ();
	server->update (delta);
	client->update (delta);
	client->draw ();
	Display::flip_display ();
	// FIXME: Remove me to gain some fps
	CL_Display::clear_display (1.0, 1.0, 1.0);
	CL_System::keep_alive ();
	CL_System::sleep (0);
      }

    delete plf;
  } catch (...) {
    std::cout << "MultiplayerGame: Something went wrong and we have probally a memory leak." << std::endl;
  }
}

/* EOF */
