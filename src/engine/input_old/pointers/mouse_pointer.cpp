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

#include "mouse_pointer.hpp"

namespace Input {
namespace Pointers {

MousePointer::MousePointer() 
  : x_pos(0),
    y_pos(0)
{
  Controller::add_mouse_callback(MousePointer::move_signal, this);
}

const float&
MousePointer::get_x_pos() const
{
  return x_pos;
}

const float&
MousePointer::get_y_pos() const
{
  return y_pos;
}

void
MousePointer::set_pos(float new_x, float new_y)
{
  SDL_WarpMouse(static_cast<int>(new_x), static_cast<int>(new_y));
}

void
MousePointer::update(float)
{
}

void
MousePointer::move_signal(const SDL_Event& event, void* userdata)
{
  if (event.type != SDL_MOUSEMOTION)
    return;

  MousePointer* mp = (MousePointer*)userdata;
  mp->x_pos = (float)event.motion.x;
  mp->y_pos = (float)event.motion.y;
}

} // namespace Pointers
} // namespace Input

/* EOF */
