//  $Id: slider_box.hpp 3503 2007-11-09 08:38:26Z grumbel $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_SLIDER_BOX_HPP
#define HEADER_SLIDER_BOX_HPP

#include <string>
#include <boost/signal.hpp>
#include "gui/rect_component.hpp"

/** */
class SliderBox : public GUI::RectComponent
{
private:
  int value;
  bool drag_drop;

public:
  SliderBox(const Rect& rect);

  void draw(DrawingContext& gc);
  void on_primary_button_press(int x, int y);
  void on_primary_button_release(int x, int y);
  void on_pointer_move(int x, int y);
  void update_layout() {}

  boost::signal<void (int)> on_change;

private:
  SliderBox (const SliderBox&);
  SliderBox& operator= (const SliderBox&);
};

#endif

/* EOF */
