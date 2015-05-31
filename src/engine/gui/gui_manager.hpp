// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_ENGINE_GUI_GUI_MANAGER_HPP
#define HEADER_PINGUS_ENGINE_GUI_GUI_MANAGER_HPP

#include "engine/gui/group_component.hpp"
#include "engine/input/event.hpp"

class DrawingContext;
class GameDelta;

namespace GUI {

/** The GUIManager class holds a group of components and manages
    them. It dispatches the GameDelta to each individual
    component

    FIXME: We translate GameDelta into another 'language' which is
    then understood by the GUI, this seems unclear, not sure at
    which point it is best to split the GameDelta into
    on_primary_button_press(), etc.
*/
class GUIManager : public GroupComponent
{
private:
  Vector2i mouse_pos;

public:
  GUIManager();
  GUIManager(const Rect&);
  virtual ~GUIManager ();

  virtual void update(float delta);
  virtual void update(const Input::Event& event);

private:
  GUIManager (const GUIManager&);
  GUIManager& operator= (const GUIManager&);
};

} // namespace GUI

#endif

/* EOF */
