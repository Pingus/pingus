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
#include "label.hpp"
#include "gui_style.hpp"
#include "object_properties.hpp"

namespace Editor {

ObjectProperties::ObjectProperties(EditorScreen* editor_, const Rect& rect)
  : GUI::GroupComponent(rect),
    editor(editor_)
{
  add(type_label = new Label(Rect(Vector2i(10, 10), Size(120, 20)), "Object:"), true);

  add(new Label(Rect(Vector2i(  10,  30), Size( 80, 20)), "Type:"), true);

  add(new Button(Rect(Vector2i( 60,  30), Size( 80, 20)), "Ground"), true);
  add(new Button(Rect(Vector2i( 60,  50), Size( 80, 20)), "Transparent"), true);
  add(new Button(Rect(Vector2i( 60,  70), Size( 80, 20)), "Solid"), true);
  add(new Button(Rect(Vector2i(140,  30), Size( 80, 20)), "Bridge"), true);
  add(new Button(Rect(Vector2i(140,  50), Size( 80, 20)), "Remove"), true);
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
  GUIStyle::draw_raised_box(gc, Rect(0,0, rect.get_width(), rect.get_height()));
}

void
ObjectProperties::set_objects(const std::vector<LevelObj*>& objs)
{
  objects = objs;

  std::string obj_type;
  if (objects.empty())
    {
      type_label->set_text("Object: [Empty]");
    }
  else if (objects.size() > 1)
    {
      type_label->set_text("Object: [Group]");
    }
  else
    {
      type_label->set_text("Object: " + objects.front()->get_section_name());
    }
}

} // namespace Editor

/* EOF */
