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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_FONT_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_FONT_HPP

#include <memory>

#include "math/rect.hpp"

namespace pingus {

class FontImpl;
class FontDescription;
class Framebuffer;

class Font
{
public:
  Font();
  Font(FontDescription const& desc);

  void render(int x, int y, std::string const& text, Framebuffer& fb);
  void render(Origin origin, int x, int y, std::string const& text, Framebuffer& fb);

  int  get_height() const;
  float get_width(uint32_t unicode) const;
  float get_width(std::string const& text) const;
  Size get_size(std::string const& text) const;
  Rect bounding_rect(int , int, std::string const& str) const;

private:
  std::shared_ptr<FontImpl> impl;
};

} // namespace pingus

#endif

/* EOF */
