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

#include "label.hpp"
#include "inputbox.hpp"
#include "gui_style.hpp"
#include "level_properties.hpp"

/*
Level Properties:
Levelname, Description, Author, 
Number of Pingus, Time, Pingus to Save,
Width, Height, (could be handled elsewhere)
Difficulty
Comment
Playable (unneeded)
*/

namespace Editor {

LevelProperties::LevelProperties(EditorScreen* editor, const Rect& rect)
  : GroupComponent(rect)
{
  int w = rect.get_width() - 120;
  add(new Label   (Rect(Vector2i( 10,  10), Size( 80, 20)), "Author:"), true);
  add(new Inputbox(Rect(Vector2i(110,  10), Size(  w, 20))), true);
  add(new Label   (Rect(Vector2i( 10,  32), Size( 80, 20)), "Levelname:"), true);
  add(new Inputbox(Rect(Vector2i(110,  32), Size(  w, 20))), true);
  add(new Label   (Rect(Vector2i( 10,  54), Size( 80, 20)), "Description:"), true);
  add(new Inputbox(Rect(Vector2i(110,  54), Size(  w, 20*3))), true);
  
  int y = 116;
  add(new Label   (Rect(Vector2i( 10,  y), Size( 80, 20)), "Pingus Count:"), true);
  add(new Inputbox(Rect(Vector2i(110,  y), Size(  w, 20))), true);
  add(new Label   (Rect(Vector2i( 10,  y+22), Size( 80, 20)), "Pingus to Save:"), true);
  add(new Inputbox(Rect(Vector2i(110,  y+22), Size(  w, 20))), true);
  add(new Label   (Rect(Vector2i( 10,  y+44), Size( 80, 20)), "Time:"), true);
  add(new Inputbox(Rect(Vector2i(110,  y+44), Size(  w, 20))), true);
  add(new Label   (Rect(Vector2i( 10,  y+66), Size( 80, 20)), "Width:"), true);
  add(new Inputbox(Rect(Vector2i(110,  y+66), Size(  w, 20))), true);
  add(new Label   (Rect(Vector2i( 10,  y+88), Size( 80, 20)), "Height:"), true);
  add(new Inputbox(Rect(Vector2i(110,  y+88), Size(  w, 20))), true);

  add(new Label   (Rect(Vector2i( 10, y+110), Size( 80, 20)), "Difficulty:"), true);
  add(new Inputbox(Rect(Vector2i(110, y+110), Size(  w, 20))), true);
  add(new Label   (Rect(Vector2i( 10, y+132), Size( 80, 20)), "Comment:"), true);
  add(new Inputbox(Rect(Vector2i(110, y+132), Size(  w, 20))), true);
}

LevelProperties::~LevelProperties()
{
}

void
LevelProperties::draw_background(DrawingContext& gc)
{
  GUIStyle::draw_raised_box(gc, Rect(0,0,rect.get_width(), rect.get_height()));
}

void
LevelProperties::update(float delta)
{
}

void
LevelProperties::on_author_change(const std::string& str)
{
}

void
LevelProperties::on_levelname_change(const std::string& str)
{
}

void
LevelProperties::on_description_change(const std::string& str)
{
}

void
LevelProperties::on_width_change(const std::string& str)
{
}

void
LevelProperties::on_height_change(const std::string& str)
{
}

} // namespace Editor

/* EOF */
