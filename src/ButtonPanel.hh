//  $Id: ButtonPanel.hh,v 1.6 2000/08/09 14:39:37 grumbel Exp $
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

#ifndef BUTTON_PANEL_HH
#define BUTTON_PANEL_HH

#include <vector>

#include "PLF.hh"
#include "Pingu.hh"
#include "GuiObj.hh"
#include "ActionButton.hh"
#include "World.hh"
#include "TrueServer.hh"
#include "Client.hh"

///
class Client;

class ButtonPanel : public GuiObj
{
private:
  ///
  friend class ClientEvent;
  ///
  std::vector<ActionButton*> a_buttons;
  ///
  typedef std::vector<ActionButton*>::iterator AButtonIter;
  ///
  ActionButton*     pressed_button;

  ///
  ArmageddonButton* armageddon;
  ///
  ForwardButton*    forward;
  ///
  PauseButton*      pause;

  ///
  Server* server;
  ///
  Client* client;
  ///
  int  armageddon_pressed;
  ///
  AnimCounter arma_counter;

  ///
  bool left_pressed;
  ///
  unsigned int  last_press;
  ///  static CL_ResourceManager* resource;
  static CL_Surface* button_cap;
  ///
  static CL_Surface* button_cap_pers;
  ///  static CL_Font*    font;
  World* world;
  
public:
  ///
  ButtonPanel(PLF* plf);
  ///
  ~ButtonPanel();

  ///
  void on_button_press(const CL_Key& key);
  ///
  void on_button_release(const CL_Key& key);

  ///
  std::string get_action_name();
  ///
  void   let_move();
  ///
  void   draw();
  ///
  void   set_server(Server*);
  ///
  void   set_client(Client*);
  ///
  void   set_button(int);
}///
;

#endif

/* EOF */
