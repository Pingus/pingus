//  $Id: SmallMap.hh,v 1.11 2001/04/08 14:10:34 grumbel Exp $
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

#ifndef SMALLMAP_HH
#define SMALLMAP_HH

#include "Client.hh"
#include "GuiObj.hh"

///
class Client;

class SmallMap : public GuiObj
{
private:
  ///
  PLF* plf;
  ///
  ColMap* colmap;
  ///
  World* world;
  ///
  CL_Surface exit_sur;
  ///
  CL_Surface entrance_sur;
  ///
  CL_Surface sur;
  ///
  Client*     client;
  ///
  int x_pos;
  ///
  int y_pos;
  ///
  int width;
  ///
  int height;
  ///
  bool scroll_mode;
  ///
  int rwidth;
  ///
  int rheight;
public:
  ///
  SmallMap();
  ///
  ~SmallMap();
  
  ///
  bool on_button_press(const CL_Key& key);
  ///
  bool on_button_release(const CL_Key& key);
  ///
  void set_client(Client* c);

  ///
  bool mouse_over();
  ///
  void init();
  ///
  void draw();
  ///
  void draw_pingus();
  ///
  void update(float delta);
};

#endif

/* EOF */
