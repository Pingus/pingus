//  $Id: ButtonPanel.hh,v 1.15 2002/06/07 19:10:33 grumbel Exp $
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
#include "boost/smart_ptr.hpp"

#include "GuiObj.hh"
#include "ActionButton.hh"

class Client;
class PLF;
class World;
class Controller;

class ButtonPanel : public GuiObj
{
private:
  friend class ClientEvent;

  std::vector<boost::shared_ptr<ActionButton> > a_buttons;
  typedef std::vector<boost::shared_ptr<ActionButton> >::iterator AButtonIter;
  AButtonIter  pressed_button;
  boost::shared_ptr<ArmageddonButton> armageddon;
  boost::shared_ptr<ForwardButton>    forward;
  boost::shared_ptr<PauseButton>      pause;

  Server* server;
  Client* client;

  int  armageddon_pressed;
  AnimCounter arma_counter;

  bool left_pressed;
  unsigned int  last_press;
  static CL_Surface button_cap;
  World* world;

  boost::shared_ptr<Controller> controller;

  int x_pos, y_pos;
public:
  ButtonPanel(PLF* plf,
	      boost::shared_ptr<Controller>,int arg_x_pos, int arg_y_pos);
  ~ButtonPanel();

  void on_button_press(const CL_Key& key);
  void on_button_release(const CL_Key& key);

  std::string get_action_name();
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
