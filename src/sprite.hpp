/*  $Id$
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#ifndef HEADER_SPRITE_HPP
#define HEADER_SPRITE_HPP

#include <string>
#include <boost/shared_ptr.hpp>
#include "SDL.h"

class Color;
class PixelBuffer;
class SpriteImpl;
class SpriteDescription;

/** */
class Sprite
{
public:
  Sprite();
  Sprite(const SpriteDescription& desc);
  Sprite(const PixelBuffer& pixelbuffer);
  ~Sprite();

  int get_width();
  int get_height();

  void update(float delta = 0.033f);

  void draw(float x, float y, SDL_Surface* target);
  void set_frame(int i);
  int  get_frame_count() const;
  int  get_current_frame() const;
  bool is_finished() const;
  bool is_looping() const;
  void set_play_loop(bool loop = true);
  void restart();
  void finish();
  operator bool();
  SDL_Surface* get_surface() const;
  void set_surface(SDL_Surface* surface);

  /** Resizes the Sprite to the given size. This is a destructive
      operation. The original Surface will be lost. */
  void scale(int w, int h);

  /** Fills the Sprite with the given color. Transparent areas aren't
      touched.  This is a destructive operation. The original Surface
      will be lost. */
  void fill(const Color& color);

private:
  boost::shared_ptr<SpriteImpl> impl;
};

#endif

/* EOF */
