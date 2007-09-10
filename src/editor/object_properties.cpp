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

#include <boost/bind.hpp>
#include "button.hpp"
#include "editor_screen.hpp"
#include "level_objs.hpp"
#include "fonts.hpp"
#include "label.hpp"
#include "gui_style.hpp"
#include "groundtype.hpp"
#include "combobox.hpp"
#include "checkbox.hpp"
#include "object_properties.hpp"

namespace Editor {

ObjectProperties::ObjectProperties(EditorScreen* editor_, const Rect& rect)
  : GUI::GroupComponent(rect, false),
    editor(editor_)
{
  add(type_label = new Label(Rect(Vector2i(10, 10), Size(120, 20)), "Object:"), true);
  

  // Groundpiece Type
  add(gptype_label = new Label(Rect(Vector2i(10, 30), Size(120, 20)), "GPType:"), true);
  add(gptype_type  = new Combobox(Rect(Vector2i(60, 30), Size(120, 20))), true);

  gptype_type->add(Groundtype::GP_TRANSPARENT, "Transparent");
  gptype_type->add(Groundtype::GP_SOLID,       "Solid");
  gptype_type->add(Groundtype::GP_GROUND,      "Ground");
  gptype_type->add(Groundtype::GP_BRIDGE,      "Bridge");
  gptype_type->add(Groundtype::GP_WATER,       "Water");
  gptype_type->add(Groundtype::GP_LAVA,        "Lava");
  gptype_type->add(Groundtype::GP_REMOVE,      "Remove");
  gptype_type->set_selected_item(Groundtype::GP_GROUND);

  gptype_type->on_select.connect(boost::bind(&ObjectProperties::on_gptype_change, this, _1));

  add(entrance_direction_label = new Label(Rect(Vector2i(10, 90), Size(80, 20)), "Direction"), true);
  add(entrance_direction = new Combobox(Rect(Vector2i(60, 90), Size(120, 20))), true);
  entrance_direction->add(0, "Right");
  entrance_direction->add(1, "Misc");
  entrance_direction->add(2, "Left");
  entrance_direction->set_selected_item(0);

  entrance_direction->on_select.connect(boost::bind(&ObjectProperties::on_entrance_direction_change, this, _1));
 
  // Background Stretch
  add(stretch_x_checkbox = new Checkbox(Rect(Vector2i(10, 60),  Size(80, 20)), "Stretch-X"), true);
  add(stretch_y_checkbox = new Checkbox(Rect(Vector2i(110, 60), Size(80, 20)), "Stretch-Y"), true);

  stretch_x_checkbox->on_change.connect(boost::bind(&ObjectProperties::on_stretch_x_change, this, _1));
  stretch_y_checkbox->on_change.connect(boost::bind(&ObjectProperties::on_stretch_y_change, this, _1));
 
  if (0)
    {
      add(new Label(Rect(Vector2i(  10,  30), Size( 80, 20)), "Type:"), true);

      add(new Button(Rect(Vector2i( 60,  30), Size( 80, 20)), "Ground"), true);
      add(new Button(Rect(Vector2i( 60,  50), Size( 80, 20)), "Transparent"), true);
      add(new Button(Rect(Vector2i( 60,  70), Size( 80, 20)), "Solid"), true);
      add(new Button(Rect(Vector2i(140,  30), Size( 80, 20)), "Bridge"), true);
      add(new Button(Rect(Vector2i(140,  50), Size( 80, 20)), "Remove"), true);
    }
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

void
ObjectProperties::on_gptype_change(const ComboItem& item)
{
  std::cout << "ObjectProperties::on_gpytpe_change: switch to: "
            << Groundtype::type_to_string((Groundtype::GPType)item.id) << std::endl;
}

void
ObjectProperties::on_stretch_x_change(bool t)
{
  std::cout << "ObjectProperties::on_stretch_x_change: switch to: " << t << std::endl;
}

void
ObjectProperties::on_stretch_y_change(bool t)
{
  std::cout << "ObjectProperties::on_stretch_y_change: switch to: " << t << std::endl;
}

void
ObjectProperties::on_entrance_direction_change(const ComboItem& item)
{
  std::cout << "ObjectProperties::on_entrance_direction_change: " << item.label << std::endl;
}

} // namespace Editor

/* EOF */
