//  $Id: playfield.hxx,v 1.14 2003/02/19 10:37:31 grumbel Exp $
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

#ifndef HEADER_PINGUS_PLAYFIELD_HXX
#define HEADER_PINGUS_PLAYFIELD_HXX

#include <ClanLib/Core/Math/rect.h>
#include "view.hxx"
#include "client.hxx"
#include "gui/component.hxx"

class Pingu;
class World;
class Server;
class PinguInfo;
class ButtonPanel;
class Controller;

/** This class encapsulates all the different Views */
class Playfield : public GUI::Component
{
private:  
  friend class Client;

  CL_Surface buffer;
  Server* server;
  Client* client;
  ///Range x_offset2, y_offset2;
  ButtonPanel* buttons;
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

  std::vector<CL_Rect> clipping_rectangles;

  int mouse_x;
  int mouse_y;
public:
  Playfield (Client*);
  virtual ~Playfield();

  int get_x_offset();
  int get_y_offset();

  void scroll (int x, int y);

  void set_viewpoint(int, int);

  void draw(GraphicContext& gc);
  void update(float delta);
  void set_world(World*);
  Pingu* current_pingu_find(int x_pos, int y_pos);

  void on_primary_button_press (int x, int y);
  void on_secondary_button_press (int x, int y);
  void on_secondary_button_release (int x, int y);
  void on_pointer_move (int x, int y);

  void enable_scroll_mode();
  void do_scrolling();
  void disable_scroll_mode();

  void generate_clipping_rects(int, int, int, int);

  /// Members used to communicate between different screen objs
  void set_pingu_info(PinguInfo*);
  void set_buttons(ButtonPanel*);
  void set_server(Server*);

  bool is_at (int x, int y) { UNUSED_ARG(x); UNUSED_ARG(y); return true; }
  
private:
  Playfield (const Playfield&);
  Playfield& operator= (const Playfield&);
};

#endif

/* EOF */
