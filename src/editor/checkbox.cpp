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
#include "checkbox.hpp"

namespace Editor {

Checkbox::Checkbox(const Rect& rect, const std::string& label_)
  : RectComponent(rect),
    checked(false),
    label(label_)
{
}

Checkbox::~Checkbox()
{
}

void
Checkbox::draw(DrawingContext& gc)
{
  if (checked)
    GUIStyle::draw_lowered_box(gc, rect);
  else
    GUIStyle::draw_raised_box(gc, rect);

  if (!label.empty())
    gc.print_center(Fonts::verdana11, 
                    rect.left + rect.get_width()/2, 
                    rect.top + rect.get_height()/2 - Fonts::verdana11.get_height()/2, 
                    label);
}

void
Checkbox::on_primary_button_press(int x, int y)
{
  checked = !checked;
  on_change(checked);
}

void
Checkbox::set_checked(bool t) 
{
  checked = t;
  //on_change(checked);
}

} // namespace Editor

/* EOF */
