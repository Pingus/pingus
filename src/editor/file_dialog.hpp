//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_FILE_DIALOG_HPP
#define HEADER_FILE_DIALOG_HPP

#include "file_list.hpp"
#include "gui/group_component.hpp"

namespace Editor {

class Button;
class EditorScreen;

/** */
class FileDialog : public GUI::GroupComponent
{
private:
  EditorScreen* editor;
  FileList file_list;
  Button* up_button;
  Button* down_button;
  Button* open_button;
  Button* cancel_button;

  Button* home_button;

  std::string pathname;
  std::string filename;

public:
  FileDialog(EditorScreen* editor, const Rect& rect);
  ~FileDialog();
  
  void draw_background(DrawingContext& gc);
  void update_layout();

  void load_file(const System::DirectoryEntry& entry);
  void set_directory(const std::string& pathname);

  void on_cancel();
  void on_open();

  void on_up();
  void on_down();
  
  void on_home();

private:
  void update_button_state();

  FileDialog (const FileDialog&);
  FileDialog& operator= (const FileDialog&);
};

} // namespace Editor

#endif

/* EOF */
