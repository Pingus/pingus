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
#include "level_objs.hpp"
#include "fonts.hpp"
#include "object_properties.hpp"

namespace Editor {

ObjectProperties::ObjectProperties(EditorScreen* editor_, const Rect& rect)
  : GUI::GroupComponent(rect),
    editor(editor_)
{
  add(new Button(Rect(10, 50, 100, 70), "Test 2"), true);
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

  std::string obj_type;
  if (objects.empty())
    {
      obj_type = "[Empty]";
    }
  else if (objects.size() > 1)
    {
      obj_type = "[Group]";
    }
  else
    {
      obj_type = objects.front()->get_section_name();
    }
  gc.print_center(Fonts::courier_small, rect.get_width()/2, 10, 
                  obj_type);
}

void
ObjectProperties::set_objects(const std::vector<LevelObj*>& objs)
{
  objects = objs;
}

} // namespace Editor

/* EOF */
