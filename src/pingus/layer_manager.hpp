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

#ifndef HEADER_PINGUS_PINGUS_LAYER_MANAGER_HPP
#define HEADER_PINGUS_PINGUS_LAYER_MANAGER_HPP

#include "engine/display/display.hpp"
#include "engine/display/drawing_context.hpp"
#include "engine/display/sprite.hpp"
#include "pingus/globals.hpp"

class LayerManager
{
private:
  class Layer
  {
  private:
    Sprite sur;

    float x_pos;
    float y_pos;

    float x_update;
    float y_update;

    float x_offset;
    float y_offset;

  public:
    Layer () :
      sur(),
      x_pos(0), y_pos(0),
      x_update(0), y_update(0),
      x_offset(0), y_offset(0)
    {}

    Layer (const Sprite& arg_sur, float x_o, float y_o, float x_u, float y_u) :
      sur(arg_sur),
      x_pos(0), y_pos(0),
      x_update(x_u), y_update(y_u),
      x_offset(x_o), y_offset(y_o)
    {}

    void draw (DrawingContext& gc)
    {
      gc.draw(sur, Vector3f(x_pos + x_offset, y_pos + y_offset));
      gc.draw(sur, Vector3f(x_pos + x_offset - static_cast<float>(gc.get_width()), y_pos + y_offset));
    }

    void update (float delta)
    {
      if (!globals::static_graphics)
      {
        x_pos = Math::mod((x_pos + x_update * delta),static_cast<float>(Display::get_width()));
        y_pos = Math::mod((y_pos + y_update * delta),static_cast<float>(Display::get_height()));
      }
    }
  };

  std::vector<Layer> layers;

public:
  LayerManager ();
  ~LayerManager();

  void add_layer (Sprite sur, float x_o, float y_o, float x_u, float y_u);
  void draw (DrawingContext& gc);
  void update (float delta);

private:
  LayerManager (const LayerManager&);
  LayerManager& operator= (const LayerManager&);
};

#endif

/* EOF */
