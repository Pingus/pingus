//  $Id: layer_manager.cxx,v 1.4 2003/03/24 11:18:53 grumbel Exp $
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

#include <iostream>
#include "layer_manager.hxx"

LayerManager::LayerManager ()
{
}

LayerManager::~LayerManager()
{
}

void
LayerManager::add_layer (const CL_Surface& sur, float x_o, float y_o, float x_u, float y_u)
{
  Layer layer;
  layer.sur = sur;
  layer.x_offset = x_o;
  layer.y_offset = y_o;
  layer.x_update = x_u;
  layer.y_update = y_u;

  layers.push_back (layer);
}

void
LayerManager::draw (GraphicContext& gc)
{
  for (std::vector<Layer>::iterator i = layers.begin (); 
       i != layers.end (); ++i)
    i->draw (gc);
}

void 
LayerManager::update (float delta)
{
  for (std::vector<Layer>::iterator i = layers.begin (); 
       i != layers.end (); ++i)
    i->update (delta);
}

/* EOF */
