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
    stored in an ObjectData and described by the shared ObjectSchema, the
    typed accessors of LevelObj map onto the named properties. Accessors
    for properties the object type doesn't have are ignored. */
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

  /** HAS_* flags derived from the object type's properties */
  unsigned attribs;

public:
  /** Create an object of the named type with default properties */
  GenericLevelObj(std::string const& obj_name);
  GenericLevelObj(ObjectData data);

  ObjectData const& get_data() const { return data; }

  Vector2f get_pos() const override { return data.get_pos(); }
  Vector2f get_orig_pos() const override { return orig_pos; }
  unsigned int get_attribs() const override { return attribs; }
  ResDescriptor get_res_desc() const override { return desc; }
  std::string get_section_name() const override { return data.get_name(); }

  std::string get_type() const override { return {}; }
  std::string get_ground_type() const override { return get_or<std::string>("type", {}); }
  int get_speed() const override { return get_or("speed", 0); }
  int get_release_rate() const override { return get_or("release-rate", 0); }
  float get_parallax() const override { return get_or("parallax", 0.0f); }
  int get_owner() const override { return get_or("owner-id", 0); }
  int get_repeat() const override { return get_or("repeat", 0); }
  Color get_color() const override { return get_or("colori", Color(0, 0, 0, 0)); }
  bool get_stretch_x() const override { return get_or("stretch-x", false); }
  bool get_stretch_y() const override { return get_or("stretch-y", false); }
  bool get_keep_aspect() const override { return get_or("keep-aspect", false); }
  float get_scroll_x() const override { return get_or("scroll-x", 0.0f); }
  float get_scroll_y() const override { return get_or("scroll-y", 0.0f); }
  float get_para_x() const override { return get_or("para-x", 0.0f); }
  float get_para_y() const override { return get_or("para-y", 0.0f); }
  std::string get_direction() override { return get_or<std::string>("direction", {}); }
  std::string get_id() const override { return get_or<std::string>("id", {}); }
  std::string get_target_id() const override { return get_or<std::string>("target-id", {}); }
  int get_height() const override { return get_or("height", 0); }
  int get_small_stars() const override { return get_or("small-stars", 0); }
  int get_middle_stars() const override { return get_or("middle-stars", 0); }
  int get_large_stars() const override { return get_or("large-stars", 0); }

  void set_pos(Vector2f const& p) override { data.set_pos(p); }
  float z_index() const override { return data.get_z_index(); }
  void set_z_index(float z_index) override { data.set_z_index(z_index); }
  void set_pos_x(float x) override { data.set_pos(Vector2f(x, data.get_pos().y())); }
  float get_pos_x() const override { return data.get_pos().x(); }
  void set_pos_y(float y) override { data.set_pos(Vector2f(data.get_pos().x(), y)); }
  float get_pos_y() const override { return data.get_pos().y(); }
  void set_orig_pos(Vector2f const& p) override { orig_pos = p; }

  void set_res_desc(ResDescriptor const& d) override;
  void set_modifier(std::string const& m) override;
  void set_modifier(ResourceModifier::Enum modifier) override;
  ResourceModifier::Enum get_modifier() const override;

  /** The section name is defined by the object type */
  void set_section_name(std::string const& /* sn */) override {}
  void set_type(std::string const& /* t */) override {}

  void set_ground_type(std::string const& t) override { set_if("type", t); }
  void set_speed(int s) override { set_if("speed", s); }
  void set_release_rate(int r) override { set_if("release-rate", r); }
  void set_parallax(float para) override { set_if("parallax", para); }
  void set_repeat(int w) override { set_if("repeat", w); }
  void set_owner(int owner) override { set_if("owner-id", owner); }
  void set_scroll_x(float s) override { set_if("scroll-x", s); }
  void set_scroll_y(float s) override { set_if("scroll-y", s); }
  void set_stretch_x(bool s) override { set_if("stretch-x", s); }
  void set_stretch_y(bool s) override { set_if("stretch-y", s); }
  void set_keep_aspect(bool a) override { set_if("keep-aspect", a); }
  void set_color(Color const& c) override { set_if("colori", c); }
  void set_para_x(float p) override { set_if("para-x", p); }
  void set_para_y(float p) override { set_if("para-y", p); }
  void set_direction(std::string const& d) override { set_if("direction", d); }
  void set_id(std::string const& t) override { set_if("id", t); }
  void set_target_id(std::string const& t) override { set_if("target-id", t); }
  void set_height(int h) override { set_if("height", h); }
  void set_small_stars(int n) override { set_if("small-stars", n); }
  void set_middle_stars(int n) override { set_if("middle-stars", n); }
  void set_large_stars(int n) override { set_if("large-stars", n); }

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

  template<typename T>
  void set_if(std::string_view name, T const& value)
  {
    if (data.has(name)) {
      data.set(name, value);
    }
  }

  /** Load the sprite shown for this object */
  void init_sprite();
};

} // namespace pingus::editor

#endif

/* EOF */
