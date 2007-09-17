//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_EDITOR_OBJECT_PROPERTIES_HPP
#define HEADER_EDITOR_OBJECT_PROPERTIES_HPP

#include "gui/group_component.hpp"

namespace Editor {

class Label;
class EditorScreen;
class Combobox;
class ComboItem;
class Checkbox;
class Inputbox;

/** */
class ObjectProperties : public GUI::GroupComponent
{
private:
  EditorScreen* editor;
  typedef std::vector<LevelObj*> Objects;
  Objects objects;

  Label* type_label;
  Label* mesg_label;

  // Groundpiece Type
  Label*    gptype_label;
  Combobox* gptype_type;

  Label*    entrance_direction_label;
  Combobox* entrance_direction;

  Label*    release_rate_label;
  Inputbox* release_rate_inputbox;

  Label*    stretch_label;
  Checkbox* stretch_x_checkbox;
  Checkbox* stretch_y_checkbox;
  
  Label*    para_x_label;
  Inputbox* para_x_inputbox;

  Label*    para_y_label;
  Inputbox* para_y_inputbox;

  Label*    scroll_x_label;
  Inputbox* scroll_x_inputbox;

  Label*    scroll_y_label;
  Inputbox* scroll_y_inputbox;

  Label*    owner_label;
  Inputbox* owner_inputbox;

  Label*    pos_z_label;
  Inputbox* pos_z_inputbox;

  Label*    color_label;
  Inputbox* color_r_inputbox;
  Inputbox* color_g_inputbox;
  Inputbox* color_b_inputbox;
  Inputbox* color_a_inputbox;

  int y_pos;
public:
  ObjectProperties(EditorScreen* editor, const Rect& rect);
  ~ObjectProperties();
  
  void set_object(LevelObj* obj);
  void draw_background(DrawingContext& gc);

  void set_objects(const std::vector<LevelObj*>& objs);

  // GUI Placement functions
  void hide_all();
  void advance();
  void finalize();
  void place(GUI::RectComponent* comp);
  void place(GUI::RectComponent* comp1, GUI::RectComponent* comp2);

  void on_gptype_change(const ComboItem& item);
  void on_stretch_x_change(bool t);
  void on_stretch_y_change(bool t);
  void on_entrance_direction_change(const ComboItem& item);
  void on_owner_change(const std::string& str);
  void on_para_x_change(const std::string& str);
  void on_para_y_change(const std::string& str);
  void on_scroll_x_change(const std::string& str);
  void on_scroll_y_change(const std::string& str);
  void on_release_rate_change(const std::string& str);
  void on_pos_z_change(const std::string& str);

  void on_color_r_change(const std::string& str);
  void on_color_g_change(const std::string& str);
  void on_color_b_change(const std::string& str);
  void on_color_a_change(const std::string& str);

};

} // namespace Editor

#endif

/* EOF */
