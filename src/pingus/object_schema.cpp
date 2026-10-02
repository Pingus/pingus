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
  return PropertyDef{"surface", PropertyType::SURFACE, ResDescriptor()};
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
ObjectTypeDef::find_property(std::string_view name) const
{
  auto it = std::find_if(properties.begin(), properties.end(),
                         [name](PropertyDef const& prop) { return prop.name == name; });
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
    {"groundpiece", {}, {prop_string("type", "ground"), prop_surface()},
     0.0f, "", true},

    {"hotspot", {}, {prop_surface(), prop_int("speed", 0), prop_float("parallax", 0.0f)},
     0.0f, "", true},

    {"liquid", {}, {prop_surface(), prop_int("speed", 0), prop_int("repeat", 0, {"width"})},
     0.0f, "", false},

    {"surface-background", {},
     {prop_surface(),
      prop_color("colori"),
      prop_bool("stretch-x", false), prop_bool("stretch-y", false), prop_bool("keep-aspect", false),
      prop_float("scroll-x", 0.0f), prop_float("scroll-y", 0.0f),
      prop_float("para-x", 0.5f), prop_float("para-y", 0.5f)},
     -150.0f, "", false},

    {"solidcolor-background", {}, {prop_color("colori")},
     0.0f, "core/editor/solidcolorbackground", false},

    {"starfield-background", {},
     {prop_int("small-stars", 100), prop_int("middle-stars", 50), prop_int("large-stars", 25)},
     0.0f, "core/editor/starfield", false},

    {"entrance", {},
     {prop_int("owner-id", 0), prop_string("direction", "misc"), prop_int("release-rate", 150)},
     0.0f, "entrances/generic", false},

    {"exit", {}, {prop_surface(), prop_int("owner-id", 0)},
     0.0f, "", false},

    {"spike", {}, {}, 0.0f, "traps/spike_editor", false},
    {"smasher", {}, {}, 0.0f, "traps/smasher", false},
    {"laser_exit", {}, {}, 0.0f, "traps/laser_exit", false},
    {"hammer", {}, {}, 0.0f, "traps/hammer", false},
    {"fake_exit", {}, {}, 0.0f, "traps/fake_exit", false},
    {"guillotine", {}, {}, 0.0f, "traps/guillotineidle", false},

    {"snow-generator", {"snow"}, {prop_float("intensity", 1.0f)},
     0.0f, "core/editor/weather_snow", false},

    {"rain-generator", {"rain"}, {},
     0.0f, "core/editor/weather_rain", false},

    {"teleporter", {}, {prop_string("target-id", "")},
     0.0f, "worldobjs/teleporter", false},

    {"teleporter-target", {}, {prop_string("id", "")},
     0.0f, "worldobjs/teleportertarget", false},

    {"iceblock", {}, {prop_int("repeat", 0, {"width"})},
     0.0f, "worldobjs/iceblock", false},

    {"conveyorbelt", {}, {prop_int("speed", 0), prop_int("repeat", 0, {"width"})},
     0.0f, "worldobjs/conveyorbelt_middle", false},

    {"switchdoor-door", {}, {prop_string("id", ""), prop_int("height", 15)},
     0.0f, "worldobjs/switchdoor_box", false},

    {"switchdoor-switch", {}, {prop_string("target-id", "")},
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
