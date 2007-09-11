/*  $Id$
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#ifndef HEADER_ACTION_PROPERTIES_HPP
#define HEADER_ACTION_PROPERTIES_HPP

#include "pingu_enums.hpp"
#include "gui/group_component.hpp"

namespace Editor {

class EditorScreen;

/** */
class ActionProperties : public GUI::GroupComponent
{
private:
  EditorScreen* editor;
  int y_pos;

public:
  ActionProperties(EditorScreen* editor, const Rect& rect);
  ~ActionProperties();

  void draw_background(DrawingContext& gc);
  void update (float delta); 
  
  void add_action(Actions::ActionName id);

  void on_checkbox_change(bool t, Actions::ActionName id);
  void on_inputbox_change(const std::string& str, Actions::ActionName id);
private:
  ActionProperties (const ActionProperties&);
  ActionProperties& operator= (const ActionProperties&);
};

} // namespace Editor

#endif

/* EOF */
