//  $Id: ClientEvent.hh,v 1.5 2000/08/10 15:13:07 grumbel Exp $
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

#ifndef CLIENTEVENT_HH
#define CLIENTEVENT_HH

#include <ClanLib/core.h>
#include "Playfield.hh"
#include "Client.hh"

///
class Client;
class Playfield;

class ClientEvent //: public CL_Event_ButtonPress, public CL_Event_ButtonRelease
{
private:
  ///
  friend Client;
  ///
  Client* client;
  ///
  Playfield* playfield;

  ///
  thSlot on_button_press_slot;
  ///
  thSlot on_button_release_slot;

  ///
  bool enabled;
public:
  ///
  ClientEvent();
  ///
  virtual ~ClientEvent();

  ///
  virtual void on_button_press(CL_InputDevice *device, const CL_Key &key);
  ///
  virtual void on_button_release(CL_InputDevice *device, const CL_Key &key);

  ///
  void on_mouse_button_press(const CL_Key &key);
  ///
  void on_mouse_button_release(const CL_Key &key);
  
  ///
  void on_keyboard_button_press(const CL_Key &key);
  ///
  void on_keyboard_button_release(const CL_Key &key);

  ///
  void register_event_handler();
  ///
  void unregister_event_handler();

  ///
  void disable_event_handler();
  ///
  void enable_event_handler();
};

#endif

/* EOF */
