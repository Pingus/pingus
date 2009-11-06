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

#ifndef HEADER_PINGUS_ENGINE_INPUT_OLD_POINTERS_MOUSE_POINTER_HPP
#define HEADER_PINGUS_ENGINE_INPUT_OLD_POINTERS_MOUSE_POINTER_HPP

#include "../pointer.hpp"
#include "../controller.hpp"

namespace Input {
namespace Pointers {

/**
   @brief maps the standard mouse into a pointer

   XML definition: <mouse-pointer/>
*/
class MousePointer : public Pointer {

private:
  float x_pos;
  float y_pos;

public:
  MousePointer();

  virtual const float& get_x_pos() const;
  virtual const float& get_y_pos() const;

  virtual void set_pos(float new_x, float new_y);

  virtual void update(float);

private:
  static void move_signal(const SDL_Event&, void*);
      
  MousePointer(const MousePointer&);
  MousePointer& operator= (const MousePointer&);
};

} // namespace Pointers
} // namespace Input

#endif

/* EOF */
