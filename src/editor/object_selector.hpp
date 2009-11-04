//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_EDITOR_OBJECT_SELECTOR_HPP
#define HEADER_PINGUS_EDITOR_OBJECT_SELECTOR_HPP

#include "gui/component.hpp"
#include "pingus/res_descriptor.hpp"

class DrawingContext;

namespace Editor {

class EditorScreen;
class ObjectSelectorList;
class ObjectSelectorSet;

/** */
class ObjectSelector : public GUI::GroupComponent
{
private:
  EditorScreen* editor;
  Vector2i button_pos;
  ObjectSelectorList* object_list;

  ObjectSelectorSet*  gp_ground_set;
  ObjectSelectorSet*  gp_solid_set;
  ObjectSelectorSet*  gp_bridge_set;
  ObjectSelectorSet*  gp_transparent_set;
  ObjectSelectorSet*  gp_remove_set;
  ObjectSelectorSet*  hotspot_set;
  ObjectSelectorSet*  background_set;
  ObjectSelectorSet*  entrance_set;
  ObjectSelectorSet*  exit_set;
  ObjectSelectorSet*  liquid_set;
  ObjectSelectorSet*  trap_set;
  ObjectSelectorSet*  weather_set;
  ObjectSelectorSet*  worldobj_set;

public:
  typedef void (ObjectSelectorList::*Callback)();
  Callback callback;

  ObjectSelector(EditorScreen* editor, const Rect& rect);
  ~ObjectSelector();

  void draw_background(DrawingContext& gc);
  void add_button(const std::string& image, const std::string& tooltip, ObjectSelectorSet* set);

  ObjectSelectorSet* create_objects(const std::string& prefix);
  ObjectSelectorSet* create_groundpiece(const std::string& prefix, const std::string& type);
  ObjectSelectorSet* create_gp_ground();
  ObjectSelectorSet* create_gp_solid();
  ObjectSelectorSet* create_gp_bridge();
  ObjectSelectorSet* create_gp_transparent();
  ObjectSelectorSet* create_gp_remove();
  ObjectSelectorSet* create_hotspot();
  ObjectSelectorSet* create_background();
  ObjectSelectorSet* create_entrance();
  ObjectSelectorSet* create_exit();
  ObjectSelectorSet* create_liquid();
  ObjectSelectorSet* create_trap();
  ObjectSelectorSet* create_weather();
  ObjectSelectorSet* create_worldobj();

  void update_layout();
private:
  ObjectSelector (const ObjectSelector&);
  ObjectSelector& operator= (const ObjectSelector&);
};

} // namespace Editor

#endif

/* EOF */
