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
  add(mesg_label = new Label(Rect(Vector2i(10, 0), Size(180, 20)), "Nothing selected"), true);
    
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

  release_rate_inputbox->on_change.connect(boost::bind(&ObjectProperties::on_release_rate_change, this, _1));
  
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

  para_x_inputbox->on_change.connect(boost::bind(&ObjectProperties::on_para_x_change, this, _1));
  para_y_inputbox->on_change.connect(boost::bind(&ObjectProperties::on_para_y_change, this, _1));
  
  add(scroll_x_label = new Label(label_rect, "Scroll-X:"), true);
  add(scroll_y_label = new Label(label_rect, "Scroll-Y:"), true);

  add(scroll_x_inputbox = new Inputbox(box_rect), true);
  add(scroll_y_inputbox = new Inputbox(box_rect), true);

  scroll_x_inputbox->on_change.connect(boost::bind(&ObjectProperties::on_scroll_x_change, this, _1));
  scroll_y_inputbox->on_change.connect(boost::bind(&ObjectProperties::on_scroll_y_change, this, _1));
  
  add(owner_label    = new Label(label_rect, "Owner Id:"), true);
  add(owner_inputbox = new Inputbox(box_rect), true);
  owner_inputbox->on_change.connect(boost::bind(&ObjectProperties::on_owner_change, this, _1));
  
  add(pos_z_label    = new Label(label_rect, "Z-Pos:"), true);
  add(pos_z_inputbox = new Inputbox(box_rect), true);
  pos_z_inputbox->on_change.connect(boost::bind(&ObjectProperties::on_pos_z_change, this, _1));
  
  Size color_s(box_rect.get_width()/4, box_rect.get_height());

  add(color_label = new Label(label_rect, "Color:"), true);
  add(color_r_inputbox = new Inputbox(Rect(Vector2i(box_rect.left + 0*color_s.width, box_rect.top), color_s)), true);
  add(color_g_inputbox = new Inputbox(Rect(Vector2i(box_rect.left + 1*color_s.width, box_rect.top), color_s)), true);
  add(color_b_inputbox = new Inputbox(Rect(Vector2i(box_rect.left + 2*color_s.width, box_rect.top), color_s)), true);
  add(color_a_inputbox = new Inputbox(Rect(Vector2i(box_rect.left + 3*color_s.width, box_rect.top), color_s)), true);

  color_r_inputbox->on_change.connect(boost::bind(&ObjectProperties::on_color_r_change, this, _1));
  color_g_inputbox->on_change.connect(boost::bind(&ObjectProperties::on_color_g_change, this, _1));
  color_b_inputbox->on_change.connect(boost::bind(&ObjectProperties::on_color_b_change, this, _1));
  color_a_inputbox->on_change.connect(boost::bind(&ObjectProperties::on_color_a_change, this, _1));
  
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
  y_pos = 30;
      
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

  pos_z_label->hide();
  pos_z_inputbox->hide();

  color_label->hide();
  color_r_inputbox->hide();
  color_g_inputbox->hide();
  color_b_inputbox->hide();
  color_a_inputbox->hide();
}

void
ObjectProperties::set_object(LevelObj* obj)
{
  hide_all();

  if (obj)
    {
      unsigned int attr = obj->get_attribs();
      if (attr & HAS_GPTYPE)
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
        { // obsolete in large part, since sprites have their own speed
        }
      
      if (attr & HAS_PARALLAX)
        { // used for hotspot
        }
        
      if (attr & HAS_WIDTH)
        { // used by liquid and some obscure ones
        }
        
      if (attr & HAS_OWNER)
        {
          owner_inputbox->set_text(StringUtil::to_string(obj->get_owner()));
          place(owner_label, owner_inputbox);
        }
        
      if (attr & HAS_COLOR)
        {
          color_r_inputbox->set_text(StringUtil::to_string((int)obj->get_color().r));
          color_g_inputbox->set_text(StringUtil::to_string((int)obj->get_color().g));
          color_b_inputbox->set_text(StringUtil::to_string((int)obj->get_color().b));
          color_a_inputbox->set_text(StringUtil::to_string((int)obj->get_color().a));

          place(color_label);
          place(color_r_inputbox);
          place(color_g_inputbox);
          place(color_b_inputbox);
          place(color_a_inputbox);
          advance();
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

      if (1) // everybody has z-pos
        {
          pos_z_inputbox->set_text(StringUtil::to_string(obj->get_pos_z()));
          place(pos_z_label, pos_z_inputbox);
        }
    }
  else
    {
      place(mesg_label);
      advance();
    }
  finalize();
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
ObjectProperties::finalize()
{
  set_rect(Rect(rect.left, rect.bottom - y_pos - 10, rect.right, rect.bottom));
}

void
ObjectProperties::on_gptype_change(const ComboItem& item)
{
  for(Objects::iterator i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_ground_type(Groundtype::type_to_string((Groundtype::GPType)item.id));
}

void
ObjectProperties::on_stretch_x_change(bool t)
{
  for(Objects::iterator i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_stretch_x(t);
}

void
ObjectProperties::on_stretch_y_change(bool t)
{
  for(Objects::iterator i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_stretch_y(t);
}

void
ObjectProperties::on_entrance_direction_change(const ComboItem& item)
{
  for(Objects::iterator i = objects.begin(); i != objects.end(); ++i)
    {
      if (item.id == 0)
        (*i)->set_direction("left");
      else if (item.id == 1)
        (*i)->set_direction("misc");
      else // (item.id == 2)
        (*i)->set_direction("right");
    }
}

void
ObjectProperties::on_owner_change(const std::string& str)
{
  for(Objects::iterator i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_owner(StringUtil::to<int>(str));
}


void
ObjectProperties::on_pos_z_change(const std::string& str)
{
  for(Objects::iterator i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_pos_z(StringUtil::to<float>(str));
}

void
ObjectProperties::on_para_x_change(const std::string& str)
{
  for(Objects::iterator i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_para_x(StringUtil::to<float>(str));
}

void
ObjectProperties::on_para_y_change(const std::string& str)
{
  for(Objects::iterator i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_para_y(StringUtil::to<float>(str));
}

void
ObjectProperties::on_scroll_x_change(const std::string& str)
{
  for(Objects::iterator i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_scroll_x(StringUtil::to<float>(str));
}

void
ObjectProperties::on_scroll_y_change(const std::string& str)
{
  for(Objects::iterator i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_scroll_y(StringUtil::to<float>(str));
}

void
ObjectProperties::on_release_rate_change(const std::string& str)
{
  for(Objects::iterator i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_release_rate(StringUtil::to<int>(str));
}

void
ObjectProperties::on_color_r_change(const std::string& str)
{
  for(Objects::iterator i = objects.begin(); i != objects.end(); ++i)
    {
      Color color = (*i)->get_color(); 
      color.r = StringUtil::to<int>(str);
      (*i)->set_color(color);
    }
}

void
ObjectProperties::on_color_g_change(const std::string& str)
{
  for(Objects::iterator i = objects.begin(); i != objects.end(); ++i)
    {
      Color color = (*i)->get_color(); 
      color.g = StringUtil::to<int>(str);
      (*i)->set_color(color);
    }
}

void
ObjectProperties::on_color_b_change(const std::string& str)
{
  for(Objects::iterator i = objects.begin(); i != objects.end(); ++i)
    {
      Color color = (*i)->get_color(); 
      color.b = StringUtil::to<int>(str);
      (*i)->set_color(color);
    }
}

void
ObjectProperties::on_color_a_change(const std::string& str)
{
  for(Objects::iterator i = objects.begin(); i != objects.end(); ++i)
    { 
      Color color = (*i)->get_color(); 
      color.a = StringUtil::to<int>(str);
      (*i)->set_color(color);
    }
}


} // namespace Editor

/* EOF */
