// Pingus - A free Lemmings clone
// Copyright (C) 2006 Ingo Ruhnke <grumbel@gmail.com>
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

#include "editor/object_properties.hpp"

#include <algorithm>
#include <functional>

#include <logmich/log.hpp>
#include <strut/from_string.hpp>
#include <strut/to_string.hpp>

#include "editor/button.hpp"
#include "editor/checkbox.hpp"
#include "editor/combobox.hpp"
#include "editor/gui_style.hpp"
#include "editor/inputbox.hpp"
#include "editor/label.hpp"
#include "editor/level_obj.hpp"
#include "pingus/gettext.h"

namespace pingus::editor {

namespace {

Rect const label_rect(10, 0, 80, 20);
Rect const box_rect(80, 0, 190, 20);

std::string label_text(PropertyDef const& prop)
{
  return prop.label.empty() ? prop.name + ":" : _(prop.label);
}

} // namespace

ObjectProperties::ObjectProperties(EditorScreen* editor_, Rect const& rect_) :
  gui::GroupComponent(rect_, false),
  editor(editor_),
  objects(),
  type_label(),
  mesg_label(),
  pos_x_label(),
  pos_x_inputbox(),
  pos_y_label(),
  pos_y_inputbox(),
  pos_z_label(),
  pos_z_inputbox(),
  flip_horizontal_button(),
  flip_vertical_button(),
  rotate_90_button(),
  rotate_270_button(),
  property_widgets(),
  y_pos()
{
  type_label = create<Label>(Rect(geom::ipoint(4, 4), Size(120, 20)), _("Object:"));
  mesg_label = create<Label>(Rect(geom::ipoint(10, 0), Size(180, 20)), _("Nothing selected"));

  pos_x_label = create<Label>(label_rect, _("X-Pos:"));
  pos_x_inputbox = create<Inputbox>(box_rect);
  pos_x_inputbox->on_change.connect(std::bind(&ObjectProperties::on_pos_x_change, this, std::placeholders::_1));

  pos_y_label = create<Label>(label_rect, _("Y-Pos:"));
  pos_y_inputbox = create<Inputbox>(box_rect);
  pos_y_inputbox->on_change.connect(std::bind(&ObjectProperties::on_pos_y_change, this, std::placeholders::_1));

  pos_z_label = create<Label>(label_rect, _("Z-Pos:"));
  pos_z_inputbox = create<Inputbox>(box_rect);
  pos_z_inputbox->on_change.connect(std::bind(&ObjectProperties::on_pos_z_change, this, std::placeholders::_1));

  flip_horizontal_button = create<Button>(Rect(geom::ipoint(15+40*0-3, 0), Size(34, 34)), "|");
  flip_vertical_button   = create<Button>(Rect(geom::ipoint(15+40*1-3, 0), Size(34, 34)), "--");
  rotate_270_button      = create<Button>(Rect(geom::ipoint(15+40*2-3 + 20, 0), Size(34, 34)), "<-.");
  rotate_90_button       = create<Button>(Rect(geom::ipoint(15+40*3-3 + 20, 0), Size(34, 34)), ".->");

  flip_vertical_button->on_click.connect(std::bind(&ObjectProperties::on_flip_vertical, this));
  flip_horizontal_button->on_click.connect(std::bind(&ObjectProperties::on_flip_horizontal, this));
  rotate_90_button->on_click.connect(std::bind(&ObjectProperties::on_rotate_90, this));
  rotate_270_button->on_click.connect(std::bind(&ObjectProperties::on_rotate_270, this));

  set_object(LevelObjPtr());
}

ObjectProperties::~ObjectProperties()
{
}

ObjectProperties::PropertyWidgets
ObjectProperties::create_property_widgets(PropertyDef const& prop)
{
  PropertyWidgets widgets;
  widgets.prop = &prop;
  widgets.label = create<Label>(label_rect, label_text(prop));

  std::string const name = prop.name;

  switch (prop.type)
  {
    case PropertyType::INT:
      widgets.inputbox = create<Inputbox>(box_rect);
      widgets.inputbox->on_change.connect([this, name](std::string const& str) {
        set_property(name, strut::from_string<int>(str));
      });
      break;

    case PropertyType::FLOAT:
      widgets.inputbox = create<Inputbox>(box_rect);
      widgets.inputbox->on_change.connect([this, name](std::string const& str) {
        set_property(name, strut::from_string<float>(str));
      });
      break;

    case PropertyType::BOOL:
      widgets.checkbox = create<Checkbox>(box_rect, _("on"));
      widgets.checkbox->on_change.connect([this, name](bool value) {
        set_property(name, value);
      });
      break;

    case PropertyType::STRING:
      if (prop.choices.empty())
      {
        widgets.inputbox = create<Inputbox>(box_rect);
        widgets.inputbox->on_change.connect([this, name](std::string const& str) {
          set_property(name, str);
        });
      }
      else
      {
        widgets.combobox = create<Combobox>(box_rect);
        for (size_t i = 0; i < prop.choices.size(); ++i) {
          widgets.combobox->add(static_cast<int>(i), _(prop.choices[i].label));
        }
        PropertyDef const* prop_ptr = &prop;
        widgets.combobox->on_select.connect([this, prop_ptr](ComboItem const& item) {
          set_property(prop_ptr->name, prop_ptr->choices[static_cast<size_t>(item.id)].value);
        });
      }
      break;

    case PropertyType::COLOR:
    {
      Size const color_size(box_rect.width() / 4, box_rect.height());
      for (int i = 0; i < 4; ++i)
      {
        Inputbox* inputbox = create<Inputbox>(Rect(geom::ipoint(box_rect.left() + i * color_size.width(), box_rect.top()),
                                                   color_size));
        inputbox->on_change.connect([this, name, i](std::string const& str) {
          uint8_t const component = static_cast<uint8_t>(std::clamp(strut::from_string<int>(str), 0, 255));
          for (auto const& obj : objects)
          {
            if (obj->has_property(name))
            {
              Color color = obj->get<Color>(name);
              uint8_t* channels[] = { &color.r, &color.g, &color.b, &color.a };
              *channels[i] = component;
              obj->set(name, color);
            }
          }
        });
        widgets.color[static_cast<size_t>(i)] = inputbox;
      }
      break;
    }

    case PropertyType::SURFACE:
      // not editable in the panel, the label is hidden too
      break;
  }

  return widgets;
}

std::vector<ObjectProperties::PropertyWidgets>&
ObjectProperties::get_property_widgets(ObjectTypeDef const& type)
{
  auto it = property_widgets.find(&type);
  if (it != property_widgets.end()) {
    return it->second;
  }

  std::vector<PropertyWidgets> widgets;
  for (auto const& prop : type.properties)
  {
    if (!prop.hidden && prop.type != PropertyType::SURFACE) {
      widgets.push_back(create_property_widgets(prop));
    }
  }

  // newly created widgets are visible, hide_all() didn't know them yet
  for (auto& w : widgets)
  {
    w.label->hide();
    for (gui::RectComponent* comp : std::initializer_list<gui::RectComponent*>{w.inputbox, w.checkbox, w.combobox,
                                                                              w.color[0], w.color[1], w.color[2], w.color[3]}) {
      if (comp) { comp->hide(); }
    }
  }

  return property_widgets.emplace(&type, std::move(widgets)).first->second;
}

void
ObjectProperties::show_property(PropertyWidgets& widgets, LevelObj const& data)
{
  PropertyDef const& prop = *widgets.prop;

  switch (prop.type)
  {
    case PropertyType::INT:
      widgets.inputbox->set_text(strut::to_string(data.get<int>(prop.name)));
      place(widgets.label, widgets.inputbox);
      break;

    case PropertyType::FLOAT:
      widgets.inputbox->set_text(strut::to_string(data.get<float>(prop.name)));
      place(widgets.label, widgets.inputbox);
      break;

    case PropertyType::BOOL:
      widgets.checkbox->set_checked(data.get<bool>(prop.name));
      place(widgets.label, widgets.checkbox);
      break;

    case PropertyType::STRING:
    {
      std::string const& value = data.get<std::string>(prop.name);
      if (widgets.combobox)
      {
        auto it = std::find_if(prop.choices.begin(), prop.choices.end(),
                               [&value](PropertyChoice const& choice) { return choice.value == value; });
        if (it == prop.choices.end()) {
          log_error("unknown value for {}: '{}'", prop.name, value);
        } else {
          widgets.combobox->set_selected_item(static_cast<int>(it - prop.choices.begin()));
        }
        place(widgets.label, widgets.combobox);
      }
      else
      {
        widgets.inputbox->set_text(value);
        place(widgets.label, widgets.inputbox);
      }
      break;
    }

    case PropertyType::COLOR:
    {
      Color const color = data.get<Color>(prop.name);
      int const channels[] = { color.r, color.g, color.b, color.a };
      place(widgets.label);
      for (size_t i = 0; i < 4; ++i)
      {
        widgets.color[i]->set_text(strut::to_string(channels[i]));
        place(widgets.color[i]);
      }
      advance();
      break;
    }

    case PropertyType::SURFACE:
      break;
  }
}

void
ObjectProperties::set_object(LevelObjPtr const& obj)
{
  hide_all();

  if (obj)
  {
    // groups only have the prefab overrides they set
    for (auto& widgets : get_property_widgets(obj->get_type_def()))
    {
      if (obj->has_property(widgets.prop->name)) {
        show_property(widgets, *obj);
      }
    }

    // everybody has x-pos, y-pos and z-pos
    pos_x_inputbox->set_text(strut::to_string(obj->get_pos_x()));
    place(pos_x_label, pos_x_inputbox);
    pos_y_inputbox->set_text(strut::to_string(obj->get_pos_y()));
    place(pos_y_label, pos_y_inputbox);
    pos_z_inputbox->set_text(strut::to_string(obj->z_index()));
    place(pos_z_label, pos_z_inputbox);

    if (obj->get_type_def().editor_can_rotate)
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
ObjectProperties::set_property(std::string const& name, PropertyValue const& value)
{
  for (auto const& obj : objects) {
    obj->set_property(name, value);
  }
}

void
ObjectProperties::draw_background(DrawingContext& gc)
{
  GUIStyle::draw_raised_box(gc, Rect(0,0, rect.width(), rect.height()));
}

void
ObjectProperties::update_layout()
{
  GroupComponent::update_layout();
}

void
ObjectProperties::set_objects(Selection const& objs)
{
  objects = objs;

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
ObjectProperties::hide_all()
{
  y_pos = 30;

  mesg_label->hide();

  pos_x_label->hide();
  pos_x_inputbox->hide();
  pos_y_label->hide();
  pos_y_inputbox->hide();
  pos_z_label->hide();
  pos_z_inputbox->hide();

  flip_horizontal_button->hide();
  flip_vertical_button->hide();
  rotate_90_button->hide();
  rotate_270_button->hide();

  for (auto& [type, widgets_list] : property_widgets)
  {
    for (auto& w : widgets_list)
    {
      w.label->hide();
      for (gui::RectComponent* comp : std::initializer_list<gui::RectComponent*>{w.inputbox, w.checkbox, w.combobox,
                                                                                w.color[0], w.color[1], w.color[2], w.color[3]}) {
        if (comp) { comp->hide(); }
      }
    }
  }
}

void
ObjectProperties::advance()
{
  y_pos += 22;
}

void
ObjectProperties::place(gui::RectComponent* comp) // NOLINT
{
  Rect crect = comp->get_rect();
  comp->set_rect(Rect(crect.left(),
                      y_pos,
                      crect.right(),
                      y_pos + crect.height()));
  comp->show();
}

void
ObjectProperties::place(gui::RectComponent* comp1, gui::RectComponent* comp2) // NOLINT
{
  place(comp1);
  place(comp2);
  y_pos += 22;
}

void
ObjectProperties::finalize()
{
  set_rect(Rect(rect.left(), rect.bottom() - y_pos - 10, rect.right(), rect.bottom()));
}

void
ObjectProperties::on_pos_x_change(std::string const& str)
{
  for (auto const& obj : objects) {
    obj->set_pos_x(strut::from_string<float>(str));
  }
}

void
ObjectProperties::on_pos_y_change(std::string const& str)
{
  for (auto const& obj : objects) {
    obj->set_pos_y(strut::from_string<float>(str));
  }
}

void
ObjectProperties::on_pos_z_change(std::string const& str)
{
  for (auto const& obj : objects) {
    obj->set_z_index(strut::from_string<float>(str));
  }
}

void
ObjectProperties::on_flip_horizontal()
{
  for (auto const& obj : objects) {
    obj->set_modifier(ResourceModifier::horizontal_flip(obj->get_modifier()));
  }
}

void
ObjectProperties::on_flip_vertical()
{
  for (auto const& obj : objects) {
    obj->set_modifier(ResourceModifier::vertical_flip(obj->get_modifier()));
  }
}

void
ObjectProperties::on_rotate_90()
{
  for (auto const& obj : objects) {
    obj->set_modifier(ResourceModifier::rotate_90(obj->get_modifier()));
  }
}

void
ObjectProperties::on_rotate_270()
{
  for (auto const& obj : objects) {
    obj->set_modifier(ResourceModifier::rotate_270(obj->get_modifier()));
  }
}

} // namespace pingus::editor

/* EOF */
