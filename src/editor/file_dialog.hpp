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

#ifndef HEADER_PINGUS_EDITOR_FILE_DIALOG_HPP
#define HEADER_PINGUS_EDITOR_FILE_DIALOG_HPP

#include "editor/file_list.hpp"
#include "engine/gui/group_component.hpp"

namespace pingus::editor {

class Button;
class EditorScreen;
class Inputbox;
class Label;

class FileDialog : public gui::GroupComponent
{
public:
  enum Mode { LOAD, SAVE };
private:
  EditorScreen* editor;
  Mode mode;

  FileList* file_list;
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
  FileDialog(EditorScreen* editor, Rect const& rect, Mode mode);
  ~FileDialog() override;

  void draw_background(DrawingContext& gc) override;
  void update_layout() override;

  void load_file(System::DirectoryEntry const& entry);
  void set_directory(std::string const& pathname);

private:
  void on_cancel();
  void on_open();

  void on_up();
  void on_down();

  void on_datadir();
  void on_userdir();

  void on_filename_enter();
  void on_pathname_change();

private:
  void update_button_state();

  FileDialog (FileDialog const&);
  FileDialog& operator= (FileDialog const&);
};

} // namespace pingus::editor

#endif

/* EOF */
