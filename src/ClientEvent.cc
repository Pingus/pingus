//  $Id: ClientEvent.cc,v 1.3 2000/02/11 16:58:25 grumbel Exp $
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

#include "OptionMenu.hh"
#include "ClientEvent.hh"

ClientEvent::ClientEvent()
{
  playfield = 0;
  client = 0;
}

ClientEvent::~ClientEvent()
{
}
  
void 
ClientEvent::register_event_handler()
{
  std::cout << "ClientEvent: register_event_handler()" << std::endl;
  CL_Input::chain_button_press.push_back(this);
  CL_Input::chain_button_release.push_back(this);
  enabled = true;
}

void
ClientEvent::unregister_event_handler()
{
  std::cout << "ClientEvent: unregister_event_handler()" << std::endl;
  CL_Input::chain_button_release.remove(this);
  CL_Input::chain_button_press.remove(this);
  enabled = false;
}

void
ClientEvent::disable_event_handler()
{
  enabled = false;
}

void
ClientEvent::enable_event_handler()
{
  enabled = true;
}

bool
ClientEvent::on_button_press(CL_InputDevice *device, const CL_Key &key)
{
  if (!enabled)
    return false;
  
  if (device == CL_Input::keyboards[0])
    {
      return on_keyboard_button_press(key);
    }
  else if (device == CL_Input::pointers[0])
    {
      return on_mouse_button_press(key);
    }
  else
    {
      std::cout << "Unknown device pressed: device=" << device << "; key.id=" << key.id << std::endl;
      return false;
    }
}

bool
ClientEvent::on_button_release(CL_InputDevice *device, const CL_Key &key)
{
  if (!enabled)
    return false;

  if (device == CL_Input::keyboards[0])
    {
      return on_keyboard_button_release(key);
    }
  else if (device == CL_Input::pointers[0])
    {
      return on_mouse_button_release(key);
    }
  else
    {
      std::cout << "Unknown device released: device=" << device << "; key.id=" << key.id << std::endl;
      return false;
    }  
  return false;
}

bool
ClientEvent::on_keyboard_button_press(const CL_Key& key)
{
  if (CL_Keyboard::get_keycode(CL_KEY_LSHIFT)
      || CL_Keyboard::get_keycode(CL_KEY_RSHIFT))
    {
      playfield->scroll_speed = 50;
    }
  else
    {
      playfield->scroll_speed = 15;
    }
  return false;
}

bool
ClientEvent::on_keyboard_button_release(const CL_Key& key)
{
  switch (key.id)
    {
    case CL_KEY_O:
      enabled = false;
      option_menu.display();
      enabled = true;
      break;

      // Playfield scrolling	
    case CL_KEY_LEFT:
      playfield->view[playfield->current_view].set_x_offset(playfield->view[playfield->current_view].get_x_offset() + playfield->scroll_speed);
      break;
	  
    case CL_KEY_RIGHT:
      playfield->view[playfield->current_view].set_x_offset(playfield->view[playfield->current_view].get_x_offset() - playfield->scroll_speed);
      break;
	  
    case CL_KEY_UP:
      playfield->view[playfield->current_view].set_y_offset(playfield->view[playfield->current_view].get_y_offset() + playfield->scroll_speed);
      break;
      
    case CL_KEY_DOWN:
      playfield->view[playfield->current_view].set_y_offset(playfield->view[playfield->current_view].get_y_offset() - playfield->scroll_speed);
      break;

      // Playfield zooming
    case CL_KEY_PAGEDOWN:
      playfield->view[playfield->current_view].set_zoom(playfield->view[playfield->current_view].get_zoom() / 1.05);
      break;
      
    case CL_KEY_PAGEUP:
      playfield->view[playfield->current_view].set_zoom(playfield->view[playfield->current_view].get_zoom() / 0.95);
      break;
      
    case CL_KEY_END:
      playfield->view[playfield->current_view].set_zoom(1.0);
      break;

      // Misc
    case CL_KEY_P:
      client->pause = !client->pause;
      client->server->set_pause(client->pause);
      break;

    case CL_KEY_A:
      client->server->send_event("armageddon");
      break;
	  
    case CL_KEY_R:
      client->do_restart();
      break;
	  
    case CL_KEY_SPACE:
      client->set_fast_forward(!client->get_fast_forward());
      break;

    case CL_KEY_ESCAPE:
      client->server->set_finished();
      break;

    case CL_KEY_F1:
      client->button_panel->set_button(0);
      break;
    case CL_KEY_F2:
      client->button_panel->set_button(1);
      break;
    case CL_KEY_F3:
      client->button_panel->set_button(2);
      break;      
    case CL_KEY_F4:
      client->button_panel->set_button(3);
      break;      
    case CL_KEY_F5:
      client->button_panel->set_button(4);
      break;      
    case CL_KEY_F6:
      client->button_panel->set_button(5);
      break;      
    case CL_KEY_F7:
      client->button_panel->set_button(6);
      break;      
    case CL_KEY_F8:
      client->button_panel->set_button(7);
      break;      
    default:
      std::cout << "ClientEvent: Got unknown button: ID=" << key.id << " ASCII=" << char(key.ascii) << std::endl;
    }
  return true;
}

bool
ClientEvent::on_mouse_button_press(const CL_Key& key)
{
  client->button_panel->on_button_press(key);

  switch(key.id)
    {
    case 0:
      break;
    case 1:
      break;
    case 2:
      playfield->enable_scroll_mode();
      break;
    default:
      std::cout << "ClientEvent: Unknown mouse button released: " << key.id << std::endl;
    }
  return false;
}

bool
ClientEvent::on_mouse_button_release(const CL_Key& key)
{
  client->button_panel->on_button_release(key);

  switch(key.id)
    {
    case 0:
      break;
    case 1:
      break;
    case 2:
      playfield->disable_scroll_mode();
      break;
    default:
      std::cout << "ClientEvent: Unknown mouse button released: " << key.id << std::endl;
    }
  return false;
}

/* EOF */
