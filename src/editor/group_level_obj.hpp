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

class ReaderMapping;

namespace Editor {

class GroupLevelObj : public LevelObj
{
public:
  static std::shared_ptr<GroupLevelObj> from_prefab(const std::string& name);

private:
  /** unnamed Groups are saved as (group ...) named ones are
      considered prefabs and will be saved as (prefab ...) */
  std::string m_name;

  std::list<LevelObjPtr> m_objects;
  Vector3f m_pos;
  Vector3f m_orig_pos;

  // properties
  unsigned int m_overrides;
  int m_repeat;
  int m_owner_id;
  int m_release_rate;
  std::string m_direction;

public:
  GroupLevelObj();
  ~GroupLevelObj();

  bool is_prefab() const { return !m_name.empty(); }

  void add_child(LevelObjPtr);

  void draw(DrawingContext& gc);
  void draw_selection(DrawingContext &gc);

  std::list<LevelObjPtr>& get_objects() { return m_objects; }

  void set_overrides(const ReaderMapping& reader);

public:
  /** Retrieve the object's position */
  Vector3f get_pos() const { return m_pos; }
  Vector3f get_orig_pos() const { return m_orig_pos; }

  /** Retrieve this object's attribute number */
  unsigned int get_attribs() const { return m_overrides; }

  /** Retrieve the object's resource name */
  ResDescriptor get_res_desc() const { return ResDescriptor(); }

  /** Retrieve the name of the section header for this object */
  std::string get_section_name() const;

  /** Retrieve the object's type */
  std::string get_type() const { return "group"; }

  std::string get_ground_type() const { return ""; }

  /** Retrieve the object's speed */
  int get_speed() const { return 0; }

  /** Retrieve the object's release rate (entrances) */
  int get_release_rate() const { return m_release_rate; }

  /** Retrive the object's parallax (is this even used???) */
  float get_parallax() const { return 0; }

  /** Retrieve the object's owner */
  int get_owner() const { return m_owner_id; }

  /** Retrieve the object's repeat */
  int get_repeat() const { return m_repeat; }

  /** Retrieve the object's color */
  Color get_color() const { return Color(); }

  /** Returns true if the object is stretched in the x direction */
  bool get_stretch_x() const { return false; }

  /** Returns true if the object is stretched in the y direction */
  bool get_stretch_y() const { return false; }

  /** Returns true if the object is to maintain it's aspect ratio if stretched */
  bool get_keep_aspect() const { return false; }

  /** Retrive the objects scroll value in the x direction */
  float get_scroll_x() const { return false; }

  /** Retrive the objects scroll value in the y direction */
  float get_scroll_y() const { return 0; }

  /** Returns the parallax speed multiplier in the x direction */
  float get_para_x() const { return 0; }

  /** Returns the parallax speed multiplier in the y direction */
  float get_para_y() const { return 0; }

  /** Retrieve the object's direction */
  std::string get_direction() { return m_direction; }

  std::string get_id() const { return 0; }
  std::string get_target_id() const { return 0; }

  int get_height() const { return 0; }

  /////////////////////////////////////////////////////////
  /// Operations
public:
  /** Set the object's position */
  void set_pos(const Vector3f p);

  void set_pos_x(float x) { }
  float get_pos_x() const { return 0.0f; }

  void set_pos_y(float y) { }
  float get_pos_y() const { return 0.0f; }

  void set_pos_z(float z) { }
  float get_pos_z() const { return 0.0f; }

  /** Original position of the objects before being dragged around */
  void set_orig_pos(const Vector3f p) { m_orig_pos = p; }

  /** Set the object's resource name */
  void set_res_desc(const ResDescriptor d) { }

  /** Set the object's modifier */
  void set_modifier(const std::string m) { }

  /** Set the object's modifier */
  void set_modifier(ResourceModifier::Enum modifier) { }

  ResourceModifier::Enum get_modifier() const { return ResourceModifier::Enum::ROT0; }

  /** Set the object's section header name */
  void set_section_name(const std::string sn) { }

  /** Set the object's type */
  void set_type(const std::string t) { }

  void set_ground_type(const std::string t) { }

  /** Set the object's speed */
  void set_speed(const int s) { }

  /** Set the objects release rate */
  void set_release_rate(const int r);

  /** Set the object's parallax */
  void set_parallax(const float para) { }

  /** Set the object's repeat */
  void set_repeat(const int w);

  /** Set the object's owner_id */
  void set_owner(const int owner);

  /** Set the object's scroll rate in the x direction */
  void set_scroll_x(const float s) { }

  /** Set the object's scroll rate in the y direction */
  void set_scroll_y(const float s) { }

  /** Set the objects stretch in the x direction value */
  void set_stretch_x(const bool s) { }

  /** Set the objects stretch in the y direction value */
  void set_stretch_y(bool s) { }

  /** Set whether or not the object should maintain it's aspect ratio when stretched */
  void set_keep_aspect(const bool a) { }

  /** Set the objects color if applicable */
  void set_color(const Color& c) { }

  /** Set the object's parallax scroll multiplier in the x direction */
  void set_para_x(const float p) { }

  /** Set the object's parallax scroll multiplier in the y direction */
  void set_para_y(const float p) { }

  /** Set the object's direction if applicable */
  void set_direction(const std::string d);

  void set_id(const std::string& t) { }
  void set_target_id(const std::string& t) { }

  void set_height(const int h) { }

  /** Write basic properties to the file for this type */
  void write_properties(Writer &fw);

  /** Call when the sprite needs to be reloaded */
  void refresh_sprite() { }

  /** Returns true if the mouse is hovering over this object */
  bool is_at (int x, int y);

  /** Returns a number representing which attributes this object possesses */
  unsigned int get_attributes(std::string obj_type) { return 0; }

  Rect get_rect() const;

  int get_small_stars() const { return 0; }
  int get_middle_stars() const { return 0; }
  int get_large_stars() const  { return 0; }

  void set_small_stars(int n)  {}
  void set_middle_stars(int n) {}
  void set_large_stars(int n)  {}

  LevelObjPtr duplicate(const Vector2i& offset) const;

private:
  GroupLevelObj(const GroupLevelObj&);
  GroupLevelObj& operator=(const GroupLevelObj&);
};

} // namespace Editor

#endif

/* EOF */
