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

#include "group_component.hpp"

namespace GUI {

GroupComponent::GroupComponent(const Rect& rect)
  : RectComponent(rect)
{
}

GroupComponent::~GroupComponent()
{
}
  	
void
GroupComponent::draw (DrawingContext& gc)
{
}

void
GroupComponent::update (float delta)
{
}

bool is_at (int x, int y);

void
GroupComponent::on_primary_button_press (int x, int y)
{
}

void
GroupComponent::on_primary_button_release (int x, int y)
{
}

void
GroupComponent::on_primary_button_double_click (int x, int y)
{
}
  
void
GroupComponent::on_secondary_button_press (int x, int y)
{
}

void
GroupComponent::on_secondary_button_release (int x, int y)
{
}

void
GroupComponent::on_primary_button_click (int x, int y)
{
}

void
GroupComponent::on_secondary_button_click (int x, int y)
{
}

void
GroupComponent::on_pointer_enter () 
{
}

void
GroupComponent::on_pointer_leave () 
{
}

void
GroupComponent::on_pointer_move (int x, int y)
{
}

void
GroupComponent::on_key_pressed(const unsigned short c)
{
}

void
GroupComponent::add(Component*)
{
  
}

} // namespace GUI

/* EOF */
