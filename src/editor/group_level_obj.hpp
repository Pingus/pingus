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
  /** Prefab overrides set for this group */
  enum Override : unsigned {
    OVERRIDE_REPEAT       = 1 << 0,
    OVERRIDE_OWNER        = 1 << 1,
    OVERRIDE_RELEASE_RATE = 1 << 2,
    OVERRIDE_DIRECTION    = 1 << 3
  };
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
  /** Type describing the prefab overrides a group can have */
  static ObjectTypeDef const& overrides_type();

  ObjectTypeDef const& get_type_def() const override { return overrides_type(); }

  /** True for the prefab overrides the group has */
  bool has_property(std::string_view name) const override;

  PropertyValue get_property(std::string_view name) const override;

  /** Set an override, passed on to all objects of the group */
  void set_property(std::string_view name, PropertyValue const& value) override;

  std::string get_section_name() const override;

  Vector2f get_pos() const override { return m_pos; }
  void set_pos(Vector2f const& p) override;

  float get_pos_x() const override { return 0.0f; }
  void set_pos_x(float /* x */) override { }
  float get_pos_y() const override { return 0.0f; }
  void set_pos_y(float /* y */) override { }

  float z_index() const override { return 0.0f; }
  void set_z_index(float /* z */) override { }

  Vector2f get_orig_pos() const override { return m_orig_pos; }
  void set_orig_pos(Vector2f const& p) override { m_orig_pos = p; }

  ResDescriptor get_res_desc() const override { return ResDescriptor(); }
  void set_res_desc(ResDescriptor const& /* d */) override { }

  ResourceModifier::Enum get_modifier() const override { return ResourceModifier::Enum::ROT0; }
  void set_modifier(std::string const& /* m */) override { }
  void set_modifier(ResourceModifier::Enum /* modifier */) override { }

  void write_properties(Writer &fw) override;
  void refresh_sprite() override { }
  bool is_at (int x, int y) override;
  Rect get_rect() const override;

  LevelObjPtr duplicate(Vector2i const& offset) const override;

private:
  GroupLevelObj(GroupLevelObj const&);
  GroupLevelObj& operator=(GroupLevelObj const&);
};

} // namespace pingus::editor

#endif

/* EOF */
