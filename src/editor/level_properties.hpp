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

#ifndef HEADER_LEVEL_PROPERTIES_HPP
#define HEADER_LEVEL_PROPERTIES_HPP

#include "gui/group_component.hpp"

namespace Editor {

class EditorScreen;

/** */
class LevelProperties : public GUI::GroupComponent
{
private:
  EditorScreen* editor;

public:
  LevelProperties(EditorScreen* editor, const Rect& rect);
  ~LevelProperties();

  void draw_background (DrawingContext& gc);
  void update (float delta); 

  void on_author_change(const std::string& str);
  void on_levelname_change(const std::string& str);
  void on_description_change(const std::string& str);

  void on_width_change(const std::string& str);
  void on_height_change(const std::string& str);

private:
  LevelProperties (const LevelProperties&);
  LevelProperties& operator= (const LevelProperties&);
};

} // namespace Editor

#endif

/* EOF */
