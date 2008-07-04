//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "../resource.hpp"
#include "../math/vector3f.hpp"
#include "../display/drawing_context.hpp"
#include "surface_button.hpp"


namespace GUI {

SurfaceButton::SurfaceButton (int arg_x_pos, int arg_y_pos,
			      const ResDescriptor& arg_button_surface,
			      const ResDescriptor& arg_button_pressed_surface,
			      const ResDescriptor& arg_button_mouse_over_surface)
  : x_pos (arg_x_pos), y_pos (arg_y_pos), pressed (false), mouse_over (false)
{
  button_surface            = Resource::load_sprite(arg_button_surface);
  button_pressed_surface    = Resource::load_sprite(arg_button_pressed_surface);
  button_mouse_over_surface = Resource::load_sprite(arg_button_mouse_over_surface);
}

SurfaceButton::~SurfaceButton ()
{
}

void
SurfaceButton::draw (DrawingContext& gc)
{
  if (pressed && mouse_over)
    gc.draw(button_pressed_surface, Vector2i(x_pos, y_pos));
  else if (!pressed && mouse_over)
    gc.draw(button_mouse_over_surface, Vector2i(x_pos, y_pos));
  else
    gc.draw(button_surface, Vector2i(x_pos, y_pos));
}

bool
SurfaceButton::is_at (int x, int y)
{
  /*std::cout << "Is AT: " << (x > x_pos && x < x_pos + int(button_surface.get_width ())
			     && y > y_pos && y < y_pos + int(button_surface.get_height ()))
	    << std::endl;*/

  return x > x_pos && x < x_pos + int(button_surface.get_width ())
      && y > y_pos && y < y_pos + int(button_surface.get_height ());
}

void
SurfaceButton::on_primary_button_press (int x, int y)
{
  pressed = true;

  UNUSED_ARG(x);
  UNUSED_ARG(y);
}

void
SurfaceButton::on_primary_button_release (int x, int y)
{
  pressed = false;

  UNUSED_ARG(x);
  UNUSED_ARG(y);
}

void
SurfaceButton::on_pointer_enter ()
{
  mouse_over = true;
}

void
SurfaceButton::on_pointer_leave ()
{
  mouse_over = false;
}

void
SurfaceButton::on_primary_button_click (int x, int y)
{
  on_click();
  UNUSED_ARG(x);
  UNUSED_ARG(y);
}

} // namespace GUI

/* EOF */
