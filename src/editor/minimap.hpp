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

#ifndef HEADER_PINGUS_EDITOR_MINIMAP_HPP
#define HEADER_PINGUS_EDITOR_MINIMAP_HPP

#include <memory>

#include "engine/gui/rect_component.hpp"

namespace pingus::editor {

class EditorScreen;

class Minimap : public gui::RectComponent
{
private:
  EditorScreen* editor;
  std::unique_ptr<DrawingContext> drawing_context;
  bool dragging;

public:
  Minimap(EditorScreen*, Rect const& );
  ~Minimap() override;

  void draw (DrawingContext& gc) override;
  void update (float delta) override;

  void on_primary_button_press (int x, int y) override;
  void on_primary_button_release (int x, int y) override;
  void on_pointer_move(int x, int y) override;

  void update_layout() override;

private:
  Minimap(Minimap const&);
  Minimap & operator=(Minimap const&);
};

} // namespace pingus::editor

#endif

/* EOF */
