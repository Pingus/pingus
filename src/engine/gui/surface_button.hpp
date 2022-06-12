// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_ENGINE_GUI_SURFACE_BUTTON_HPP
#define HEADER_PINGUS_ENGINE_GUI_SURFACE_BUTTON_HPP

#include "engine/display/sprite.hpp"
#include "engine/gui/component.hpp"

namespace pingus::gui {

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
                 std::string const& button_surface,
                 std::string const& button_pressed_surface,
                 std::string const& button_mouse_over_surface);
  ~SurfaceButton() override;

  virtual void set_pos(int x, int y);

  void draw (DrawingContext& gc) override;
  bool is_at (int x, int y) override;

  void on_primary_button_press (int x, int y) override;
  void on_primary_button_release (int x, int y) override;
  void on_primary_button_click (int x, int y) override;

  void on_pointer_enter() override;
  void on_pointer_leave() override;

  virtual void on_click() {}

private:
  SurfaceButton (SurfaceButton const&);
  SurfaceButton& operator= (SurfaceButton const&);
};

} // namespace pingus::gui

#endif

/* EOF */
