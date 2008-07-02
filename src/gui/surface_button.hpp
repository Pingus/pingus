//  $Id$
//
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

#ifndef HEADER_PINGUS_GUI_SURFACE_BUTTON_HPP
#define HEADER_PINGUS_GUI_SURFACE_BUTTON_HPP

#include "../sprite.hpp"
#include "component.hpp"

class ResDescriptor;

namespace GUI {

/** A simple surface button, which different surfaces for pressed,
    released and mouse over */
class SurfaceButton : public Component
{
protected:
  int x_pos;
  int y_pos;
  
  Sprite button_surface;
  Sprite button_pressed_surface;
  Sprite button_mouse_over_surface;

  bool pressed;
  bool mouse_over;

public:
  SurfaceButton (int x_pos, int y_pos,
                 const ResDescriptor& button_surface,
                 const ResDescriptor& button_pressed_surface,
                 const ResDescriptor& button_mouse_over_surface);
  virtual ~SurfaceButton ();

  virtual void draw (DrawingContext& gc);
  virtual bool is_at (int x, int y);

  void on_primary_button_press (int x, int y);
  void on_primary_button_release (int x, int y);
  void on_primary_button_click (int x, int y);

  virtual void on_pointer_enter ();
  virtual void on_pointer_leave ();

  virtual void on_click() {}
private:
  SurfaceButton (const SurfaceButton&);
  SurfaceButton& operator= (const SurfaceButton&);
};

} // namespace GUI

#endif

/* EOF */
