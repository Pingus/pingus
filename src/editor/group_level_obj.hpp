// Pingus - A free Lemmings clone
// Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_EDITOR_GROUP_LEVEL_OBJ_HPP
#define HEADER_PINGUS_EDITOR_GROUP_LEVEL_OBJ_HPP

#include "editor/level_obj.hpp"

#include <list>

#include "util/reader.hpp"

namespace pingus::editor {

class GroupLevelObj : public LevelObj
{
public:
  static std::shared_ptr<GroupLevelObj> from_prefab(std::string const& name);

private:
  /** unnamed Groups are saved as (group ...) named ones are
      considered prefabs and will be saved as (prefab ...) */
  std::string m_name;

  std::list<LevelObjPtr> m_objects;
  Vector2f m_pos;
  Vector2f m_orig_pos;

  // properties
  unsigned int m_overrides;
  int m_repeat;
  int m_owner_id;
  int m_release_rate;
  std::string m_direction;

public:
  GroupLevelObj();
  ~GroupLevelObj() override;

  bool is_prefab() const { return !m_name.empty(); }

  void add_child(LevelObjPtr const&);

  void draw(DrawingContext& gc) override;
  void draw_selection(DrawingContext &gc) override;

  std::list<LevelObjPtr>& get_objects() { return m_objects; }

  void set_overrides(ReaderMapping const& reader);

public:
  /** Retrieve the object's position */
  Vector2f get_pos() const override { return m_pos; }
  Vector2f get_orig_pos() const override { return m_orig_pos; }

  /** Retrieve this object's attribute number */
  unsigned int get_attribs() const override { return m_overrides; }

  /** Retrieve the object's resource name */
  ResDescriptor get_res_desc() const override { return ResDescriptor(); }

  /** Retrieve the name of the section header for this object */
  std::string get_section_name() const override;

  /** Retrieve the object's type */
  std::string get_type() const override { return "group"; }

  std::string get_ground_type() const override { return ""; }

  /** Retrieve the object's speed */
  int get_speed() const override { return 0; }

  /** Retrieve the object's release rate (entrances) */
  int get_release_rate() const override { return m_release_rate; }

  /** Retrive the object's parallax (is this even used???) */
  float get_parallax() const override { return 0; }

  /** Retrieve the object's owner */
  int get_owner() const override { return m_owner_id; }

  /** Retrieve the object's repeat */
  int get_repeat() const override { return m_repeat; }

  /** Retrieve the object's color */
  Color get_color() const override { return Color(); }

  /** Returns true if the object is stretched in the x direction */
  bool get_stretch_x() const override { return false; }

  /** Returns true if the object is stretched in the y direction */
  bool get_stretch_y() const override { return false; }

  /** Returns true if the object is to maintain it's aspect ratio if stretched */
  bool get_keep_aspect() const override { return false; }

  /** Retrive the objects scroll value in the x direction */
  float get_scroll_x() const override { return false; }

  /** Retrive the objects scroll value in the y direction */
  float get_scroll_y() const override { return 0; }

  /** Returns the parallax speed multiplier in the x direction */
  float get_para_x() const override { return 0; }

  /** Returns the parallax speed multiplier in the y direction */
  float get_para_y() const override { return 0; }

  /** Retrieve the object's direction */
  std::string get_direction() override { return m_direction; }

  std::string get_id() const override { return {}; }
  std::string get_target_id() const override { return {}; }

  int get_height() const override { return 0; }

  /////////////////////////////////////////////////////////
  /// Operations
public:
  /** Set the object's position */
  void set_pos(Vector2f const& p) override;

  void set_pos_x(float x) override { }
  float get_pos_x() const override { return 0.0f; }

  void set_pos_y(float y) override { }
  float get_pos_y() const override { return 0.0f; }

  void set_z_index(float z) override { }
  float z_index() const override { return 0.0f; }

  /** Original position of the objects before being dragged around */
  void set_orig_pos(Vector2f const& p) override { m_orig_pos = p; }

  /** Set the object's resource name */
  void set_res_desc(ResDescriptor const& d) override { }

  /** Set the object's modifier */
  void set_modifier(std::string const& m) override { }

  /** Set the object's modifier */
  void set_modifier(ResourceModifier::Enum modifier) override { }

  ResourceModifier::Enum get_modifier() const override { return ResourceModifier::Enum::ROT0; }

  /** Set the object's section header name */
  void set_section_name(std::string const& sn) override { }

  /** Set the object's type */
  void set_type(std::string const& t) override { }

  void set_ground_type(std::string const& t) override { }

  /** Set the object's speed */
  void set_speed(int s) override { }

  /** Set the objects release rate */
  void set_release_rate(int r) override;

  /** Set the object's parallax */
  void set_parallax(float para) override { }

  /** Set the object's repeat */
  void set_repeat(int w) override;

  /** Set the object's owner_id */
  void set_owner(int owner) override;

  /** Set the object's scroll rate in the x direction */
  void set_scroll_x(float s) override { }

  /** Set the object's scroll rate in the y direction */
  void set_scroll_y(float s) override { }

  /** Set the objects stretch in the x direction value */
  void set_stretch_x(const bool s) override { }

  /** Set the objects stretch in the y direction value */
  void set_stretch_y(bool s) override { }

  /** Set whether or not the object should maintain it's aspect ratio when stretched */
  void set_keep_aspect(const bool a) override { }

  /** Set the objects color if applicable */
  void set_color(Color const& c) override { }

  /** Set the object's parallax scroll multiplier in the x direction */
  void set_para_x(float p) override { }

  /** Set the object's parallax scroll multiplier in the y direction */
  void set_para_y(float p) override { }

  /** Set the object's direction if applicable */
  void set_direction(std::string const& d) override;

  void set_id(std::string const& t) override { }
  void set_target_id(std::string const& t) override { }

  void set_height(int h) override { }

  /** Write basic properties to the file for this type */
  void write_properties(Writer &fw) override;

  /** Call when the sprite needs to be reloaded */
  void refresh_sprite() override { }

  /** Returns true if the mouse is hovering over this object */
  bool is_at (int x, int y) override;

  /** Returns a number representing which attributes this object possesses */
  unsigned int get_attributes(std::string const& obj_type) { return 0; }

  Rect get_rect() const override;

  int get_small_stars() const override { return 0; }
  int get_middle_stars() const override { return 0; }
  int get_large_stars() const override  { return 0; }

  void set_small_stars(int n) override  {}
  void set_middle_stars(int n) override {}
  void set_large_stars(int n) override  {}

  LevelObjPtr duplicate(Vector2i const& offset) const override;

private:
  GroupLevelObj(GroupLevelObj const&);
  GroupLevelObj& operator=(GroupLevelObj const&);
};

} // namespace pingus::editor

#endif

/* EOF */
