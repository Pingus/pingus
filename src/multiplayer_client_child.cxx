//  $Id: multiplayer_client_child.cxx,v 1.2 2002/06/20 12:22:51 grumbel Exp $
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

#include <stdio.h>
#include <ClanLib/Display/Input/key.h>
#include "multiplayer_client_child.hxx"
#include "pingu.hxx"
#include "server.hxx"
#include "controller.hxx"


MultiplayerClientChild::MultiplayerClientChild (Controller* arg_controller,
						Server * s, const CL_Rect& arg_rect)
  : Controllable (arg_controller),
    server (s),
    cursor (new Cursor ("cursors/cross", "core", controller)),
    button_panel (new ButtonPanel (s->get_plf (), controller, arg_rect.x1, arg_rect.y1)),
    capture_rect ("cursors/capgood", "core"),
    playfield (new PlayfieldView (server->get_world (), arg_rect)),
    counterbar (new PingusCounterBar (server, PingusCounterBar::VERTICAL, 
				      CL_Rect (arg_rect.x2 - 20, arg_rect.y1, arg_rect.x2, arg_rect.y2))),
    rect (arg_rect)
{
  capture_rect.set_align_center ();

  // FIXME: The buttonpanel sucks...
  button_panel->set_server (server);
  
  //std::cout << "MultiplayerClientChild: Creating: " << server.get() << std::endl;
  gui_objs.push_back (playfield);
  gui_objs.push_back (button_panel);
  gui_objs.push_back (counterbar);

  controller->set_range (rect);

  set_clip_rect(rect.x1, rect.y1, rect.x2, rect.y2);

  Display::add_flip_screen_hook (cursor);
}

MultiplayerClientChild::~MultiplayerClientChild ()
{
  Display::remove_flip_screen_hook (cursor);
  delete cursor;
  delete button_panel;
  delete playfield;
  delete counterbar;
}

void 
MultiplayerClientChild::draw ()
{
  //std::cout << "Rect: " << rect.x1 << " " << rect.y1 << " " << rect.x2 << " " << rect.y2 << std::endl;
  for (GuiObjIter i = gui_objs.begin (); i != gui_objs.end (); ++i)
    (*i)->draw_clipped ();

  Pingu* pingu = playfield->get_pingu (controller->get_pos ());
  if (pingu && pingu->get_owner () == controller->get_owner ())
    capture_rect.put_screen(controller->get_pos ());
}

void 
MultiplayerClientChild::update (float delta)
{
  for (GuiObjIter i = gui_objs.begin (); i != gui_objs.end (); ++i)
    (*i)->update (delta);

  playfield->scroll (scroll_vec * delta);
}

void MultiplayerClientChild::on_left_press (const CL_Vector& pos)
{
  CL_Key key;
  key.id = CL_MOUSE_LEFTBUTTON;
  key.x = pos.x;
  key.y = pos.y;
  button_panel->on_button_press(key);

  Pingu* pingu = playfield->get_pingu (controller->get_pos ());
  if (pingu && pingu->get_owner () == controller->get_owner ())
    {
      char str[256];
      sprintf(str, "Pingu: %d:%s", pingu->get_id(), button_panel->get_action_name().c_str());
      server->send_event(str);
    }
}

void MultiplayerClientChild::on_right_press (const CL_Vector& /*pos*/)
{
}

void MultiplayerClientChild::on_right_release (const CL_Vector& /*pos*/)
{
}

void 
MultiplayerClientChild::on_scroll_left_press (const CL_Vector& /*pos*/)
{
  scroll_vec += CL_Vector (450.0f, 0.0f);
}

void 
MultiplayerClientChild::on_scroll_right_press (const CL_Vector& /*pos*/)
{
  scroll_vec += CL_Vector (-450.0f, 0.0f);
}

void 
MultiplayerClientChild::on_scroll_left_release (const CL_Vector& /*pos*/)
{
  scroll_vec -= CL_Vector (450.0f,0.0f);
}

void 
MultiplayerClientChild::on_scroll_right_release (const CL_Vector& /*pos*/)
{
  scroll_vec -= CL_Vector (-450.0f, 0.0f);
}

void 
MultiplayerClientChild::on_scroll_up_press (const CL_Vector& /*pos*/)
{
  scroll_vec += CL_Vector (0.0f, 450.0f);
}

void 
MultiplayerClientChild::on_scroll_up_release (const CL_Vector& /*pos*/)
{
  scroll_vec -= CL_Vector (0.0f, 450.0f);
}

void 
MultiplayerClientChild::on_scroll_down_press (const CL_Vector& /*pos*/)
{
  scroll_vec += CL_Vector (0.0f, -450.0f);
}

void 
MultiplayerClientChild::on_scroll_down_release (const CL_Vector& /*pos*/)
{
  scroll_vec -= CL_Vector (0.0f, -450.0f);
}

void 
MultiplayerClientChild::on_next_action_press (const CL_Vector& /*pos*/)
{
  button_panel->next_action ();
}

void 
MultiplayerClientChild::on_previous_action_press (const CL_Vector& /*pos*/)
{
  button_panel->previous_action ();
}

/* EOF */
