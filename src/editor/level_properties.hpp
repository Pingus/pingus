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

#ifndef HEADER_PINGUS_EDITOR_LEVEL_PROPERTIES_HPP
#define HEADER_PINGUS_EDITOR_LEVEL_PROPERTIES_HPP

#include "engine/gui/group_component.hpp"

namespace pingus::editor {

class EditorScreen;
class EditorLevel;
class Inputbox;

class LevelProperties : public gui::GroupComponent
{
private:
  EditorScreen* editor;
  EditorLevel*  level;

  Inputbox* author;
  Inputbox* levelname;
  Inputbox* description;

  Inputbox* number_of_pingus;
  Inputbox* number_to_save;

  Inputbox* time;
  Inputbox* width;
  Inputbox* height;
  Inputbox* music;

public:
  LevelProperties(EditorScreen* editor, Rect const& rect);
  ~LevelProperties() override;

  void draw_background (DrawingContext& gc) override;
  void update(float delta) override;

  void set_level(EditorLevel* level);

  void on_author_change(std::string const& str);
  void on_levelname_change(std::string const& str);
  void on_description_change(std::string const& str);

  void on_number_to_save_change(std::string const& str);
  void on_number_of_pingus_change(std::string const& str);

  void on_time_change(std::string const& str);
  void on_width_change(std::string const& str);
  void on_height_change(std::string const& str);
  void on_music_change(std::string const& str);

  void update_layout() override;

private:
  LevelProperties (LevelProperties const&);
  LevelProperties& operator= (LevelProperties const&);
};

} // namespace pingus::editor

#endif

/* EOF */
