//  $Id: smallmap.hxx,v 1.5 2002/07/29 10:44:12 grumbel Exp $
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

#ifndef HEADER_PINGUS_SMALLMAP_HXX
#define HEADER_PINGUS_SMALLMAP_HXX

#include <ClanLib/Display/Display/surface.h>
#include "gui_obj.hxx"


class CL_Key;
class Client;

class SmallMap : public GuiObj
{
private:
  Client* client;

  CL_Surface exit_sur;
  CL_Surface entrance_sur;
  CL_Surface sur;

  int x_pos;
  int y_pos;
  int width;
  int height;
  bool scroll_mode;
  int rwidth;
  int rheight;
  bool has_focus;
public:
  SmallMap();
  ~SmallMap();
  
  bool on_button_press(const CL_Key& key);
  bool on_button_release(const CL_Key& key);

  void set_client(Client* c);
  bool mouse_over();
  void init();
  void draw();
  void draw_pingus();
  void update(float delta);

  bool mouse_over(int x, int y);

  // Events
  void on_mouse_enter ();
  void on_mouse_leave ();
};

#endif

/* EOF */
