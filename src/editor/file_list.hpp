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

#ifndef HEADER_PINGUS_EDITOR_FILE_LIST_HPP
#define HEADER_PINGUS_EDITOR_FILE_LIST_HPP

#include <boost/signals2.hpp>

#include "engine/display/sprite.hpp"
#include "engine/gui/rect_component.hpp"
#include "util/system.hpp"

namespace Editor {

class FileList : public GUI::RectComponent
{
private:
  int hspace;
  int vspace;

  Sprite file_icon;
  Sprite directory_icon;
  std::string m_direction;
  System::Directory directory;
  int current_item;
  int click_item;
  int page;
  int num_pages;

public:
  FileList(const Rect& rect);

  void draw (DrawingContext& gc);
  void update (float delta);

  void update_layout();
  std::string get_direction() const { return m_direction; }
  void set_directory(const std::string& pathname, const std::string& pattern = "*");

  void on_pointer_move (int x, int y);

  void on_primary_button_press (int x, int y);
  void on_primary_button_release (int x, int y);

  void next_page();
  void prev_page();

  bool has_more_next_pages();
  bool has_more_prev_pages();

  boost::signals2::signal<void (const System::DirectoryEntry&)> on_click;

private:
  int items_per_page();

  FileList (const FileList&);
  FileList& operator= (const FileList&);
};

} // namespace Editor

#endif

/* EOF */
