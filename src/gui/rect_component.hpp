//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_GUI_RECT_COMPONENT_HPP
#define HEADER_PINGUS_GUI_RECT_COMPONENT_HPP

#include "math/rect.hpp"
#include "component.hpp"

namespace GUI {

/** */
class RectComponent : public Component
{
protected:
  Rect rect;

public:
  RectComponent(const Rect& rect_)
    : rect(rect_)
  {}
  
  virtual bool is_at (int x, int y) { return rect.is_inside(Vector2i(x, y)); }
  virtual void update_layout() =0;
  
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
