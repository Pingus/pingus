// Pingus - A free Lemmings clone
// Copyright (C) 2007 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_EDITOR_OBJECT_PROPERTIES_HPP
#define HEADER_PINGUS_EDITOR_OBJECT_PROPERTIES_HPP

#include <array>
#include <map>
#include <string>
#include <vector>

#include "editor/selection.hpp"
#include "engine/gui/group_component.hpp"
#include "pingus/object_schema.hpp"

namespace pingus::editor {

class Button;
class Checkbox;
class Combobox;
class EditorScreen;
class Inputbox;
class Label;
class LevelObj;

/** Panel showing and editing the properties of the selected object. The
    widgets for each object type are generated from its ObjectTypeDef in
    the ObjectSchema; the position and the rotation buttons are shown for
    all objects. */
class ObjectProperties : public gui::GroupComponent
{
private:
  /** The widgets editing one property, which of them exist depends on
      the property type */
  struct PropertyWidgets
  {
    PropertyDef const* prop = nullptr;
    Label* label = nullptr;
    Inputbox* inputbox = nullptr;
    Checkbox* checkbox = nullptr;
    Combobox* combobox = nullptr;
    std::array<Inputbox*, 4> color = {};
  };

  EditorScreen* editor;
  Selection objects;

  Label* type_label;
  Label* mesg_label;

  Label*    pos_x_label;
  Inputbox* pos_x_inputbox;
  Label*    pos_y_label;
  Inputbox* pos_y_inputbox;
  Label*    pos_z_label;
  Inputbox* pos_z_inputbox;

  Button*   flip_horizontal_button;
  Button*   flip_vertical_button;
  Button*   rotate_90_button;
  Button*   rotate_270_button;

  /** Widgets per object type, created when an object of the type is
      first selected */
  std::map<ObjectTypeDef const*, std::vector<PropertyWidgets>> property_widgets;

  int y_pos;

public:
  ObjectProperties(EditorScreen* editor, Rect const& rect);
  ~ObjectProperties() override;

  ObjectProperties(ObjectProperties const&) = delete;
  ObjectProperties& operator=(ObjectProperties const&) = delete;

  void set_object(LevelObjPtr const& obj);
  void draw_background(DrawingContext& gc) override;
  void update_layout() override;

  void set_objects(Selection const& objs);

private:
  // GUI Placement functions
  void hide_all();
  void advance();
  void finalize();
  void place(gui::RectComponent* comp);
  void place(gui::RectComponent* comp1, gui::RectComponent* comp2);

  std::vector<PropertyWidgets>& get_property_widgets(ObjectTypeDef const& type);
  PropertyWidgets create_property_widgets(PropertyDef const& prop);

  /** Show the property's widgets with the value from 'data' */
  void show_property(PropertyWidgets& widgets, LevelObj const& data);

  /** Set the property on all selected objects */
  void set_property(std::string const& name, PropertyValue const& value);

  void on_pos_x_change(std::string const& str);
  void on_pos_y_change(std::string const& str);
  void on_pos_z_change(std::string const& str);

  void on_flip_horizontal();
  void on_flip_vertical();
  void on_rotate_90();
  void on_rotate_270();
};

} // namespace pingus::editor

#endif

/* EOF */
