//  $Id: MultiplayerClientChild.cc,v 1.1 2001/04/14 11:41:21 grumbel Exp $
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
#include "Display.hh"
#include "MultiplayerClientChild.hh"

using boost::shared_ptr;
using boost::dummy_ptr;

MultiplayerClientChild::MultiplayerClientChild (shared_ptr<Controller> arg_controller,
						dummy_ptr<Server> s, const CL_Rect& arg_rect)
  : Controllable (arg_controller),
    controller (arg_controller),
    server (s),
    cursor (new Cursor ("cursors/cross", "core", controller)),
    button_panel (new ButtonPanel (s->get_plf (), controller, arg_rect.x1, arg_rect.y1)),
    playfield (new PlayfieldView (server->get_world (), arg_rect)),
    rect (arg_rect)
{
  // FIXME: The buttonpanel sucks...
  button_panel->set_server (server.get ());
  
  std::cout << "MultiplayerClientChild: Creating: " << server.get() << std::endl;
  gui_objs.push_back (playfield);
  gui_objs.push_back (button_panel);

  controller->set_range (rect);

  set_clip_rect(rect.x1, rect.y1, rect.x2, rect.y2);

  Display::add_flip_screen_hook (cursor.get ());
}

MultiplayerClientChild::~MultiplayerClientChild ()
{
  Display::remove_flip_screen_hook (cursor.get ());
}

void 
MultiplayerClientChild::draw ()
{
  //std::cout << "Rect: " << rect.x1 << " " << rect.y1 << " " << rect.x2 << " " << rect.y2 << std::endl;
  for (GuiObjIter i = gui_objs.begin (); i != gui_objs.end (); ++i)
    (*i)->draw_clipped ();
}

void 
MultiplayerClientChild::update (float delta)
{
  for (GuiObjIter i = gui_objs.begin (); i != gui_objs.end (); ++i)
    (*i)->update (delta);
}

void MultiplayerClientChild::on_left_press (const CL_Vector& pos)
{
  std::cout << "Left pressed" << std::endl;
  CL_Key key;
  key.id = CL_MOUSE_LEFTBUTTON;
  key.x = pos.x;
  key.y = pos.y;
  button_panel->on_button_press(key);
}

void MultiplayerClientChild::on_right_press (const CL_Vector& pos)
{
}

void MultiplayerClientChild::on_right_release (const CL_Vector& pos)
{
}

void 
MultiplayerClientChild::on_scroll_left_press (const CL_Vector& pos)
{
  std::cout << "Scroll left" << std::endl;
  //playfield->scroll (CL_Vector (-1.0 ,0.0));
}

void 
MultiplayerClientChild::on_scroll_right_press (const CL_Vector& pos)
{
  std::cout << "Scroll right" << std::endl;
  //playfield->scroll (CL_Vector (1.0 ,0.0));
}

/* EOF */
