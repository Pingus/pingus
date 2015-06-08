// Pingus - A free Lemmings clone
// Copyright (C) 2007 Jason Green <jave27@gmail.com>,
//                    Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_EDITOR_PANEL_HPP
#define HEADER_PINGUS_EDITOR_PANEL_HPP

#include "engine/display/sprite.hpp"
#include "engine/gui/component.hpp"
#include "engine/gui/group_component.hpp"

namespace Editor {

class EditorScreen;

class Panel : public GUI::GroupComponent
{
private:
  EditorScreen* editor;
  Sprite logo;
  Vector2i pos;

  typedef void (EditorScreen::*Callback)();
  Callback callback;

public:
  Panel(EditorScreen* e, const Rect& rect_);
  ~Panel();

  void draw_background(DrawingContext& gc);
  void update(float delta);

  void add_button(const std::string& image, const std::string& tooltip = "", Callback callback = 0);
  void add_toggle_button(const std::string& image);
  void add_separator();

private:
  Panel (const Panel&);
  Panel& operator= (const Panel&);
};

} // namespace Editor

#endif

/* EOF */
