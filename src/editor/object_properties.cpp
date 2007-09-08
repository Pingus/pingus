//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2006 Ingo Ruhnke <grumbel@gmx.de>
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

#include "button.hpp"
#include "editor_screen.hpp"
#include "object_properties.hpp"

namespace Editor {

ObjectProperties::ObjectProperties(EditorScreen* editor_, const Rect& rect)
  : GUI::GroupComponent(rect),
    editor(editor_)
{
  add(new Button(Rect(10, 10, 100, 20), "Test"), true);
  add(new Button(Rect(10, 100, 100, 220), "Test 2"), true);
}

ObjectProperties::~ObjectProperties()
{
}

void
ObjectProperties::set_object(LevelObj* obj)
{
}

void
ObjectProperties::draw_background(DrawingContext& gc)
{
  gc.draw_fillrect(0,0, rect.get_width(), rect.get_height(), 
                   Color(255, 255, 0));
}

} // namespace Editor

/* EOF */
