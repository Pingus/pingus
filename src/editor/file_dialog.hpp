//  $Id: file_dialog.hpp 3176 2007-09-18 15:23:33Z grumbel $
//
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

#ifndef HEADER_FILE_DIALOG_HPP
#define HEADER_FILE_DIALOG_HPP

#include "file_list.hpp"
#include "gui/group_component.hpp"

namespace Editor {

class Button;
class EditorScreen;
class Inputbox;
class Label;

/** */
class FileDialog : public GUI::GroupComponent
{
public: 
  enum Mode { LOAD, SAVE };
private:
  EditorScreen* editor;
  Mode mode;

  FileList file_list;
  Button* up_button;
  Button* down_button;
  Button* open_button;
  Button* cancel_button;

  Button* datadir_button;
  Button* userdir_button;

  Label* pathname_label;
  Label* filename_label;

  Inputbox* pathname_inputbox;
  Inputbox* filename_inputbox;

public:
  FileDialog(EditorScreen* editor, const Rect& rect, Mode mode);
  ~FileDialog();
  
  void draw_background(DrawingContext& gc);
  void update_layout();

  void load_file(const System::DirectoryEntry& entry);
  void set_directory(const std::string& pathname);

  void on_cancel();
  void on_open();

  void on_up();
  void on_down();
  
  void on_datadir();
  void on_userdir();

private:
  void update_button_state();

  FileDialog (const FileDialog&);
  FileDialog& operator= (const FileDialog&);
};

} // namespace Editor

#endif

/* EOF */
