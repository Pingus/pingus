//  $Id: Playfield.hh,v 1.4 2000/04/10 21:19:56 grumbel Exp $
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

#ifndef PLAYFIELD_HH
#define PLAYFIELD_HH

#include <ClanLib/core.h>

#include "Range.hh"
#include "ButtonPanel.hh"
#include "GuiObj.hh"
#include "World.hh"
#include "View.hh"
#include "PinguInfo.hh"
#include "Server.hh"
#include "Client.hh"

class Client;
class ButtonPanel;

class Playfield : public GuiObj
{
private:  
  friend class ClientEvent;

  CL_Surface* buffer;
  Server* server;
  Client* client;
  //Range x_offset2, y_offset2;
  ButtonPanel* buttons;
  PinguHolder* pingus;
  PinguInfo* pingu_info;
  World* world;
  vector<View> view;
  
  //int  x_offset, y_offset;
  Pingu* current_pingu;
  bool mouse_scrolling;
  bool needs_clear_screen;
  int current_view;
  int scroll_speed;
  int scroll_center_x;
  int scroll_center_y;
public:
  Playfield();
  Playfield(PLF*, World*);
  ~Playfield();

  int get_x_offset();
  int get_y_offset();

  void set_viewpoint(int, int);

  void draw();
  void let_move();
  void process_input();
  void process_input_interactive();
  void process_input_demomode();
  void set_world(World*);
  Pingu* current_pingu_find(int x_pos, int y_pos);

  void enable_scroll_mode();
  void do_scrolling();
  void disable_scroll_mode();

  // Members used to communicate between different screen objs
  void set_pingu_info(PinguInfo*);
  void set_buttons(ButtonPanel* b);
  void set_server(Server*);
  void set_client(Client*);
};

#endif

/* EOF */
