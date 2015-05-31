// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pingus/layer_manager.hpp"

LayerManager::LayerManager () :
  layers()
{
}

LayerManager::~LayerManager()
{
}

void
LayerManager::add_layer(Sprite sprite, float x_o, float y_o, float x_u, float y_u)
{
  layers.push_back(Layer(sprite, x_o, y_o, x_u, y_u));
}

void
LayerManager::draw (DrawingContext& gc)
{
  for (std::vector<Layer>::iterator i = layers.begin ();
       i != layers.end (); ++i)
    i->draw(gc);
}

void
LayerManager::update (float delta)
{
  for (std::vector<Layer>::iterator i = layers.begin ();
       i != layers.end (); ++i)
    i->update(delta);
}

/* EOF */
