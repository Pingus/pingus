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

#ifndef HEADER_PINGUS_EDITOR_ACTION_PROPERTIES_HPP
#define HEADER_PINGUS_EDITOR_ACTION_PROPERTIES_HPP

#include <map>

#include "engine/gui/group_component.hpp"
#include "pingus/action_name.hpp"

namespace pingus::editor {

class Checkbox;
class Inputbox;

class EditorScreen;
class EditorLevel;

class ActionProperties : public gui::GroupComponent
{
private:
  EditorScreen* editor;
  EditorLevel*  level;
  int y_pos;

  struct ActionComponent {
    Checkbox* checkbox;
    Inputbox* inputbox;
  };

  typedef std::map<ActionName::Enum, ActionComponent> ActionComponents;
  ActionComponents action_comps;

public:
  ActionProperties(EditorScreen* editor, Rect const& rect);
  ~ActionProperties() override;

  void draw_background(DrawingContext& gc) override;
  void update (float delta) override;

  void add_action(ActionName::Enum id);

  void set_level(EditorLevel* level);

  void on_checkbox_change(bool t, ActionName::Enum id);
  void on_inputbox_change(std::string const& str, ActionName::Enum id);

private:
  ActionProperties (ActionProperties const&);
  ActionProperties& operator= (ActionProperties const&);
};

} // namespace pingus::editor

#endif

/* EOF */
