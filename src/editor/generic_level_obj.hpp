// Pingus - A free Lemmings clone
// Copyright (C) 2007-2011 Jason Green <jave27@gmail.com>,
//                         Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_EDITOR_GENERIC_LEVEL_OBJ_HPP
#define HEADER_PINGUS_EDITOR_GENERIC_LEVEL_OBJ_HPP

#include "engine/display/surface.hpp"
#include "editor/level_obj.hpp"
#include "pingus/object_schema.hpp"

namespace pingus::editor {

/** Editor representation of a single level object. The properties are
    stored in an ObjectData and described by the shared ObjectSchema. */
class GenericLevelObj : public LevelObj
{
private:
  ObjectData data;

  /** Sprite used to draw this object, either its "surface" property or
      the schema's editor sprite */
  Sprite sprite;
  Surface surface;
  ResDescriptor desc;

  /** Location of this object before moving it around */
  Vector2f orig_pos;

public:
  /** Create an object of the named type with default properties */
  GenericLevelObj(std::string const& obj_name);
  GenericLevelObj(ObjectData data);

  ObjectData const& get_data() const { return data; }

  ObjectTypeDef const& get_type_def() const override { return data.type(); }
  bool has_property(std::string_view name) const override { return data.has(name); }
  PropertyValue get_property(std::string_view name) const override { return data.get_value(name); }
  void set_property(std::string_view name, PropertyValue const& value) override;

  std::string get_section_name() const override { return data.get_name(); }

  Vector2f get_pos() const override { return data.get_pos(); }
  void set_pos(Vector2f const& p) override { data.set_pos(p); }
  float get_pos_x() const override { return data.get_pos().x(); }
  void set_pos_x(float x) override { data.set_pos(Vector2f(x, data.get_pos().y())); }
  float get_pos_y() const override { return data.get_pos().y(); }
  void set_pos_y(float y) override { data.set_pos(Vector2f(data.get_pos().x(), y)); }

  float z_index() const override { return data.get_z_index(); }
  void set_z_index(float z_index) override { data.set_z_index(z_index); }

  Vector2f get_orig_pos() const override { return orig_pos; }
  void set_orig_pos(Vector2f const& p) override { orig_pos = p; }

  ResDescriptor get_res_desc() const override { return desc; }
  void set_res_desc(ResDescriptor const& d) override;

  ResourceModifier::Enum get_modifier() const override;
  void set_modifier(std::string const& m) override;
  void set_modifier(ResourceModifier::Enum modifier) override;

  void write_properties(Writer& fw) override;
  void refresh_sprite() override;
  void draw(DrawingContext& gc) override;
  void draw_selection(DrawingContext& gc) override;
  bool is_at(int x, int y) override;
  Rect get_rect() const override;

  LevelObjPtr duplicate(Vector2i const& offset) const override;

private:
  template<typename T>
  T get_or(std::string_view name, T const& fallback) const
  {
    return data.has(name) ? data.get<T>(name) : fallback;
  }

  /** Load the sprite shown for this object */
  void init_sprite();

  /** True if the object is drawn with a sprite, either its "surface" or
      the schema's editor sprite */
  bool has_sprite() const;
};

} // namespace pingus::editor

#endif

/* EOF */
