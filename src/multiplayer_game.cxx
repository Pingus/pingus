//  $Id: multiplayer_game.cxx,v 1.5 2002/06/20 12:36:56 torangan Exp $
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
#include "path_manager.hxx"
#include "true_server.hxx"
#include "multiplayer_client.hxx"
#include "xml_plf.hxx"
#include "mouse_controller.hxx"
#include "gamepad_controller.hxx"
#include "keyboard_controller.hxx"
#include "multiplayer_game.hxx"
#include "multiplayer_client_child.hxx"

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
    std::auto_ptr<Server>     server (new TrueServer (plf));
    Controller* controller1;
    Controller* controller2;
    Controller* controller3;
    Controller* controller4;

    if (CL_Input::joysticks.size () > 0)
      controller1 = new GamepadController (CL_Input::joysticks[0], 0);
    else 
      controller1 = new KeyboardController (0);

    if (CL_Input::joysticks.size () > 1)
      controller2 = new GamepadController (CL_Input::joysticks[1], 0);
    else
      controller2 = new MouseController (1);

    if (CL_Input::joysticks.size () > 2)
      controller3 = new GamepadController (CL_Input::joysticks[2], 0);
    else
      controller3 = new KeyboardController (2);

    controller4 = new MouseController (3);

    MultiplayerClient* client;
    int player = 2;
    if (player == 2)
      {
	client = new MultiplayerClient (server.get (),
                                        new MultiplayerClientChild (controller1,
				                                    server.get (),
					                            CL_Rect (0,0, 
				                                             CL_Display::get_width ()/2-2,
                                                                             CL_Display::get_height ()
                                                                            )
                                                                   ),
                                        new MultiplayerClientChild (controller2,
					                            server.get (),
                                                                    CL_Rect (CL_Display::get_width ()/2, 0,
                                                                             CL_Display::get_width (), 
                                                                             CL_Display::get_height ()
                                                                            )
                                                                   )
                                       );
      }
    else
      {
	client = new MultiplayerClient (server.get (),
                                        new MultiplayerClientChild (controller1,
					                            server.get (),
					                            CL_Rect (0,
					                                     0,
						                             CL_Display::get_width  ()/2 - 4,
						                             CL_Display::get_height ()/2 - 4)
						                            ),
                                 	new MultiplayerClientChild (controller2,
					                            server.get (),
					                            CL_Rect (CL_Display::get_width ()/2 + 4,
						                             0,
						                             CL_Display::get_width (), 
						                             CL_Display::get_height ()/2 - 4
						                            )
						                   ),
                                        new MultiplayerClientChild (controller3,
					                            server.get (),
					                            CL_Rect (0,
						                             CL_Display::get_height ()/2 + 4,
						                             CL_Display::get_width ()/2 - 4,
						                             CL_Display::get_height ()
						                            )
						                   ),
	                                new MultiplayerClientChild (controller4,
					                            server.get (),
					                            CL_Rect (CL_Display::get_width ()/2 + 4,
					                                     CL_Display::get_height ()/2 + 4,
					                                     CL_Display::get_width (),
					                                     CL_Display::get_height ()
					                                    )
					                           )
	                               );
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
    delete controller1;
    delete controller2;
    delete controller3;
    delete controller4;
  } catch (...) {
    std::cout << "MultiplayerGame: Something went wrong and we have probally a memory leak." << std::endl;
  }
}

/* EOF */
