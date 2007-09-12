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
#include "inputbox.hpp"
#include "checkbox.hpp"
#include "object_properties.hpp"

namespace Editor {

ObjectProperties::ObjectProperties(EditorScreen* editor_, const Rect& rect)
  : GUI::GroupComponent(rect, false),
    editor(editor_)
{
  add(type_label = new Label(Rect(Vector2i(4, 4), Size(120, 20)), "Object:"), true);
  add(mesg_label = new Label(Rect(Vector2i(40, rect.get_height()/2- 20), Size(120, 20)), "nothing selected"), true);
    
  // Groundpiece Type
  add(gptype_label = new Label(Rect(Vector2i(10, 0), Size(90, 20)), "GPType:"), true);
  add(gptype_type  = new Combobox(Rect(Vector2i(100, 0), Size(90, 20))), true);

  gptype_type->add(Groundtype::GP_TRANSPARENT, "Transparent");
  gptype_type->add(Groundtype::GP_SOLID,       "Solid");
  gptype_type->add(Groundtype::GP_GROUND,      "Ground");
  gptype_type->add(Groundtype::GP_BRIDGE,      "Bridge");
  gptype_type->add(Groundtype::GP_WATER,       "Water");
  gptype_type->add(Groundtype::GP_LAVA,        "Lava");
  gptype_type->add(Groundtype::GP_REMOVE,      "Remove");
  gptype_type->set_selected_item(Groundtype::GP_GROUND);

  gptype_type->on_select.connect(boost::bind(&ObjectProperties::on_gptype_change, this, _1));
  
  add(entrance_direction_label = new Label(Rect(Vector2i(10, 0), Size(90, 20)), "Direction:"), true);
  add(entrance_direction = new Combobox(Rect(Vector2i(100, 0), Size(90, 20))), true);
  entrance_direction->add(0, "Right");
  entrance_direction->add(1, "Misc");
  entrance_direction->add(2, "Left");
  entrance_direction->set_selected_item(0);

  entrance_direction->on_select.connect(boost::bind(&ObjectProperties::on_entrance_direction_change, this, _1));
  
  // Background Stretch
  add(stretch_x_checkbox = new Checkbox(Rect(Vector2i(10, 0),  Size(90, 20)), "Stretch-X"), true);
  add(stretch_y_checkbox = new Checkbox(Rect(Vector2i(100, 0), Size(90, 20)), "Stretch-Y"), true);

  stretch_x_checkbox->on_change.connect(boost::bind(&ObjectProperties::on_stretch_x_change, this, _1));
  stretch_y_checkbox->on_change.connect(boost::bind(&ObjectProperties::on_stretch_y_change, this, _1));
  
  add(para_x_label = new Label(Rect(Vector2i(10, 0), Size(90,20)), "Para-X:"), true);
  add(para_y_label = new Label(Rect(Vector2i(10, 0), Size(90,20)), "Para-Y:"), true);

  add(para_x_inputbox = new Inputbox(Rect(Vector2i(100, 0), Size(90,20))), true);
  add(para_y_inputbox = new Inputbox(Rect(Vector2i(100, 0), Size(90,20))), true);
  
  add(scroll_x_label = new Label(Rect(Vector2i(10, 0), Size(90,20)), "Scroll-X:"), true);
  add(scroll_y_label = new Label(Rect(Vector2i(10, 0), Size(90,20)), "Scroll-Y:"), true);

  add(scroll_x_inputbox = new Inputbox(Rect(Vector2i(100, 0), Size(90,20))), true);
  add(scroll_y_inputbox = new Inputbox(Rect(Vector2i(100, 0), Size(90,20))), true);
  
  add(owner_label    = new Label(Rect(Vector2i(10,0), Size(90,20)), "Owner Id:"), true);
  add(owner_inputbox = new Inputbox(Rect(Vector2i(100,0), Size(90,20))), true);

  hide_all();
}

ObjectProperties::~ObjectProperties()
{
}

void
ObjectProperties::place(GUI::RectComponent* comp)
{
  Rect crect = comp->get_rect();
  comp->set_rect(Rect(crect.left, 
                      y_pos, 
                      crect.right, 
                      y_pos + crect.get_height()));
  comp->show();
  y_pos += 22;
}

void
ObjectProperties::place(GUI::RectComponent* comp1, GUI::RectComponent* comp2)
{
  Rect rect1 = comp1->get_rect();
  Rect rect2 = comp2->get_rect();

  comp1->set_rect(Rect(rect1.left, 
                       y_pos, 
                       rect1.right, 
                       y_pos + rect1.get_height()));

  comp2->set_rect(Rect(rect2.left, 
                       y_pos, 
                       rect2.right, 
                       y_pos + rect2.get_height()));

  comp1->show();
  comp2->show();

  y_pos += 22;
}

void
ObjectProperties::hide_all()
{
  // Hide everything
  mesg_label->hide();

  gptype_label->hide();
  gptype_type->hide();
  
  entrance_direction_label->hide();
  entrance_direction->hide();
 
  stretch_x_checkbox->hide();
  stretch_y_checkbox->hide();
 
  para_x_label->hide();
  para_x_inputbox->hide();
 
  para_y_label->hide();
  para_y_inputbox->hide();

  scroll_x_label->hide();
  scroll_x_inputbox->hide();
 
  scroll_y_label->hide();
  scroll_y_inputbox->hide();

  owner_label->hide();
  owner_inputbox->hide();

}

void
ObjectProperties::set_object(LevelObj* obj)
{
  hide_all();

  if (obj)
    {
      unsigned int attr = obj->get_attribs();
      y_pos = 30;
      if (attr & HAS_TYPE)
        {
          place(gptype_label, gptype_type);
          // FIXME:  update the content of the box
        }
      
      if (attr & HAS_DIRECTION)
        {
          place(entrance_direction_label, entrance_direction);
        }
      
      if (attr & HAS_SPEED)
        {
        }
      
      if (attr & HAS_PARALLAX)
        {
        }
        
      if (attr & HAS_WIDTH)
        {
        }
        
      if (attr & HAS_OWNER)
        {
          place(owner_label, owner_inputbox);
        }
        
      if (attr & HAS_COLOR)
        {
        }

      if (attr & HAS_SCROLL)
        {
          place(scroll_x_label, scroll_x_inputbox);
          place(scroll_y_label, scroll_y_inputbox);
        }

      if (attr & HAS_PARA)
        {
          place(para_x_label, para_x_inputbox);
          place(para_y_label, para_y_inputbox);
        }

      if (attr & HAS_STRETCH)
        {
          place(stretch_x_checkbox, stretch_y_checkbox);
        }

      if (attr & HAS_RELEASE_RATE)
        {
        }
    }
  else
    {
      mesg_label->show();
    }
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
      type_label->set_text("Object:");
      mesg_label->set_text("Nothing selected");
      set_object(0);
    }
  else if (objects.size() > 1)
    {
      type_label->set_text("Object: [Group]");
      mesg_label->set_text("Group not supported");
      set_object(0);
    }
  else
    {
      type_label->set_text("Object: " + objects.front()->get_section_name());
      set_object(objects.front());
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
