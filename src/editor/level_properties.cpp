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

#include "editor/level_properties.hpp"

#include "editor/editor_level.hpp"
#include "editor/editor_screen.hpp"
#include "editor/gui_style.hpp"
#include "editor/inputbox.hpp"
#include "editor/label.hpp"
#include "editor/viewport.hpp"
#include "util/log.hpp"
#include "util/string_util.hpp"
#include "pingus/gettext.h"

/*
  Level Properties:
  Levelname, Description, Author, 
  Number of Pingus, Time, Pingus to Save,
  Width, Height, (could be handled elsewhere)
  Playable (unneeded)
*/

namespace Editor {

LevelProperties::LevelProperties(EditorScreen* editor_, const Rect& rect_) :
  GroupComponent(rect_),
  editor(editor_),
  level(0),
  author(),
  levelname(),
  description(),
  number_of_pingus(),
  number_to_save(),
  time(),
  width(),
  height(),
  music()
{
  create<Label>(Rect(Vector2i( 10,  10), Size( 80, 20)), _("Author:"));
  author = create<Inputbox>();

  create<Label>(Rect(Vector2i( 10,  32), Size( 80, 20)), _("Levelname:"));
  levelname = create<Inputbox>();

  create<Label>(Rect(Vector2i( 10,  54), Size( 80, 20)), _("Description:"));
  description = create<Inputbox>();

  int y = 116;
  create<Label>(Rect(Vector2i( 10,  y), Size( 80, 20)), _("Pingus Count:"));
  number_of_pingus = create<Inputbox>();

  create<Label>(Rect(Vector2i( 10,  y+22), Size( 80, 20)), _("Pingus to Save:"));
  number_to_save = create<Inputbox>();

  create<Label>(Rect(Vector2i( 10,  y+44), Size( 80, 20)), _("Time:"));
  time = create<Inputbox>();

  create<Label>(Rect(Vector2i( 10,  y+66), Size( 80, 20)), _("Width:"));
  width = create<Inputbox>();

  create<Label>(Rect(Vector2i( 10,  y+88), Size( 80, 20)), _("Height:"));
  height = create<Inputbox>();

  create<Label>(Rect(Vector2i( 10, y+110), Size( 80, 20)), _("Music:"));
  music = create<Inputbox>();

  author->on_change.connect(std::bind(&LevelProperties::on_author_change, this, std::placeholders::_1));
  levelname->on_change.connect(std::bind(&LevelProperties::on_levelname_change, this, std::placeholders::_1));
  description->on_change.connect(std::bind(&LevelProperties::on_description_change, this, std::placeholders::_1));

  number_to_save->on_change.connect(std::bind(&LevelProperties::on_number_to_save_change, this, std::placeholders::_1));
  number_of_pingus->on_change.connect(std::bind(&LevelProperties::on_number_of_pingus_change, this, std::placeholders::_1));

  width->on_change.connect(std::bind(&LevelProperties::on_width_change, this, std::placeholders::_1));
  height->on_change.connect(std::bind(&LevelProperties::on_height_change, this, std::placeholders::_1));
  time->on_change.connect(std::bind(&LevelProperties::on_time_change, this, std::placeholders::_1));
  music->on_change.connect(std::bind(&LevelProperties::on_music_change, this, std::placeholders::_1));
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
LevelProperties::set_level(EditorLevel* level_)
{
  level = level_;
  description->set_text(level->get_description());
  levelname->set_text(level->get_levelname());
  author->set_text(level->get_author());

  number_of_pingus->set_text(StringUtil::to_string(level->get_number_of_pingus()));
  number_to_save->set_text(StringUtil::to_string(level->get_number_to_save()));

  time->set_text(StringUtil::to_string(level->get_time()));
  width->set_text(StringUtil::to_string(level->get_size().width));
  height->set_text(StringUtil::to_string(level->get_size().height));
  music->set_text(level->get_music());
}

void
LevelProperties::on_author_change(const std::string& str)
{
  level->set_author(str);
}

void
LevelProperties::on_levelname_change(const std::string& str)
{
  level->set_levelname(str);
}

void
LevelProperties::on_description_change(const std::string& str)
{
  level->set_description(str);
}

void
LevelProperties::on_width_change(const std::string& str)
{
  Size s = level->get_size();
  level->set_size(Size(StringUtil::to<int>(str), s.height));
  editor->get_viewport()->refresh();
}

void
LevelProperties::on_height_change(const std::string& str)
{
  Size s = level->get_size();
  level->set_size(Size(s.width, StringUtil::to<int>(str)));
  editor->get_viewport()->refresh();
}

void
LevelProperties::on_number_to_save_change(const std::string& str)
{
  int num = 0;
  if (StringUtil::from_string(str, num))
  {
    level->set_number_to_save(num);
  }
  else
  {
    log_error("LevelProperties::on_number_to_save_change: '%1%' not an integer", str);
  }
  
}

void
LevelProperties::on_number_of_pingus_change(const std::string& str)
{
  int num = 0;
  if (StringUtil::from_string(str, num))
  {
    level->set_number_of_pingus(num);
  }
  else
  {
    log_error("LevelProperties::on_number_of_pingus_change: '%1%' not an integer", str);
  }
}

void
LevelProperties::on_time_change(const std::string& str)
{
  level->set_time(StringUtil::to<int>(str));
}

void
LevelProperties::on_music_change(const std::string& str)
{ 
  level->set_music(str);
}

void
LevelProperties::update_layout()
{
  GroupComponent::update_layout();

  int w = rect.get_width() - 120; 
  author->set_rect(Rect(Vector2i(110,  10), Size(  w, 20)));
  levelname->set_rect(Rect(Vector2i(110,  32), Size(  w, 20)));
  description->set_rect(Rect(Vector2i(110,  54), Size(  w, 20*3)));
  
  int y = 116;
  number_of_pingus->set_rect(Rect(Vector2i(110,  y), Size(  w, 20)));
  number_to_save->set_rect(Rect(Vector2i(110,  y+22), Size(  w, 20))); 

  time->set_rect(Rect(Vector2i(110,  y+44), Size(  w, 20)));
  width->set_rect(Rect(Vector2i(110,  y+66), Size(  w, 20)));
  height->set_rect(Rect(Vector2i(110,  y+88), Size(  w, 20)));
  
  music->set_rect(Rect(Vector2i(110, y+110), Size(  w, 20)));
}

} // namespace Editor

/* EOF */
