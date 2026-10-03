// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pingus/object_schema.hpp"

#include <algorithm>
#include <stdexcept>

namespace pingus {

namespace {

PropertyDef prop_int(std::string name, int value, std::vector<std::string> aliases = {})
{
  return PropertyDef{std::move(name), PropertyType::INT, value, std::move(aliases)};
}

/** Set the editor label of a property */
PropertyDef label(PropertyDef prop, std::string text)
{
  prop.label = std::move(text);
  return prop;
}

/** Hide a property from the editor's property panel */
PropertyDef hidden(PropertyDef prop)
{
  prop.hidden = true;
  return prop;
}

PropertyDef choices(PropertyDef prop, std::vector<PropertyChoice> values)
{
  prop.choices = std::move(values);
  return prop;
}

std::vector<PropertyChoice> const groundtype_choices = {
  {"transparent", "Transparent"},
  {"solid", "Solid"},
  {"ground", "Ground"},
  {"bridge", "Bridge"},
  {"water", "Water"},
  {"lava", "Lava"},
  {"remove", "Remove"},
};

std::vector<PropertyChoice> const direction_choices = {
  {"left", "Left"},
  {"misc", "Misc"},
  {"right", "Right"},
};

PropertyDef prop_float(std::string name, float value)
{
  return PropertyDef{std::move(name), PropertyType::FLOAT, value};
}

PropertyDef prop_bool(std::string name, bool value)
{
  return PropertyDef{std::move(name), PropertyType::BOOL, value};
}

PropertyDef prop_string(std::string name, std::string value)
{
  return PropertyDef{std::move(name), PropertyType::STRING, std::move(value)};
}

PropertyDef prop_color(std::string name)
{
  return PropertyDef{std::move(name), PropertyType::COLOR, Color(0, 0, 0, 0)};
}

PropertyDef prop_surface()
{
  // the surface is picked with the object selector, not the panel
  PropertyDef prop{"surface", PropertyType::SURFACE, ResDescriptor()};
  prop.hidden = true;
  return prop;
}

bool written_before_position(PropertyDef const& prop)
{
  return prop.name == "type" || prop.name == "surface";
}

bool read_property(ReaderMapping const& reader, std::string const& key, PropertyType type, PropertyValue& value)
{
  switch (type)
  {
    case PropertyType::INT: {
      int v;
      if (!reader.read(key, v)) { return false; }
      value = v;
      return true;
    }

    case PropertyType::FLOAT: {
      float v;
      if (!reader.read(key, v)) { return false; }
      value = v;
      return true;
    }

    case PropertyType::BOOL: {
      bool v;
      if (!reader.read(key, v)) { return false; }
      value = v;
      return true;
    }

    case PropertyType::STRING: {
      std::string v;
      if (!reader.read(key, v)) { return false; }
      value = v;
      return true;
    }

    case PropertyType::COLOR: {
      Color v;
      if (!reader.read(key, v)) { return false; }
      value = v;
      return true;
    }

    case PropertyType::SURFACE: {
      ResDescriptor v;
      if (!reader.read(key, v)) { return false; }
      value = v;
      return true;
    }
  }
  return false;
}

void write_property(Writer& writer, PropertyDef const& prop, PropertyValue const& value)
{
  switch (prop.type)
  {
    case PropertyType::INT:
      writer.write(prop.name, std::get<int>(value));
      break;

    case PropertyType::FLOAT:
      writer.write(prop.name, std::get<float>(value));
      break;

    case PropertyType::BOOL:
      writer.write(prop.name, std::get<bool>(value));
      break;

    case PropertyType::STRING:
      writer.write(prop.name, std::get<std::string>(value));
      break;

    case PropertyType::COLOR:
      writer.write(prop.name, std::get<Color>(value));
      break;

    case PropertyType::SURFACE: {
      ResDescriptor const& desc = std::get<ResDescriptor>(value);
      writer.begin_mapping(prop.name);
      writer.write("image", desc.res_name);
      writer.write("modifier", ResourceModifier::to_string(desc.modifier));
      writer.end_mapping();
      break;
    }
  }
}

} // namespace

PropertyDef const*
ObjectTypeDef::find_property(std::string_view prop_name) const
{
  auto it = std::find_if(properties.begin(), properties.end(),
                         [prop_name](PropertyDef const& prop) { return prop.name == prop_name; });
  return it != properties.end() ? &*it : nullptr;
}

ObjectData
ObjectData::from_reader(ObjectTypeDef const& type, std::string_view name, ReaderMapping const& reader)
{
  ObjectData data(type, name);

  InVector2fZ in_vec{data.m_pos, data.m_z_index};
  if (!reader.read("position", in_vec)) {
    data.m_pos = Vector2f(0.0f, 0.0f);
    data.m_z_index = type.default_z_index;
  }

  for (size_t i = 0; i < type.properties.size(); ++i)
  {
    PropertyDef const& prop = type.properties[i];
    if (prop.type == PropertyType::COLOR)
    {
      // "colori" holds 0-255 values, the old "color" 0.0-1.0 floats
      Color color;
      Colorf colorf;
      if (reader.read("colori", color)) {
        data.m_values[i] = color;
      } else if (reader.read("color", colorf)) {
        data.m_values[i] = colorf.to_color();
      }
    }
    else if (!read_property(reader, prop.name, prop.type, data.m_values[i]))
    {
      for (auto const& alias : prop.aliases) {
        if (read_property(reader, alias, prop.type, data.m_values[i])) {
          break;
        }
      }
    }
  }

  return data;
}

ObjectData::ObjectData(ObjectTypeDef const& type, std::string_view name) :
  m_type(&type),
  m_name(name.empty() ? type.name : std::string(name)),
  m_pos(0.0f, 0.0f),
  m_z_index(type.default_z_index),
  m_values()
{
  m_values.reserve(type.properties.size());
  for (auto const& prop : type.properties) {
    m_values.push_back(prop.default_value);
  }
}

bool
ObjectData::has(std::string_view name) const
{
  return m_type->find_property(name) != nullptr;
}

size_t
ObjectData::index_of(std::string_view name) const
{
  auto const& props = m_type->properties;
  for (size_t i = 0; i < props.size(); ++i) {
    if (props[i].name == name) {
      return i;
    }
  }
  throw std::runtime_error("object type '" + m_type->name + "' has no property '" + std::string(name) + "'");
}

void
ObjectData::write(Writer& writer) const
{
  writer.begin_object(m_name);
  write_properties(writer);
  writer.end_object();
}

void
ObjectData::write_properties(Writer& writer) const
{
  auto const& props = m_type->properties;

  for (size_t i = 0; i < props.size(); ++i) {
    if (written_before_position(props[i])) {
      write_property(writer, props[i], m_values[i]);
    }
  }

  writer.write("position", OutVector2fZ{m_pos, m_z_index});

  for (size_t i = 0; i < props.size(); ++i) {
    if (!written_before_position(props[i])) {
      write_property(writer, props[i], m_values[i]);
    }
  }
}

ObjectSchema const&
ObjectSchema::instance()
{
  static ObjectSchema schema;
  return schema;
}

ObjectSchema::ObjectSchema() :
  m_types()
{
  // Defaults are the values the game uses when a property is missing.
  m_types = {
    {"groundpiece", {},
     {choices(label(prop_string("type", "ground"), "GPType:"), groundtype_choices), prop_surface()},
     0.0f, "", true},

    // "speed" and "parallax" are read, but have no effect in the game
    {"hotspot", {},
     {prop_surface(), hidden(prop_int("speed", 0)), hidden(prop_float("parallax", 0.0f))},
     0.0f, "", true},

    {"liquid", {},
     {prop_surface(), hidden(prop_int("speed", 0)), label(prop_int("repeat", 0, {"width"}), "Repeat:")},
     0.0f, "", false},

    {"surface-background", {},
     {prop_surface(),
      label(prop_color("colori"), "Color:"),
      label(prop_bool("stretch-x", false), "Stretch-X:"),
      label(prop_bool("stretch-y", false), "Stretch-Y:"),
      label(prop_bool("keep-aspect", false), "Aspect:"),
      label(prop_float("scroll-x", 0.0f), "Scroll-X:"),
      label(prop_float("scroll-y", 0.0f), "Scroll-Y:"),
      label(prop_float("para-x", 0.5f), "Para-X:"),
      label(prop_float("para-y", 0.5f), "Para-Y:")},
     -150.0f, "", false},

    {"solidcolor-background", {}, {label(prop_color("colori"), "Color:")},
     0.0f, "core/editor/solidcolorbackground", false},

    {"starfield-background", {},
     {label(prop_int("small-stars", 100), "Small Stars:"),
      label(prop_int("middle-stars", 50), "Middle Stars:"),
      label(prop_int("large-stars", 25), "Large Stars:")},
     0.0f, "core/editor/starfield", false},

    {"entrance", {},
     {label(prop_int("owner-id", 0), "Owner Id:"),
      choices(label(prop_string("direction", "misc"), "Direction:"), direction_choices),
      label(prop_int("release-rate", 150), "ReleaseRate:")},
     0.0f, "entrances/generic", false},

    {"exit", {}, {prop_surface(), label(prop_int("owner-id", 0), "Owner Id:")},
     0.0f, "", false},

    {"spike", {}, {}, 0.0f, "traps/spike_editor", false},
    {"smasher", {}, {}, 0.0f, "traps/smasher", false},
    {"laser_exit", {}, {}, 0.0f, "traps/laser_exit", false},
    {"hammer", {}, {}, 0.0f, "traps/hammer", false},
    {"fake_exit", {}, {}, 0.0f, "traps/fake_exit", false},
    {"guillotine", {}, {}, 0.0f, "traps/guillotineidle", false},

    {"snow-generator", {"snow"}, {label(prop_float("intensity", 1.0f), "Intensity:")},
     0.0f, "core/editor/weather_snow", false},

    {"rain-generator", {"rain"}, {},
     0.0f, "core/editor/weather_rain", false},

    {"teleporter", {}, {label(prop_string("target-id", ""), "Target Id:")},
     0.0f, "worldobjs/teleporter", false},

    {"teleporter-target", {}, {label(prop_string("id", ""), "Id:")},
     0.0f, "worldobjs/teleportertarget", false},

    {"iceblock", {}, {label(prop_int("repeat", 0, {"width"}), "Repeat:")},
     0.0f, "worldobjs/iceblock", false},

    {"conveyorbelt", {},
     {label(prop_int("speed", 0), "Speed:"), label(prop_int("repeat", 0, {"width"}), "Repeat:")},
     0.0f, "worldobjs/conveyorbelt_middle", false},

    {"switchdoor-door", {},
     {label(prop_string("id", ""), "Id:"), label(prop_int("height", 15), "Height:")},
     0.0f, "worldobjs/switchdoor_box", false},

    {"switchdoor-switch", {}, {label(prop_string("target-id", ""), "Target Id:")},
     0.0f, "worldobjs/switchdoor_switch", false},
  };
}

ObjectTypeDef const*
ObjectSchema::find(std::string_view name) const
{
  for (auto const& type : m_types)
  {
    if (type.name == name ||
        std::find(type.aliases.begin(), type.aliases.end(), name) != type.aliases.end())
    {
      return &type;
    }
  }
  return nullptr;
}

} // namespace pingus

/* EOF */
