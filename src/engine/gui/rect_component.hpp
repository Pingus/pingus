// Pingus - A free Lemmings clone
// Copyright (C) 2007 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_ENGINE_GUI_RECT_COMPONENT_HPP
#define HEADER_PINGUS_ENGINE_GUI_RECT_COMPONENT_HPP

#include "engine/gui/component.hpp"
#include "math/rect.hpp"

namespace GUI {

class RectComponent : public Component
{
protected:
  Rect rect;

public:
  RectComponent() :
    rect()
  {}

  RectComponent(const Rect& rect_) :
    rect(rect_)
  {}

  virtual bool is_at (int x, int y) { return rect.contains(Vector2i(x, y)); }
  virtual void update_layout() {}

  void set_rect(const Rect& rect_)
  {
    rect = rect_;
    update_layout();
  }

  Rect get_rect() const { return rect; }
};

} // namespace GUI

#endif

/* EOF */
