//  $Id: playfield.hxx,v 1.1 2002/06/12 19:06:13 grumbel Exp $
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

#include "view.hxx"
#include "client.hxx"
#include "gui_obj.hxx"

class Pingu;
class World;
class Server;
class PinguInfo;
class PinguHolder;
class ButtonPanel;
class Controller;

///
class Playfield : public GuiObj
{
private:  
  friend class Client;

  CL_Surface buffer;
  Server* server;
  Client* client;
  ///Range x_offset2, y_offset2;
  ButtonPanel* buttons;
  PinguHolder* pingus;
  PinguInfo* pingu_info;
  World* world;
  std::vector<View*> view;
  
  ///int  x_offset, y_offset;
  Pingu* current_pingu;
  bool mouse_scrolling;
  bool needs_clear_screen;
  int current_view;
  int scroll_speed;
  int scroll_center_x;
  int scroll_center_y;

  // FIXME: can be replaced with CL_Rect
  struct Rect {
    Rect(int arg_x1, int arg_y1, int arg_x2, int arg_y2) {
      x1 = arg_x1;
      y1 = arg_y1;
      x2 = arg_x2;
      y2 = arg_y2;
    }
    int x1, y1, x2, y2;
  };

  std::vector<Rect> clipping_rectangles;

  Controller* controller;
public:
  Playfield(PLF* plf, World*,
	    Controller*);
  ~Playfield();

  int get_x_offset();
  int get_y_offset();

  void scroll (int x, int y);

  void set_viewpoint(int, int);

  void draw();
  void update(float delta);
  void updateX();
  void process_input();
  void process_input_interactive();
  void process_input_demomode();
  void set_world(World*);
  Pingu* current_pingu_find(int x_pos, int y_pos);

  bool on_button_press(const CL_Key &key);

  void enable_scroll_mode();
  void do_scrolling();
  void disable_scroll_mode();

  void generate_clipping_rects(int, int, int, int);

  /// Members used to communicate between different screen objs
  void set_pingu_info(PinguInfo*);
  void set_buttons(ButtonPanel*);
  void set_server(Server*);
  void set_client(Client*);
};

#endif

/* EOF */
