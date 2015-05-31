// Pingus - A free Lemmings clone
// Copyright (C) 2005-2008 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_SPRITE_IMPL_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_SPRITE_IMPL_HPP

#include "engine/display/framebuffer_surface.hpp"
#include "math/vector2i.hpp"

class SpriteDescription;
class Framebuffer;

class SpriteImpl
{
private:
  friend class Sprite;

  Pathname filename;
  FramebufferSurface framebuffer_surface;

  Vector2i offset;

  Vector2i frame_pos;
  Size     frame_size;
  int      frame_delay;

  Size     array;

  bool     loop;
  bool     loop_last_cycle;
  bool     finished;

  /** Current frame */
  int frame;
  int tick_count;

public:
  SpriteImpl();
  SpriteImpl(const SpriteDescription& desc, ResourceModifier::Enum mod = ResourceModifier::ROT0);
  SpriteImpl(const Surface& surface_);
  ~SpriteImpl();

  void update(float delta);

  void render(int x, int y, Framebuffer& fb);

  void restart();
  void finish();
};

#endif

/* EOF */
