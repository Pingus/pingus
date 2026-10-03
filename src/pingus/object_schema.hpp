// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HEADER_PINGUS_PINGUS_OBJECT_SCHEMA_HPP
#define HEADER_PINGUS_PINGUS_OBJECT_SCHEMA_HPP

#include <stdexcept>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "math/color.hpp"
#include "math/vector2f.hpp"
#include "pingus/res_descriptor.hpp"
#include "util/reader.hpp"
#include "util/writer.hpp"

namespace pingus {

enum class PropertyType
{
  INT,
  FLOAT,
  BOOL,
  STRING,
  /** Read from "colori" (0-255), with "color" (0.0-1.0) as fallback */
  COLOR,
  /** (surface (image "...") (modifier "...")) */
  SURFACE
};

using PropertyValue = std::variant<int, float, bool, std::string, Color, ResDescriptor>;

/** One of the allowed values of a string property */
struct PropertyChoice
{
  std::string value;

  /** Name shown in the editor, translated with _() */
  std::string label;
};

/** One property of a level object as stored in level files */
struct PropertyDef
{
  std::string name;
  PropertyType type;
  PropertyValue default_value;

  /** Older names that are accepted when reading, never written */
  std::vector<std::string> aliases = {};

  /** Label in the editor's property panel, translated with _() */
  std::string label = {};

  /** For STRING properties: the allowed values, shown as a combo box */
  std::vector<PropertyChoice> choices = {};

  /** Not shown in the editor's property panel */
  bool hidden = false;
};

/** Description of one level object type: the properties it has in level
    files and how the editor presents it. Shared by the game, the editor
    and the tools, so that every object type is described exactly once. */
struct ObjectTypeDef
{
  std::string name;

  /** Alternative type names accepted when reading ("snow" for
      "snow-generator") */
  std::vector<std::string> aliases;

  /** Properties in the order they are written. "type" and "surface" are
      written before the position, everything else after it. */
  std::vector<PropertyDef> properties;

  /** z-index used when the object has no position at all */
  float default_z_index = 0.0f;

  /** Sprite shown by the editor for objects that have no "surface" */
  std::string editor_sprite = {};

  /** Whether the editor allows rotating and flipping the surface */
  bool editor_can_rotate = false;

  PropertyDef const* find_property(std::string_view prop_name) const;
};

/** The property values of one level object, read from or written to a
    level file according to its ObjectTypeDef. */
class ObjectData
{
public:
  /** Read the properties of 'type' from 'reader', missing properties get
      their default value. 'name' is the section name used in the file,
      which may be an alias of the type name. */
  static ObjectData from_reader(ObjectTypeDef const& type, std::string_view name, ReaderMapping const& reader);

public:
  /** Create an object with all properties set to their defaults, 'name'
      defaults to the type name */
  explicit ObjectData(ObjectTypeDef const& type, std::string_view name = {});

  ObjectData(ObjectData const&) = default;
  ObjectData(ObjectData&&) = default;
  ObjectData& operator=(ObjectData const&) = default;
  ObjectData& operator=(ObjectData&&) = default;

  ObjectTypeDef const& type() const { return *m_type; }

  /** Section name the object is written with */
  std::string const& get_name() const { return m_name; }

  Vector2f const& get_pos() const { return m_pos; }
  float get_z_index() const { return m_z_index; }
  void set_pos(Vector2f const& pos) { m_pos = pos; }
  void set_z_index(float z_index) { m_z_index = z_index; }

  bool has(std::string_view name) const;

  /** Return the value of the named property, throws when the object type
      has no such property or it has a different type */
  template<typename T>
  T const& get(std::string_view name) const
  {
    return std::get<T>(m_values[index_of(name)]);
  }

  template<typename T>
  void set(std::string_view name, T const& value)
  {
    set_value(name, PropertyValue(value));
  }

  PropertyValue const& get_value(std::string_view name) const
  {
    return m_values[index_of(name)];
  }

  /** Set the named property, throws when the value has the wrong type */
  void set_value(std::string_view name, PropertyValue const& value)
  {
    PropertyValue& slot = m_values[index_of(name)];
    if (slot.index() != value.index()) {
      throw std::runtime_error("ObjectData::set(): type mismatch for '" + std::string(name) + "'");
    }
    slot = value;
  }

  /** Write the object as a complete '(name ...)' section */
  void write(Writer& writer) const;

  /** Write only the properties, without the surrounding section */
  void write_properties(Writer& writer) const;

private:
  size_t index_of(std::string_view name) const;

private:
  ObjectTypeDef const* m_type;
  std::string m_name;
  Vector2f m_pos;
  float m_z_index;
  std::vector<PropertyValue> m_values;
};

/** Registry of all level object types */
class ObjectSchema
{
public:
  static ObjectSchema const& instance();

public:
  ObjectSchema();

  /** Find a type by name or alias, returns nullptr if unknown */
  ObjectTypeDef const* find(std::string_view name) const;

  std::vector<ObjectTypeDef> const& get_types() const { return m_types; }

private:
  std::vector<ObjectTypeDef> m_types;
};

} // namespace pingus

#endif

/* EOF */
