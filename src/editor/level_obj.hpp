// Pingus - A free Lemmings clone
// Copyright (C) 2005 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_EDITOR_LEVEL_OBJ_HPP
#define HEADER_PINGUS_EDITOR_LEVEL_OBJ_HPP

#include "editor/level_obj_ptr.hpp"
#include "engine/display/sprite.hpp"
#include "math/color.hpp"
#include "math/rect.hpp"
#include "math/vector3f.hpp"
#include "pingus/res_descriptor.hpp"
#include "util/writer.hpp"

class DrawingContext;

namespace Editor {

const unsigned HAS_TYPE =         1 << 0;
const unsigned HAS_SPEED =        1 << 1;
const unsigned HAS_PARALLAX =     1 << 2;
const unsigned HAS_REPEAT =       1 << 3;
const unsigned HAS_OWNER =        1 << 4;
const unsigned HAS_COLOR =        1 << 5;
const unsigned HAS_SCROLL =       1 << 6;
const unsigned HAS_PARA =         1 << 7;
const unsigned HAS_STRETCH =      1 << 8;
const unsigned HAS_DIRECTION =    1 << 9;
const unsigned HAS_RELEASE_RATE = 1 << 10;
const unsigned HAS_SPRITE =       1 << 11;
// HAS_SPRITE_FAKE means it has a generic image in the editor, but isn't saved.
const unsigned HAS_SPRITE_FAKE =  1 << 12;
const unsigned CAN_ROTATE =       1 << 13;
const unsigned HAS_GPTYPE =       1 << 14;
const unsigned HAS_STARFIELD =    1 << 15;
const unsigned HAS_ID =           1 << 16;
const unsigned HAS_TARGET_ID =    1 << 17;
const unsigned HAS_HEIGHT =       1 << 18;

class LevelObj
{
private:
  /** Marks if this object has been deleted or not */
  bool removed;

public:
  /** Retrieve the object's position */
  virtual Vector3f get_pos() const = 0;
  virtual Vector3f get_orig_pos() const = 0;

  /** Retrieve this object's attribute number */
  virtual unsigned int get_attribs() const = 0;

  /** Retrieve the object's resource name */
  virtual ResDescriptor get_res_desc() const = 0;

  /** Retrieve the name of the section header for this object */
  virtual std::string get_section_name() const = 0;

  /** Retrieve the object's type */
  virtual std::string get_type() const = 0;

  virtual std::string get_ground_type() const = 0;

  /** Retrieve the object's speed */
  virtual int get_speed() const = 0;

  /** Retrieve the object's release rate (entrances) */
  virtual int get_release_rate() const = 0;

  /** Retrive the object's parallax (is this even used???) */
  virtual float get_parallax() const = 0;

  /** Retrieve the object's owner */
  virtual int get_owner() const = 0;

  /** Retrieve the object's repeat */
  virtual int get_repeat() const = 0;

  /** Retrieve the object's color */
  virtual Color get_color() const = 0;

  /** Returns true if the object is stretched in the x direction */
  virtual bool get_stretch_x() const = 0;

  /** Returns true if the object is stretched in the y direction */
  virtual bool get_stretch_y() const = 0;

  /** Returns true if the object is to maintain it's aspect ratio if stretched */
  virtual bool get_keep_aspect() const = 0;

  /** Retrive the objects scroll value in the x direction */
  virtual float get_scroll_x() const = 0;

  /** Retrive the objects scroll value in the y direction */
  virtual float get_scroll_y() const = 0;

  /** Returns the parallax speed multiplier in the x direction */
  virtual float get_para_x() const = 0;

  /** Returns the parallax speed multiplier in the y direction */
  virtual float get_para_y() const = 0;

  /** Retrieve the object's direction */
  virtual std::string get_direction() = 0;

  virtual std::string get_id() const = 0;
  virtual std::string get_target_id() const = 0;

  virtual int get_height() const = 0;

  /////////////////////////////////////////////////////////
  /// Operations
public:
  LevelObj() :
    removed(false)
  {}

  LevelObj(const LevelObj& rhs) :
    removed(rhs.removed)
  {}

  /** Destructor */
  virtual ~LevelObj() { }

  /** Set the object's position */
  virtual void set_pos(const Vector3f p) = 0;

  virtual void set_pos_x(float x) = 0;
  virtual float get_pos_x() const = 0;

  virtual void set_pos_y(float y) = 0;
  virtual float get_pos_y() const = 0;

  virtual void set_pos_z(float z) = 0;
  virtual float get_pos_z() const = 0;

  /** Original position of the objects before being dragged around */
  virtual void set_orig_pos(const Vector3f p) = 0;

  /** Set the object's resource name */
  virtual void set_res_desc(const ResDescriptor d) = 0;

  /** Set the object's modifier */
  virtual void set_modifier(const std::string m) = 0;

  /** Set the object's modifier */
  virtual void set_modifier(ResourceModifier::Enum modifier) = 0;

  virtual ResourceModifier::Enum get_modifier() const = 0;

  /** Set the object's section header name */
  virtual void set_section_name(const std::string sn) = 0;

  /** Set the object's type */
  virtual void set_type(const std::string t) = 0;

  virtual void set_ground_type(const std::string t) = 0;

  /** Set the object's speed */
  virtual void set_speed(const int s) = 0;

  /** Set the objects release rate */
  virtual void set_release_rate(const int r) = 0;

  /** Set the object's parallax */
  virtual void set_parallax(const float para) = 0;

  /** Set the object's repeat */
  virtual void set_repeat(const int w) = 0;

  /** Set the object's owner_id */
  virtual void set_owner(const int owner) = 0;

  /** Set the object's scroll rate in the x direction */
  virtual void set_scroll_x(const float s) = 0;

  /** Set the object's scroll rate in the y direction */
  virtual void set_scroll_y(const float s) = 0;

  /** Set the objects stretch in the x direction value */
  virtual void set_stretch_x(const bool s) = 0;

  /** Set the objects stretch in the y direction value */
  virtual void set_stretch_y(bool s) = 0;

  /** Set whether or not the object should maintain it's aspect ratio when stretched */
  virtual void set_keep_aspect(const bool a) = 0;

  /** Set the objects color if applicable */
  virtual void set_color(const Color& c) = 0;

  /** Set the object's parallax scroll multiplier in the x direction */
  virtual void set_para_x(const float p) = 0;

  /** Set the object's parallax scroll multiplier in the y direction */
  virtual void set_para_y(const float p) = 0;

  /** Set the object's direction if applicable */
  virtual void set_direction(const std::string d) = 0;

  virtual void set_id(const std::string& t) = 0;
  virtual void set_target_id(const std::string& t) = 0;

  virtual void set_height(const int h) = 0;

  /** Soft delete of the object (needed for Undo action) */
  void remove() { removed = true; }
  bool is_removed() const { return removed; }

  /** Write basic properties to the file for this type */
  virtual void write_properties(Writer &fw) = 0;

  /** Call when the sprite needs to be reloaded */
  virtual void refresh_sprite() = 0;

  /** Draws the sprite with the modifier applied */
  virtual void draw(DrawingContext &gc) = 0;

  /** Draws the sprites selection with the modifier applied */
  virtual void draw_selection(DrawingContext &gc) = 0;

  /** Returns true if the mouse is hovering over this object */
  virtual bool is_at (int x, int y) = 0;

  virtual Rect get_rect() const = 0;

  virtual int get_small_stars() const = 0;
  virtual int get_middle_stars() const = 0;
  virtual int get_large_stars() const  = 0;

  virtual void set_small_stars(int n)  = 0;
  virtual void set_middle_stars(int n) = 0;
  virtual void set_large_stars(int n)  = 0;

  virtual LevelObjPtr duplicate(const Vector2i& offset) const = 0;
};

} // namespace Editor

#endif

/* EOF */
