//  $Id: LayerManager.hh,v 1.3 2001/03/18 17:45:04 grumbel Exp $
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

#ifndef LAYERMANAGER_HH
#define LAYERMANAGER_HH

#include <vector>
#include <ClanLib/core.h>
#include <ClanLib/display.h>

class LayerManager
{
private:
  class Layer
  {
  public:
    Layer () {
      x_pos = 0;
      y_pos = 0;
    }
    
    void draw () {
      sur.put_screen (x_pos + x_offset, y_pos + y_offset);
      sur.put_screen (x_pos + x_offset - 800, y_pos + y_offset);
    }

    void update () {
      x_pos = (x_pos + x_update) % 800;
      y_pos = (y_pos + y_update) % 600;
    }
    
    CL_Surface sur;
    
    int x_update;
    int y_update;

    int x_offset;
    int y_offset;

    int x_pos;
    int y_pos;
  };

  vector<Layer> layers;

public:
  LayerManager ();
  ~LayerManager();

  void add_layer (const CL_Surface& sur, int x_o, int y_o, int x_u, int y_u);
  void draw ();
  void update ();
};

#endif

/* EOF */
