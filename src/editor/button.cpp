/*  $Id$
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#include "fonts.hpp"
#include "gui_style.hpp"
#include "button.hpp"

namespace Editor {

Button::Button(const Rect& rect, const std::string& text_)
  : RectComponent(rect), 
    text(text_),
    mouse_over(false),
    mouse_down(false)
{
}

void
Button::draw (DrawingContext& gc)
{
 if (mouse_down && mouse_over)
   GUIStyle::draw_lowered_box(gc, rect, Color(237, 233, 227), 2);
 else if (mouse_over)
   GUIStyle::draw_raised_box(gc, rect, Color(255, 255, 255), 2);
 else
   GUIStyle::draw_raised_box(gc, rect, Color(237, 233, 227), 2);  
 
 gc.print_center(Fonts::courier_small, 
                 rect.left + rect.get_width()/2, rect.top + rect.get_height()/2 - 6,
                 text);
}

void
Button::update (float delta)
{  
}

void
Button::on_pointer_enter () 
{
  mouse_over = true;
}

void
Button::on_pointer_leave () 
{
  mouse_over = false;
}
  
void
Button::on_primary_button_press (int x, int y) 
{
  mouse_down = true;
}

void
Button::on_primary_button_release (int x, int y) 
{ 
  mouse_down = false;
  if (mouse_over)
    on_click();    
}

} // namespace Editor

/* EOF */
