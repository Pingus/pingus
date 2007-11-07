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

#include "fonts.hpp"
#include "display/drawing_context.hpp"
#include "slider_box.hpp"

SliderBox::SliderBox(const Rect& rect)
  : RectComponent(rect),
    value(10)
{
}

void
SliderBox::draw(DrawingContext& gc)
{
  gc.print_right(Fonts::chalk_normal, rect.left+rect.get_width()/2, rect.top, 
                 std::string(value, '|'));
}

void
SliderBox::on_primary_button_press(int x, int y)
{
  value += 1;
}

void
SliderBox::on_secondary_button_press(int x, int y)
{
  value -= 1;
  if (value < 0)
    value = 0;
}

/* EOF */
