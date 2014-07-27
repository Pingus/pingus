//  Pingus - A free Lemmings clone
//  Copyright (C) 2006 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "editor/object_properties.hpp"

#include "editor/button.hpp"
#include "editor/checkbox.hpp"
#include "editor/combobox.hpp"
#include "editor/gui_style.hpp"
#include "editor/inputbox.hpp"
#include "editor/label.hpp"
#include "editor/level_obj.hpp"
#include "pingus/gettext.h"
#include "pingus/groundtype.hpp"
#include "util/log.hpp"
#include "util/string_util.hpp"

namespace Editor {

ObjectProperties::ObjectProperties(EditorScreen* editor_, const Rect& rect_) :
  GUI::GroupComponent(rect_, false),
  editor(editor_),
  objects(),
  type_label(),
  mesg_label(),
  gptype_label(),
  gptype_type(),
  entrance_direction_label(),
  entrance_direction(),
  release_rate_label(),
  release_rate_inputbox(),
  stretch_label(),
  stretch_x_checkbox(),
  stretch_y_checkbox(),
  keep_aspect_label(),
  keep_aspect_checkbox(),
  para_x_label(),
  para_x_inputbox(),
  para_y_label(),
  para_y_inputbox(),
  scroll_x_label(),
  scroll_x_inputbox(),
  scroll_y_label(),
  scroll_y_inputbox(),
  owner_label(),
  owner_inputbox(),
  pos_z_label(),
  pos_z_inputbox(),
  color_label(),
  color_r_inputbox(),
  color_g_inputbox(),
  color_b_inputbox(),
  color_a_inputbox(),
  small_stars_label(),
  small_stars_inputbox(),
  middle_stars_label(),
  middle_stars_inputbox(),
  large_stars_label(),
  large_stars_inputbox(),
  repeat_label(),
  repeat_inputbox(),
  flip_horizontal_button(),
  flip_vertical_button(),
  rotate_90_button(),
  rotate_270_button(),
  id_label(),
  id_inputbox(),
  target_id_label(),
  target_id_inputbox(),
  height_label(),
  height_inputbox(),
  y_pos()
{
  type_label = create<Label>(Rect(Vector2i(4, 4), Size(120, 20)), _("Object:"));
  mesg_label = create<Label>(Rect(Vector2i(10, 0), Size(180, 20)), _("Nothing selected"));
    
  Rect label_rect(10,0, 80, 20);
  Rect box_rect(80,0, 190, 20);
  
  // Groundpiece Type
  gptype_label = create<Label>(label_rect, "GPType:");
  gptype_type = create<Combobox>(box_rect);

  gptype_type->add(Groundtype::GP_TRANSPARENT, _("Transparent"));
  gptype_type->add(Groundtype::GP_SOLID,       _("Solid"));
  gptype_type->add(Groundtype::GP_GROUND,      _("Ground"));
  gptype_type->add(Groundtype::GP_BRIDGE,      _("Bridge"));
  gptype_type->add(Groundtype::GP_WATER,       _("Water"));
  gptype_type->add(Groundtype::GP_LAVA,        _("Lava"));
  gptype_type->add(Groundtype::GP_REMOVE,      _("Remove"));
  gptype_type->set_selected_item(Groundtype::GP_GROUND);

  gptype_type->on_select.connect(std::bind(&ObjectProperties::on_gptype_change, this, std::placeholders::_1));
  
  entrance_direction_label = create<Label>(label_rect, _("Direction:"));
  entrance_direction = create<Combobox>(box_rect);
  entrance_direction->add(0, _("Left"));
  entrance_direction->add(1, _("Misc"));
  entrance_direction->add(2, _("Right"));
  entrance_direction->set_selected_item(0);

  entrance_direction->on_select.connect(std::bind(&ObjectProperties::on_entrance_direction_change, this, std::placeholders::_1));
  
  release_rate_label = create<Label>(label_rect, _("ReleaseRate:"));
  release_rate_inputbox = create<Inputbox>(box_rect);

  release_rate_inputbox->on_change.connect(std::bind(&ObjectProperties::on_release_rate_change, this, std::placeholders::_1));
  
  // Background Stretch
  stretch_label = create<Label>(label_rect, "Stretch:");
  stretch_x_checkbox = create<Checkbox>(Rect(Vector2i(box_rect.left, box_rect.top), 
                                             Size(box_rect.get_width()/2, box_rect.get_height())),
                                        "X");
  stretch_y_checkbox = create<Checkbox>(Rect(Vector2i(box_rect.left+box_rect.get_width()/2, box_rect.top), 
                                             Size(box_rect.get_width()/2, box_rect.get_height())),
                                        "Y");

  stretch_x_checkbox->on_change.connect(std::bind(&ObjectProperties::on_stretch_x_change, this, std::placeholders::_1));
  stretch_y_checkbox->on_change.connect(std::bind(&ObjectProperties::on_stretch_y_change, this, std::placeholders::_1));

  keep_aspect_label = create<Label>(label_rect, "Aspect:");
  keep_aspect_checkbox = create<Checkbox>(Rect(Vector2i(box_rect.left, box_rect.top), 
                                               Size(box_rect.get_width(), box_rect.get_height())),
                                          "keep");
  keep_aspect_checkbox->on_change.connect(std::bind(&ObjectProperties::on_keep_aspect_change, this, std::placeholders::_1));
 
  para_x_label = create<Label>(label_rect, _("Para-X:"));
  para_y_label = create<Label>(label_rect, _("Para-Y:"));

  para_x_inputbox = create<Inputbox>(box_rect);
  para_y_inputbox = create<Inputbox>(box_rect);

  para_x_inputbox->on_change.connect(std::bind(&ObjectProperties::on_para_x_change, this, std::placeholders::_1));
  para_y_inputbox->on_change.connect(std::bind(&ObjectProperties::on_para_y_change, this, std::placeholders::_1));
  
  scroll_x_label = create<Label>(label_rect, _("Scroll-X:"));
  scroll_y_label = create<Label>(label_rect, _("Scroll-Y:"));

  scroll_x_inputbox = create<Inputbox>(box_rect);
  scroll_y_inputbox = create<Inputbox>(box_rect);

  scroll_x_inputbox->on_change.connect(std::bind(&ObjectProperties::on_scroll_x_change, this, std::placeholders::_1));
  scroll_y_inputbox->on_change.connect(std::bind(&ObjectProperties::on_scroll_y_change, this, std::placeholders::_1));
  
  owner_label = create<Label>(label_rect, _("Owner Id:"));
  owner_inputbox = create<Inputbox>(box_rect);
  owner_inputbox->on_change.connect(std::bind(&ObjectProperties::on_owner_change, this, std::placeholders::_1));
  
  pos_z_label = create<Label>(label_rect, _("Z-Pos:"));
  pos_z_inputbox = create<Inputbox>(box_rect);
  pos_z_inputbox->on_change.connect(std::bind(&ObjectProperties::on_pos_z_change, this, std::placeholders::_1));
  // ___________________________________________________________________
  //  
  Size color_s(box_rect.get_width()/4, box_rect.get_height());

  color_label = create<Label>(label_rect, _("Color:"));
  color_r_inputbox = create<Inputbox>(Rect(Vector2i(box_rect.left + 0*color_s.width, box_rect.top), color_s));
  color_g_inputbox = create<Inputbox>(Rect(Vector2i(box_rect.left + 1*color_s.width, box_rect.top), color_s));
  color_b_inputbox = create<Inputbox>(Rect(Vector2i(box_rect.left + 2*color_s.width, box_rect.top), color_s));
  color_a_inputbox = create<Inputbox>(Rect(Vector2i(box_rect.left + 3*color_s.width, box_rect.top), color_s));

  color_r_inputbox->on_change.connect(std::bind(&ObjectProperties::on_color_r_change, this, std::placeholders::_1));
  color_g_inputbox->on_change.connect(std::bind(&ObjectProperties::on_color_g_change, this, std::placeholders::_1));
  color_b_inputbox->on_change.connect(std::bind(&ObjectProperties::on_color_b_change, this, std::placeholders::_1));
  color_a_inputbox->on_change.connect(std::bind(&ObjectProperties::on_color_a_change, this, std::placeholders::_1));
  // ___________________________________________________________________
  //
  small_stars_label    = create<Label>(label_rect, _("Small Stars:"));
  small_stars_inputbox = create<Inputbox>(box_rect);

  middle_stars_label    = create<Label>(label_rect, _("Middle Stars:"));
  middle_stars_inputbox = create<Inputbox>(box_rect);

  large_stars_label    = create<Label>(label_rect, _("Large Stars:"));
  large_stars_inputbox = create<Inputbox>(box_rect);

  small_stars_inputbox->on_change.connect(std::bind(&ObjectProperties::on_small_stars_change, this, std::placeholders::_1));
  middle_stars_inputbox->on_change.connect(std::bind(&ObjectProperties::on_middle_stars_change, this, std::placeholders::_1));
  large_stars_inputbox->on_change.connect(std::bind(&ObjectProperties::on_large_stars_change, this, std::placeholders::_1));
  // ___________________________________________________________________
  //
  repeat_label = create<Label>(label_rect, _("Repeat:"));
  repeat_inputbox = create<Inputbox>(box_rect);
  repeat_inputbox->on_change.connect(std::bind(&ObjectProperties::on_repeat_change, this, std::placeholders::_1));
  // ___________________________________________________________________
  //
  flip_horizontal_button = create<Button>(Rect(Vector2i(15+40*0-3, 0), Size(34, 34)), "|");
  flip_vertical_button   = create<Button>(Rect(Vector2i(15+40*1-3, 0), Size(34, 34)), "--");
  rotate_270_button      = create<Button>(Rect(Vector2i(15+40*2-3 + 20, 0), Size(34, 34)), "<-.");
  rotate_90_button       = create<Button>(Rect(Vector2i(15+40*3-3 + 20, 0), Size(34, 34)), ".->");

  flip_vertical_button->on_click.connect(std::bind(&ObjectProperties::on_flip_vertical, this));
  flip_horizontal_button->on_click.connect(std::bind(&ObjectProperties::on_flip_horizontal, this));
  rotate_90_button->on_click.connect(std::bind(&ObjectProperties::on_rotate_90, this));
  rotate_270_button->on_click.connect(std::bind(&ObjectProperties::on_rotate_270, this));
  // ___________________________________________________________________
  //
  id_label    = create<Label>(label_rect, _("Id:"));
  id_inputbox = create<Inputbox>(box_rect);
  id_inputbox->on_change.connect(std::bind(&ObjectProperties::on_id_change, this, std::placeholders::_1));

  target_id_label    = create<Label>(label_rect, _("Target Id:"));
  target_id_inputbox = create<Inputbox>(box_rect);
  target_id_inputbox->on_change.connect(std::bind(&ObjectProperties::on_target_id_change, this, std::placeholders::_1));
  // ___________________________________________________________________
  //
  height_label    = create<Label>(label_rect, _("Height:"));
  height_inputbox = create<Inputbox>(box_rect);
  height_inputbox->on_change.connect(std::bind(&ObjectProperties::on_height_change, this, std::placeholders::_1));
  // ___________________________________________________________________
  //
  set_object(LevelObjPtr());
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

  keep_aspect_label->hide();
  keep_aspect_checkbox->hide();

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

  small_stars_label->hide();
  middle_stars_label->hide();
  large_stars_label->hide();

  small_stars_inputbox->hide();
  middle_stars_inputbox->hide();
  large_stars_inputbox->hide();

  repeat_label->hide();
  repeat_inputbox->hide();

  flip_horizontal_button->hide();
  flip_vertical_button->hide();
  rotate_90_button->hide();
  rotate_270_button->hide();

  id_label->hide();
  id_inputbox->hide();

  target_id_label->hide();
  target_id_inputbox->hide();

  height_label->hide();
  height_inputbox->hide();
}

void
ObjectProperties::set_object(LevelObjPtr obj)
{
  hide_all();

  if (obj)
  {
    unsigned int attr = obj->get_attribs();
    if (attr & HAS_GPTYPE)
    {
      gptype_type->set_selected_item(Groundtype::string_to_type(obj->get_ground_type()));
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
        log_error("unknown direction: %1%", obj->get_direction());

      place(entrance_direction_label, entrance_direction);
    }
      
    if (attr & HAS_SPEED)
    { // obsolete in large part, since sprites have their own speed
    }
      
    if (attr & HAS_PARALLAX)
    { // used for hotspot
    }
        
    if (attr & HAS_REPEAT)
    { 
      repeat_inputbox->set_text(StringUtil::to_string(obj->get_repeat()));
      place(repeat_label, repeat_inputbox);
    }
        
    if (attr & HAS_OWNER)
    {
      owner_inputbox->set_text(StringUtil::to_string(obj->get_owner()));
      place(owner_label, owner_inputbox);
    }
        
    if (attr & HAS_COLOR)
    {
      color_r_inputbox->set_text(StringUtil::to_string(static_cast<int>(obj->get_color().r)));
      color_g_inputbox->set_text(StringUtil::to_string(static_cast<int>(obj->get_color().g)));
      color_b_inputbox->set_text(StringUtil::to_string(static_cast<int>(obj->get_color().b)));
      color_a_inputbox->set_text(StringUtil::to_string(static_cast<int>(obj->get_color().a)));

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

      keep_aspect_checkbox->set_checked(obj->get_keep_aspect());
      place(keep_aspect_label);
      place(keep_aspect_checkbox);
      advance();
    }

    if (attr & HAS_RELEASE_RATE)
    {
      release_rate_inputbox->set_text(StringUtil::to_string(obj->get_release_rate()));
      place(release_rate_label, release_rate_inputbox);
    }

    if (attr & HAS_STARFIELD)
    {
      small_stars_inputbox->set_text(StringUtil::to_string(obj->get_small_stars()));
      middle_stars_inputbox->set_text(StringUtil::to_string(obj->get_middle_stars()));
      large_stars_inputbox->set_text(StringUtil::to_string(obj->get_large_stars()));

      place(small_stars_label,  small_stars_inputbox);
      place(middle_stars_label, middle_stars_inputbox);
      place(large_stars_label,  large_stars_inputbox);
    }

    if (attr & HAS_ID)
    {
      id_inputbox->set_text(obj->get_id());
      place(id_label, id_inputbox);
    }

    if (attr & HAS_TARGET_ID)
    {
      target_id_inputbox->set_text(obj->get_target_id());
      place(target_id_label, target_id_inputbox);
    }

    if (attr & HAS_HEIGHT)
    {
      height_inputbox->set_text(StringUtil::to_string(obj->get_height()));
      place(height_label, height_inputbox);
    }

    if (1) // everybody has z-pos
    {
      pos_z_inputbox->set_text(StringUtil::to_string(obj->get_pos_z()));
      place(pos_z_label, pos_z_inputbox);
    }

    if (attr & CAN_ROTATE)
    {
      y_pos += 4;
      place(flip_horizontal_button);
      place(flip_vertical_button);
      place(rotate_90_button);
      place(rotate_270_button);
      y_pos += 36;
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
ObjectProperties::set_objects(const Selection& objs)
{
  objects = objs;

  std::string obj_type;
  if (objects.empty())
  {
    type_label->set_text(_("Object:"));
    mesg_label->set_text(_("Nothing selected"));
    set_object(LevelObjPtr());
  }
  else if (objects.size() > 1)
  {
    type_label->set_text(_("Object: [Group]"));
    mesg_label->set_text(_("Group not supported"));
    set_object(LevelObjPtr());
  }
  else
  {
    type_label->set_text(_("Object: ") + (*objects.begin())->get_section_name());
    set_object(*objects.begin());
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
  for(auto i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_ground_type(Groundtype::type_to_string(static_cast<Groundtype::GPType>(item.id)));
}

void
ObjectProperties::on_stretch_x_change(bool t)
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_stretch_x(t);
}

void
ObjectProperties::on_stretch_y_change(bool t)
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_stretch_y(t);
}

void
ObjectProperties::on_keep_aspect_change(bool t)
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_keep_aspect(t);
}

void
ObjectProperties::on_entrance_direction_change(const ComboItem& item)
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
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
  for(auto i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_owner(StringUtil::to<int>(str));
}

void
ObjectProperties::on_pos_z_change(const std::string& str)
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_pos_z(StringUtil::to<float>(str));
}

void
ObjectProperties::on_para_x_change(const std::string& str)
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_para_x(StringUtil::to<float>(str));
}

void
ObjectProperties::on_para_y_change(const std::string& str)
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_para_y(StringUtil::to<float>(str));
}

void
ObjectProperties::on_scroll_x_change(const std::string& str)
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_scroll_x(StringUtil::to<float>(str));
}

void
ObjectProperties::on_scroll_y_change(const std::string& str)
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_scroll_y(StringUtil::to<float>(str));
}

void
ObjectProperties::on_release_rate_change(const std::string& str)
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_release_rate(StringUtil::to<int>(str));
}

void
ObjectProperties::on_color_r_change(const std::string& str)
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
  {
    Color color = (*i)->get_color(); 
    color.r = static_cast<char>(StringUtil::to<int>(str));
    (*i)->set_color(color);
  }
}

void
ObjectProperties::on_color_g_change(const std::string& str)
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
  {
    Color color = (*i)->get_color(); 
    color.g = static_cast<uint8_t>(StringUtil::to<int>(str));
    (*i)->set_color(color);
  }
}

void
ObjectProperties::on_color_b_change(const std::string& str)
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
  {
    Color color = (*i)->get_color(); 
    color.b = static_cast<uint8_t>(StringUtil::to<int>(str));
    (*i)->set_color(color);
  }
}

void
ObjectProperties::on_color_a_change(const std::string& str)
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
  { 
    Color color = (*i)->get_color(); 
    color.a = static_cast<uint8_t>(StringUtil::to<int>(str));
    (*i)->set_color(color);
  }
}

void
ObjectProperties::on_small_stars_change(const std::string& str)
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
  {
    (*i)->set_small_stars(StringUtil::to<int>(str));
  }
}

void
ObjectProperties::on_middle_stars_change(const std::string& str)
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
  {
    (*i)->set_middle_stars(StringUtil::to<int>(str));
  }
}

void
ObjectProperties::on_large_stars_change(const std::string& str)
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
  {
    (*i)->set_large_stars(StringUtil::to<int>(str));
  }  
}

void
ObjectProperties::on_repeat_change(const std::string& str)
{
  int r = StringUtil::to<int>(str);
  if (r <= 0)
    r = 1;
  for(auto i = objects.begin(); i != objects.end(); ++i)
  {
    (*i)->set_repeat(r);
  }
}

void
ObjectProperties::on_flip_horizontal()
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_modifier(ResourceModifier::horizontal_flip((*i)->get_modifier()));
}

void
ObjectProperties::on_flip_vertical()
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_modifier(ResourceModifier::vertical_flip((*i)->get_modifier()));
}

void
ObjectProperties::on_rotate_90()
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_modifier(ResourceModifier::rotate_90((*i)->get_modifier()));
}

void
ObjectProperties::on_rotate_270()
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_modifier(ResourceModifier::rotate_270((*i)->get_modifier()));
}

void
ObjectProperties::on_id_change(const std::string& str)
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_id(str);
}

void
ObjectProperties::on_target_id_change(const std::string& str)
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_target_id(str);
}

void
ObjectProperties::on_height_change(const std::string& str)
{
  for(auto i = objects.begin(); i != objects.end(); ++i)
    (*i)->set_height(StringUtil::to<int>(str));
}

void
ObjectProperties::update_layout()
{
  GroupComponent::update_layout();
}

} // namespace Editor

/* EOF */
