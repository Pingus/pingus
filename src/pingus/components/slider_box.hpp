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

#ifndef HEADER_PINGUS_PINGUS_COMPONENTS_SLIDER_BOX_HPP
#define HEADER_PINGUS_PINGUS_COMPONENTS_SLIDER_BOX_HPP

#include <boost/signals2.hpp>

#include "engine/gui/rect_component.hpp"

class SliderBox : public GUI::RectComponent
{
private:
  int m_steps;
  int value;
  bool drag_drop;

public:
  SliderBox(const Rect& rect, int steps);

  void draw(DrawingContext& gc);
  void on_primary_button_press(int x, int y);
  void on_primary_button_release(int x, int y);
  void on_pointer_move(int x, int y);
  void update_layout() {}

  void set_value(int v);

  boost::signals2::signal<void (int)> on_change;

private:
  SliderBox (const SliderBox&);
  SliderBox& operator= (const SliderBox&);
};

#endif

/* EOF */
