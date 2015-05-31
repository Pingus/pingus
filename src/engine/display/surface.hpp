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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_SURFACE_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_SURFACE_HPP

#include <memory>

#include "engine/display/resource_modifier.hpp"
#include "math/color.hpp"
#include "math/size.hpp"
#include "util/pathname.hpp"

class Rect;
class Pathname;
class SurfaceImpl;

/** A software surface that is stored in system memory */
class Surface
{
public:
  Surface();

  Surface(const Pathname& name);

  /** Create an empty RGBA Surface */
  Surface(int width, int height);

  /** Create an empty Indexed Surface (8bit) */
  Surface(int width, int height, SDL_Palette* palette, int colorkey = -1);

  /** Create a Surface from a SDL_Surface */
  Surface(SDL_Surface* surface);

  ~Surface();

  uint8_t* get_data() const;
  void lock();
  void unlock();

  Size get_size()   const;
  int  get_width()  const;
  int  get_height() const;
  int  get_pitch()  const;

  void blit(Surface src, int x, int y);

  Color get_pixel(int x, int y) const;

  void fill(const Color& color);

  bool is_indexed() const;
  bool has_colorkey() const;

  Surface scale(int w, int h);
  Surface mod(ResourceModifier::Enum mod);
  Surface clone() const;
  Surface convert_to_rgba() const;
  Surface convert_to_rgb() const;
  Surface subsection(const Rect& rect) const;

  SDL_Surface* get_surface() const;

  operator bool() const;

  void print(std::ostream& out);

private:
  Surface(std::shared_ptr<SurfaceImpl> impl);

  std::shared_ptr<SurfaceImpl> impl;
};

#endif

/* EOF */
