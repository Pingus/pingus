//  $Id: button_panel.hxx,v 1.4 2002/06/28 15:12:22 torangan Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_BUTTON_PANEL_HXX
#define HEADER_PINGUS_BUTTON_PANEL_HXX

#include <vector>

#include "gui_obj.hxx"
#include "action_button.hxx"

class CL_Key;
class Client;
class PLF;
class Controller;

class ButtonPanel : public GuiObj
{
private:
  friend class ClientEvent;

  std::vector<ActionButton*> a_buttons;
  typedef std::vector<ActionButton*>::iterator AButtonIter;
  AButtonIter  pressed_button;
  ArmageddonButton* armageddon;
  ForwardButton*    forward;
  PauseButton*      pause;

  Server* server;
  Client* client;

  int  armageddon_pressed;
  AnimCounter arma_counter;

  bool left_pressed;
  unsigned int  last_press;
  static CL_Surface button_cap;

  Controller* controller;

  int x_pos, y_pos;
public:
  ButtonPanel(PLF* plf, Controller*, int arg_x_pos, int arg_y_pos);
  ~ButtonPanel();

  void on_button_press(const CL_Key& key);
  void on_button_release(const CL_Key& key);

  ActionName get_action_name();
  void   update(float delta);
  void   draw();
  void   set_server(Server*);
  void   set_client(Client*);
  void   set_button(int);

  /// Select the next action
  void next_action ();

  /// Select the previous action
  void previous_action ();
};

#endif

/* EOF */
