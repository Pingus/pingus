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

#include <string_view>

#include "editor/level_obj_ptr.hpp"
#include "engine/display/sprite.hpp"
#include "math/color.hpp"
#include "math/rect.hpp"
#include "math/vector2f.hpp"
#include "pingus/object_schema.hpp"
#include "pingus/res_descriptor.hpp"
#include "util/writer.hpp"
#include "fwd.hpp"

namespace pingus::editor {

/** An object in the edited level: a single level object
    (GenericLevelObj) or a group or prefab (GroupLevelObj). Properties are
    accessed by their level file name as described by the ObjectSchema. */
class LevelObj
{
private:
  /** Marks if this object has been deleted or not */
  bool removed;

public:
  LevelObj() :
    removed(false)
  {}

  LevelObj(LevelObj const& rhs) :
    removed(rhs.removed)
  {}

  virtual ~LevelObj() { }

  /** The type describing the object's properties */
  virtual ObjectTypeDef const& get_type_def() const = 0;

  /** True if the object has the named property */
  virtual bool has_property(std::string_view name) const = 0;

  /** Value of the named property, throws if the object doesn't have it */
  virtual PropertyValue get_property(std::string_view name) const = 0;

  /** Set the named property, ignored if the object doesn't have it */
  virtual void set_property(std::string_view name, PropertyValue const& value) = 0;

  template<typename T>
  T get(std::string_view name) const { return std::get<T>(get_property(name)); }

  template<typename T>
  void set(std::string_view name, T const& value) { set_property(name, PropertyValue(value)); }

  /** Name of the section the object is saved as */
  virtual std::string get_section_name() const = 0;

  virtual Vector2f get_pos() const = 0;
  virtual void set_pos(Vector2f const& p) = 0;

  virtual float get_pos_x() const = 0;
  virtual void set_pos_x(float x) = 0;
  virtual float get_pos_y() const = 0;
  virtual void set_pos_y(float y) = 0;

  virtual float z_index() const = 0;
  virtual void set_z_index(float z_index) = 0;

  /** Position before the object is dragged around */
  virtual Vector2f get_orig_pos() const = 0;
  virtual void set_orig_pos(Vector2f const& p) = 0;

  /** The surface the object is drawn with */
  virtual ResDescriptor get_res_desc() const = 0;
  virtual void set_res_desc(ResDescriptor const& d) = 0;

  virtual ResourceModifier::Enum get_modifier() const = 0;
  virtual void set_modifier(std::string const& m) = 0;
  virtual void set_modifier(ResourceModifier::Enum modifier) = 0;

  /** Mark the object as deleted */
  void remove() { removed = true; }
  bool is_removed() const { return removed; }

  /** Write the object in level file format */
  virtual void write_properties(Writer &fw) = 0;

  /** Call when the sprite needs to be reloaded */
  virtual void refresh_sprite() = 0;

  virtual void draw(DrawingContext &gc) = 0;
  virtual void draw_selection(DrawingContext &gc) = 0;

  /** True if the given position is on the object */
  virtual bool is_at (int x, int y) = 0;

  virtual Rect get_rect() const = 0;

  virtual LevelObjPtr duplicate(Vector2i const& offset) const = 0;
};

} // namespace pingus::editor

#endif

/* EOF */
