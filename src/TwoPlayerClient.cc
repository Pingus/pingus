//  $Id: TwoPlayerClient.cc,v 1.1 2001/04/13 22:17:46 grumbel Exp $
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

#include "boost/smart_ptr.hpp"
#include "PlayfieldView.hh"
#include "Display.hh"
#include "TwoPlayerClient.hh"

using boost::shared_ptr;
using boost::dummy_ptr;

TwoPlayerClient::TwoPlayerClient (shared_ptr<Controller> arg_controller1,
				  shared_ptr<Controller> arg_controller2,
				  dummy_ptr<Server> s)
  : controller1 (arg_controller1),
    controller2 (arg_controller2),
    server (s),
    cursor1 (new Cursor ("cursors/cross", "core", controller1)),
    cursor2 (new Cursor ("cursors/cross", "core", controller2)),
    button_panel1 (new ButtonPanel (s->get_plf (), controller1, 0, 2)),
    button_panel2 (new ButtonPanel (s->get_plf (), controller2, CL_Display::get_width () - 58, 2))
{
  // FIXME: The buttonpanel sucks...
  button_panel1->set_server (server.get ());
  button_panel2->set_server (server.get ());

  std::cout << "TwoPlayerClient: Creating: " << server.get() << std::endl;
  gui_objs.push_back (shared_ptr<GuiObj>(new PlayfieldView (server->get_world (),
							    0, 0,
							    CL_Display::get_width ()/2, 
							    CL_Display::get_height ())));
  
  gui_objs.push_back (shared_ptr<GuiObj>(new PlayfieldView (server->get_world (),
							    CL_Display::get_width ()/2 + 1, 
							    0,
							    CL_Display::get_width (),
							    CL_Display::get_height ())));
  gui_objs.push_back (button_panel1);
  gui_objs.push_back (button_panel2);

  controller1->set_range (0, 0,
			 CL_Display::get_width ()/2, 
			 CL_Display::get_height ());
  controller2->set_range (CL_Display::get_width ()/2 + 1, 0,
			 CL_Display::get_width (), CL_Display::get_height ());

  Display::add_flip_screen_hook (cursor1.get ());
  Display::add_flip_screen_hook (cursor2.get ());

  slot_left1_pressed = controller1->left->signal_pressed.connect (CL_CreateSlot (this, &TwoPlayerClient::on_left1_pressed));
  slot_left1_pressed = controller2->left->signal_pressed.connect (CL_CreateSlot (this, &TwoPlayerClient::on_left2_pressed));

  slot_right1_pressed = controller1->right->signal_pressed.connect (CL_CreateSlot (this, &TwoPlayerClient::on_right1_pressed));
  slot_right2_pressed = controller2->right->signal_pressed.connect (CL_CreateSlot (this, &TwoPlayerClient::on_right2_pressed));

  slot_right1_released = controller1->right->signal_released.connect (CL_CreateSlot (this, &TwoPlayerClient::on_right1_released));
  slot_right2_released = controller2->right->signal_released.connect (CL_CreateSlot (this, &TwoPlayerClient::on_right2_released));
}

TwoPlayerClient::~TwoPlayerClient ()
{
  Display::remove_flip_screen_hook (cursor1.get ());
  Display::remove_flip_screen_hook (cursor2.get ());
}

void 
TwoPlayerClient::draw ()
{
  CL_Display::clear_display (1.0, 0.0, 0.0);
  for (GuiObjIter i = gui_objs.begin (); i != gui_objs.end (); ++i)
    (*i)->draw_clipped ();
}

void 
TwoPlayerClient::update (float delta)
{
  for (GuiObjIter i = gui_objs.begin (); i != gui_objs.end (); ++i)
    (*i)->update (delta);
}

void TwoPlayerClient::on_left1_pressed (const CL_Vector& pos)
{
  std::cout << "Left pressed" << std::endl;
  CL_Key key;
  key.id = CL_MOUSE_LEFTBUTTON;
  key.x = pos.x;
  key.y = pos.y;
  button_panel1->on_button_press(key);
}

void TwoPlayerClient::on_left2_pressed (const CL_Vector& pos)
{
  std::cout << "Left2 pressed" << std::endl;
  CL_Key key;
  key.id = CL_MOUSE_LEFTBUTTON;
  key.x = pos.x;
  key.y = pos.y;
  button_panel2->on_button_press(key);
}

void TwoPlayerClient::on_right1_pressed (const CL_Vector& pos)
{
}

void TwoPlayerClient::on_right2_pressed (const CL_Vector& pos)
{
}

void TwoPlayerClient::on_right1_released (const CL_Vector& pos)
{
}

void TwoPlayerClient::on_right2_released (const CL_Vector& pos)
{
}

/* EOF */
