//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_EDITOR_OBJECT_PROPERTIES_HPP
#define HEADER_PINGUS_EDITOR_OBJECT_PROPERTIES_HPP

#include "editor/selection.hpp"
#include "engine/gui/group_component.hpp"

namespace Editor {

class Button;
class Checkbox;
class ComboItem;
class Combobox;
class EditorScreen;
class Inputbox;
class Label;
class LevelObj;

class ObjectProperties : public GUI::GroupComponent
{
private:
  EditorScreen* editor;
  Selection objects;

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

  Label*    keep_aspect_label;
  Checkbox* keep_aspect_checkbox;
  
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
  
  Label*    small_stars_label;
  Inputbox* small_stars_inputbox;

  Label*    middle_stars_label;
  Inputbox* middle_stars_inputbox;

  Label*    large_stars_label;
  Inputbox* large_stars_inputbox;

  Label*    repeat_label;
  Inputbox* repeat_inputbox;

  Button*   flip_horizontal_button;
  Button*   flip_vertical_button;
  Button*   rotate_90_button;
  Button*   rotate_270_button;

  Label*    id_label;
  Inputbox* id_inputbox;

  Label*    target_id_label;
  Inputbox* target_id_inputbox;

  Label*    height_label;
  Inputbox* height_inputbox;

  int y_pos;

public:
  ObjectProperties(EditorScreen* editor, const Rect& rect);
  ~ObjectProperties();
  
  void set_object(LevelObjPtr obj);
  void draw_background(DrawingContext& gc);
  void update_layout();

  void set_objects(const Selection& objs);

  // GUI Placement functions
  void hide_all();
  void advance();
  void finalize();
  void place(GUI::RectComponent* comp);
  void place(GUI::RectComponent* comp1, GUI::RectComponent* comp2);

  void on_gptype_change(const ComboItem& item);
  void on_stretch_x_change(bool t);
  void on_stretch_y_change(bool t);
  void on_keep_aspect_change(bool t);
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

  void on_small_stars_change(const std::string& str);
  void on_middle_stars_change(const std::string& str);
  void on_large_stars_change(const std::string& str);

  void on_repeat_change(const std::string& str);

  void on_id_change(const std::string& str);
  void on_target_id_change(const std::string& str);
  
  void on_height_change(const std::string& str);

  void on_flip_horizontal();
  void on_flip_vertical();
  void on_rotate_90();
  void on_rotate_270();

private:
  ObjectProperties(const ObjectProperties&);
  ObjectProperties & operator=(const ObjectProperties&);
};

} // namespace Editor

#endif

/* EOF */
