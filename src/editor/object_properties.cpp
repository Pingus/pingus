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
#include "string_util.hpp"
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
    
  Rect label_rect(10,0, 80, 20);
  Rect box_rect(80,0, 190, 20);
 
  // Groundpiece Type
  add(gptype_label = new Label(label_rect, "GPType:"), true);
  add(gptype_type  = new Combobox(box_rect), true);

  gptype_type->add(Groundtype::GP_TRANSPARENT, "Transparent");
  gptype_type->add(Groundtype::GP_SOLID,       "Solid");
  gptype_type->add(Groundtype::GP_GROUND,      "Ground");
  gptype_type->add(Groundtype::GP_BRIDGE,      "Bridge");
  gptype_type->add(Groundtype::GP_WATER,       "Water");
  gptype_type->add(Groundtype::GP_LAVA,        "Lava");
  gptype_type->add(Groundtype::GP_REMOVE,      "Remove");
  gptype_type->set_selected_item(Groundtype::GP_GROUND);

  gptype_type->on_select.connect(boost::bind(&ObjectProperties::on_gptype_change, this, _1));
  
  add(entrance_direction_label = new Label(label_rect, "Direction:"), true);
  add(entrance_direction = new Combobox(box_rect), true);
  entrance_direction->add(0, "Left");
  entrance_direction->add(1, "Misc");
  entrance_direction->add(2, "Right");
  entrance_direction->set_selected_item(0);

  entrance_direction->on_select.connect(boost::bind(&ObjectProperties::on_entrance_direction_change, this, _1));

  add(release_rate_label = new Label(label_rect, "ReleaseRate:"), true);
  add(release_rate_inputbox = new Inputbox(box_rect), true);
  
  // Background Stretch
  add(stretch_label = new Label(label_rect, "Stretch:"), true);
  add(stretch_x_checkbox = new Checkbox(Rect(Vector2i(box_rect.left, box_rect.top), 
                                             Size(box_rect.get_width()/2, box_rect.get_height())),
                                        "X"), true);
  add(stretch_y_checkbox = new Checkbox(Rect(Vector2i(box_rect.left+box_rect.get_width()/2, box_rect.top), 
                                             Size(box_rect.get_width()/2, box_rect.get_height())),
                                        "Y"), true);

  stretch_x_checkbox->on_change.connect(boost::bind(&ObjectProperties::on_stretch_x_change, this, _1));
  stretch_y_checkbox->on_change.connect(boost::bind(&ObjectProperties::on_stretch_y_change, this, _1));
  
  add(para_x_label = new Label(label_rect, "Para-X:"), true);
  add(para_y_label = new Label(label_rect, "Para-Y:"), true);

  add(para_x_inputbox = new Inputbox(box_rect), true);
  add(para_y_inputbox = new Inputbox(box_rect), true);
  
  add(scroll_x_label = new Label(label_rect, "Scroll-X:"), true);
  add(scroll_y_label = new Label(label_rect, "Scroll-Y:"), true);

  add(scroll_x_inputbox = new Inputbox(box_rect), true);
  add(scroll_y_inputbox = new Inputbox(box_rect), true);
  
  add(owner_label    = new Label(label_rect, "Owner Id:"), true);
  add(owner_inputbox = new Inputbox(box_rect), true);

  set_object(0);
}

ObjectProperties::~ObjectProperties()
{
}

void
ObjectProperties::advance()
{
  y_pos += 22;
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
 
  release_rate_label->hide();
  release_rate_inputbox->hide();

  stretch_label->hide();
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
          gptype_type->set_selected_item(Groundtype::string_to_type(obj->get_type()));
          place(gptype_label, gptype_type);
        }
      
      if (attr & HAS_DIRECTION)
        {
          if (obj->get_direction() == "left")
            entrance_direction->set_selected_item(0);
          else if (obj->get_direction() == "misc")
            entrance_direction->set_selected_item(1);
          else if (obj->get_direction() == "right")
            entrance_direction->set_selected_item(2);
          else
            std::cout << "Error: ObjectProperties::set_object: unknown direction: " << obj->get_direction() << std::endl;

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
          owner_inputbox->set_text(StringUtil::to_string(obj->get_owner()));
          place(owner_label, owner_inputbox);
        }
        
      if (attr & HAS_COLOR)
        {
        }

      if (attr & HAS_SCROLL)
        {
          scroll_x_inputbox->set_text(StringUtil::to_string(obj->get_scroll_x()));
          scroll_y_inputbox->set_text(StringUtil::to_string(obj->get_scroll_y()));          

          place(scroll_x_label, scroll_x_inputbox);
          place(scroll_y_label, scroll_y_inputbox);
        }

      if (attr & HAS_PARA)
        {
          para_x_inputbox->set_text(StringUtil::to_string(obj->get_para_x()));
          para_y_inputbox->set_text(StringUtil::to_string(obj->get_para_y()));

          place(para_x_label, para_x_inputbox);
          place(para_y_label, para_y_inputbox);
        }

      if (attr & HAS_STRETCH)
        {
          stretch_x_checkbox->set_checked(obj->get_stretch_y());
          stretch_y_checkbox->set_checked(obj->get_stretch_y());

          place(stretch_label);
          place(stretch_x_checkbox);
          place(stretch_y_checkbox);
          advance();
        }

      if (attr & HAS_RELEASE_RATE)
        {
          release_rate_inputbox->set_text(StringUtil::to_string(obj->get_release_rate()));
          place(release_rate_label, release_rate_inputbox);
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
  for(Objects::iterator i = objects.begin(); i != objects.end(); ++i)
    {
      (*i)->set_type(Groundtype::type_to_string((Groundtype::GPType)item.id));
    }
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
  for(Objects::iterator i = objects.begin(); i != objects.end(); ++i)
    {
      if (item.id == 0)
        (*i)->set_direction("left");
      else if (item.id == 1)
        (*i)->set_direction("left");
      else // (item.id == 2)
        (*i)->set_direction("right");
    }
}

} // namespace Editor

/* EOF */
