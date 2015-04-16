// Pingus - A free Lemmings clone
// Copyright (C) 2007 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_EDITOR_OBJECT_SELECTOR_HPP
#define HEADER_PINGUS_EDITOR_OBJECT_SELECTOR_HPP

#include <memory>

#include "engine/gui/group_component.hpp"

class DrawingContext;

namespace Editor {

class EditorScreen;
class ObjectSelectorList;
class ObjectSelectorSet;

class ObjectSelector : public GUI::GroupComponent
{
private:
  EditorScreen* editor;
  Vector2i button_pos;
  ObjectSelectorList* object_list;

  std::unique_ptr<ObjectSelectorSet>  gp_ground_set;
  std::unique_ptr<ObjectSelectorSet>  gp_solid_set;
  std::unique_ptr<ObjectSelectorSet>  gp_bridge_set;
  std::unique_ptr<ObjectSelectorSet>  gp_transparent_set;
  std::unique_ptr<ObjectSelectorSet>  gp_remove_set;
  std::unique_ptr<ObjectSelectorSet>  hotspot_set;
  std::unique_ptr<ObjectSelectorSet>  background_set;
  std::unique_ptr<ObjectSelectorSet>  entrance_set;
  std::unique_ptr<ObjectSelectorSet>  exit_set;
  std::unique_ptr<ObjectSelectorSet>  liquid_set;
  std::unique_ptr<ObjectSelectorSet>  trap_set;
  std::unique_ptr<ObjectSelectorSet>  weather_set;
  std::unique_ptr<ObjectSelectorSet>  worldobj_set;
  std::unique_ptr<ObjectSelectorSet>  prefab_set;

public:
  ObjectSelector(EditorScreen* editor, const Rect& rect);
  ~ObjectSelector();

  void draw_background(DrawingContext& gc);
  void add_button(const std::string& image, const std::string& tooltip, ObjectSelectorSet* set);

  void scroll_up();
  void scroll_down();

private:
  std::unique_ptr<ObjectSelectorSet> create_groundpiece(const std::string& prefix, const std::string& type);
  std::unique_ptr<ObjectSelectorSet> create_gp_ground();
  std::unique_ptr<ObjectSelectorSet> create_gp_solid();
  std::unique_ptr<ObjectSelectorSet> create_gp_bridge();
  std::unique_ptr<ObjectSelectorSet> create_gp_transparent();
  std::unique_ptr<ObjectSelectorSet> create_gp_remove();
  std::unique_ptr<ObjectSelectorSet> create_hotspot();
  std::unique_ptr<ObjectSelectorSet> create_background();
  std::unique_ptr<ObjectSelectorSet> create_entrance();
  std::unique_ptr<ObjectSelectorSet> create_exit();
  std::unique_ptr<ObjectSelectorSet> create_liquid();
  std::unique_ptr<ObjectSelectorSet> create_trap();
  std::unique_ptr<ObjectSelectorSet> create_weather();
  std::unique_ptr<ObjectSelectorSet> create_worldobj();
  std::unique_ptr<ObjectSelectorSet> create_prefab();

  void update_layout();
private:
  ObjectSelector (const ObjectSelector&);
  ObjectSelector& operator= (const ObjectSelector&);
};

} // namespace Editor

#endif

/* EOF */
