// Pingus - A free Lemmings clone
// Copyright (C) 2005 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_SPRITE_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_SPRITE_HPP

#include <memory>
#include <string>

#include "math/origin.hpp"
#include "engine/display/resource_modifier.hpp"

class Color;
class Surface;
class Pathname;
class SpriteImpl;
class SpriteDescription;
class ResDescriptor;
class Framebuffer;

class Sprite
{
public:
  Sprite();
  Sprite(const std::string& name);
  Sprite(const ResDescriptor& desc);
  Sprite(const SpriteDescription& desc, ResourceModifier::Enum mod = ResourceModifier::ROT0);
  Sprite(const Surface& surface);
  ~Sprite();

  int get_width()  const;
  int get_height() const;

  void render(int x, int y, Framebuffer& target);
  void update(float delta = 0.033f);

  void set_hotspot(Origin origin, int x, int y);
  Vector2i get_offset() const;
  void set_frame(int i);
  int  get_frame_count() const;
  int  get_current_frame() const;
  bool is_finished() const;
  bool is_looping() const;
  void set_play_loop(bool loop = true);
  void restart();
  void finish();
  operator bool() const;

private:
  std::shared_ptr<SpriteImpl> impl;
};

#endif

/* EOF */
