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

namespace Editor {

/** Generic Level Object (groundpiece, sign, etc.)  Only special objects will have
    to inherit from this class - most objects will be able to use this class alone */
class GenericLevelObj : public LevelObj
{
protected:
  /** Sprite used to draw this object */
  Sprite sprite;
  Surface surface;

  /** Resource Desciptor of this object */
  ResDescriptor desc;

  /** Location of this object in the World */
  Vector3f pos;

  /** Location of this object before moving it around */
  Vector3f orig_pos;

  /** Name of the section header for this object (hotspot, groundpiece, etc.) */
  std::string section_name;

  /** Optional type field for certain objects */
  std::string object_type;

  std::string ground_type;

  /** Optional speed field for certain objects */
  int speed;

  /** Optional parallax field for certain objects */
  float parallax;

  /** Optional repeat field for certain objects */
  int repeat;

  /** Optional owner id field for certain objects */
  int owner_id;

  /** Optional release rate field for certain objects (entrances) */
  int release_rate;

  /** Optional direction field for certain objects */
  std::string direction;

  /** Optional scroll field in the x direction for certain objects */
  float scroll_x;

  /** Optional scroll field in the y direction for certain objects */
  float scroll_y;

  /** Optional stretch field in the x direction for certain objects */
  bool stretch_x;

  /** Optional stretch field in the y direction for certain objects */
  bool stretch_y;

  /** Optional field for keeping the aspect ratio of stretched objects */
  bool keep_aspect;

  /** Optional parallax field in the x direction for certain objects */
  float para_x;

  /** Optional parallax field in the y direction for certain objects */
  float para_y;

  /** Optional color field for certain objects */
  Color color;

  /** Optional translation origin of the sprite */
  Origin origin;

  int small_stars;
  int middle_stars;
  int large_stars;

  std::string id;
  std::string target_id;

  int height;

  /** Number representing which attributes this object possesses */
  unsigned attribs;

  /////////////////////////////////////////////////////////
  /// Retrieve info
public:
  /** Retrieve the object's position */
  Vector3f get_pos() const { return pos; }
  Vector3f get_orig_pos() const { return orig_pos; }

  /** Retrieve this object's attribute number */
  unsigned int get_attribs() const { return attribs; }

  /** Retrieve the object's resource name */
  ResDescriptor get_res_desc() const { return desc; }

  /** Retrieve the name of the section header for this object */
  std::string get_section_name() const { return section_name; }

  /** Retrieve the object's type */
  std::string get_type() const { return object_type; }

  std::string get_ground_type() const { return ground_type; }

  /** Retrieve the object's speed */
  int get_speed() const { return speed; }

  /** Retrieve the object's release rate (entrances) */
  int get_release_rate() const { return release_rate; }

  /** Retrive the object's parallax (is this even used???) */
  float get_parallax() const { return parallax; }

  /** Retrieve the object's owner */
  int get_owner() const { return owner_id; }

  /** Retrieve the object's repeat */
  int get_repeat() const { return repeat; }

  /** Retrieve the object's color */
  Color get_color() const { return color; }

  /** Returns true if the object is stretched in the x direction */
  bool get_stretch_x() const { return stretch_x; }

  /** Returns true if the object is stretched in the y direction */
  bool get_stretch_y() const { return stretch_y; }

  /** Returns true if the object is to maintain it's aspect ratio if stretched */
  bool get_keep_aspect() const { return keep_aspect; }

  /** Retrive the objects scroll value in the x direction */
  float get_scroll_x() const { return scroll_x; }

  /** Retrive the objects scroll value in the y direction */
  float get_scroll_y() const { return scroll_y; }

  /** Returns the parallax speed multiplier in the x direction */
  float get_para_x() const { return para_x; }

  /** Returns the parallax speed multiplier in the y direction */
  float get_para_y() const { return para_y; }

  /** Retrieve the object's direction */
  std::string get_direction() { return direction; }

  std::string get_id() const { return id; }
  std::string get_target_id() const { return target_id; }

  int get_height() const { return height; }

  /////////////////////////////////////////////////////////
  /// Operations
public:
  /** Default Constructor */
  GenericLevelObj(const std::string obj_name);
  GenericLevelObj(const GenericLevelObj& rhs);

  /** Destructor */
  virtual ~GenericLevelObj() { }

  /** Set the object's position */
  void set_pos(const Vector3f p);

  void set_pos_x(float x);
  float get_pos_x() const;

  void set_pos_y(float y);
  float get_pos_y() const;

  void set_pos_z(float z);
  float get_pos_z() const;

  /** Original position of the objects before being dragged around */
  void set_orig_pos(const Vector3f p) { orig_pos = p; }

  /** Set the object's resource name */
  void set_res_desc(const ResDescriptor d);

  /** Set the object's modifier */
  void set_modifier(const std::string m);

  /** Set the object's modifier */
  void set_modifier(ResourceModifier::Enum modifier);

  ResourceModifier::Enum get_modifier() const;

  /** Set the object's section header name */
  void set_section_name(const std::string sn) { section_name = sn; }

  /** Set the object's type */
  void set_type(const std::string t) { object_type = t; }

  void set_ground_type(const std::string t) { ground_type = t; }

  /** Set the object's speed */
  void set_speed(const int s) { speed = s; }

  /** Set the objects release rate */
  void set_release_rate(const int r) { release_rate = r; }

  /** Set the object's parallax */
  void set_parallax(const float para) { parallax = para; }

  /** Set the object's repeat */
  void set_repeat(const int w) { repeat = w; }

  /** Set the object's owner_id */
  void set_owner(const int owner) { owner_id = owner; }

  /** Set the object's scroll rate in the x direction */
  void set_scroll_x(const float s) { scroll_x = s; }

  /** Set the object's scroll rate in the y direction */
  void set_scroll_y(const float s) { scroll_y = s; }

  /** Set the objects stretch in the x direction value */
  void set_stretch_x(const bool s);

  /** Set the objects stretch in the y direction value */
  void set_stretch_y(bool s);

  /** Set whether or not the object should maintain it's aspect ratio when stretched */
  void set_keep_aspect(const bool a);

  /** Set the objects color if applicable */
  void set_color(const Color& c)
  { color = c; }

  /** Set the object's parallax scroll multiplier in the x direction */
  void set_para_x(const float p) { para_x = p; }

  /** Set the object's parallax scroll multiplier in the y direction */
  void set_para_y(const float p) { para_y = p; }

  /** Set the object's direction if applicable */
  void set_direction(const std::string d) { direction = d; }

  void set_id(const std::string& t) { id = t; }
  void set_target_id(const std::string& t) { target_id = t; }

  void set_height(const int h) { height = h; }

  /** Write basic properties to the file for this type */
  virtual void write_properties(Writer &fw);

  /** Call when the sprite needs to be reloaded */
  void refresh_sprite();

  /** Draws the sprite with the modifier applied */
  virtual void draw(DrawingContext &gc);

  /** Draws the sprites selection with the modifier applied */
  virtual void draw_selection(DrawingContext &gc);

  /** Returns true if the mouse is hovering over this object */
  virtual bool is_at (int x, int y);

  /** Returns a number representing which attributes this object possesses */
  unsigned int get_attributes(std::string obj_type);

  Rect get_rect() const;

  int get_small_stars() const { return small_stars; }
  int get_middle_stars() const { return middle_stars; }
  int get_large_stars() const { return large_stars; }

  void set_small_stars(int n)  {   small_stars = n; }
  void set_middle_stars(int n) {  middle_stars = n; }
  void set_large_stars(int n)  {   large_stars = n; }

  LevelObjPtr duplicate(const Vector2i& offset) const;

private:
  /** Loads any generic images necessary for objects with HAS_FAKE_SURFACE */
  void load_generic_surface();

  /** Write any additional properties to the file for this type */
  virtual void write_extra_properties(Writer& fw) { }

  /** Sets a position vector of where the sprite is located based
      on the "translation origin" specified in the sprite file. */
  void set_translated_pos();

private:
  GenericLevelObj & operator=(const GenericLevelObj&);
};

} // namespace Editor

#endif

/* EOF */
