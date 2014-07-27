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

#include "editor/button.hpp"
#include "editor/editor_screen.hpp"
#include "editor/gui_style.hpp"
#include "editor/inputbox.hpp"
#include "editor/label.hpp"
#include "pingus/fonts.hpp"
#include "pingus/gettext.h"
#include "util/log.hpp"

namespace Editor {

FileDialog::FileDialog(EditorScreen* editor_, const Rect& rect_, Mode mode_) :
  GroupComponent(rect_),
  editor(editor_),
  mode(mode_),
  file_list(),
  up_button(),
  down_button(),
  open_button(),
  cancel_button(),
  datadir_button(),
  userdir_button(),
  pathname_label(),
  filename_label(),
  pathname_inputbox(),
  filename_inputbox()
{
  file_list = create<FileList>(Rect(4, 30 + 30 + 30,
                                    rect.get_width()-4 - 30, rect.get_height() - 4 - 35));

  file_list->on_click.connect(std::bind(&FileDialog::load_file, this, std::placeholders::_1));

  Rect file_rect = file_list->get_rect();
  up_button = create<Button>(Rect(file_rect.right + 2, file_rect.top,
                              rect.get_width()-4, file_rect.top + file_rect.get_height()/2 - 1),
                         "/\\\n|");
  down_button = create<Button>(Rect(file_rect.right + 2, file_rect.top + file_rect.get_height()/2 + 1,
                                rect.get_width()-4, file_rect.bottom),
                           "|\n\\/");

  datadir_button = create<Button>(Rect(Vector2i(4, rect.get_height() - 4 - 30),
                                       Size(100, 30)), _("Datadir"));
  userdir_button = create<Button>(Rect(Vector2i(4 + 110, rect.get_height() - 4 - 30),
                                       Size(100, 30)), _("Userdir"));
  
  open_button = create<Button>(Rect(Vector2i(rect.get_width() - 104, rect.get_height() - 4 - 30),
                                    Size(100, 30)), mode == LOAD ? _("Open") : _("Save"));
  
  cancel_button = create<Button>(Rect(Vector2i(rect.get_width() - 104 - 104, rect.get_height() - 4 - 30),
                                      Size(100, 30)), _("Cancel"));
  
  up_button->on_click.connect(std::bind(&FileDialog::on_up, this));
  down_button->on_click.connect(std::bind(&FileDialog::on_down, this));

  datadir_button->on_click.connect(std::bind(&FileDialog::on_datadir, this));
  userdir_button->on_click.connect(std::bind(&FileDialog::on_userdir, this));

  open_button->on_click.connect(std::bind(&FileDialog::on_open, this));
  cancel_button->on_click.connect(std::bind(&FileDialog::on_cancel, this));

  filename_label = create<Label>(Rect(6, 4+30, 4 + 60, 26+30), _("Filename:"));
  pathname_label = create<Label>(Rect(6, 4+60, 4 + 60, 26+60), _("Pathname:"));
  
  filename_inputbox = create<Inputbox>(Rect(4 + 60, 4+30, rect.get_width()-4, 26+30));
  pathname_inputbox = create<Inputbox>(Rect(4 + 60, 4+60, rect.get_width()-4, 26+60));
  
  filename_inputbox->on_enter.connect(std::bind(&FileDialog::on_filename_enter, this));
  pathname_inputbox->on_enter.connect(std::bind(&FileDialog::on_pathname_change, this));
}

FileDialog::~FileDialog()
{
}

void
FileDialog::draw_background(DrawingContext& gc)
{
  // Window border and title 
  GUIStyle::draw_raised_box(gc, Rect(0,0,rect.get_width(), rect.get_height()));
  gc.draw_fillrect(Rect(4,4,rect.get_width()-4, 30), Color(77,130,180));
  gc.print_center(Fonts::pingus_small, Vector2i(rect.get_width()/2, 2), 
                  mode == LOAD ? _("Open a level") : _("Save your level"));
}
  
void
FileDialog::load_file(const System::DirectoryEntry& entry)
{ // called when somebody clicks a file

  if (entry.type == System::DE_DIRECTORY)
  {
    //log_info("Directory: " << entry.name);
    set_directory(Pathname::join(pathname_inputbox->get_text(), entry.name));
  }
  else
  {
    filename_inputbox->set_text(entry.name);
  }  
}

void
FileDialog::set_directory(const std::string& pathname_)
{
  std::string pathname = System::realpath(pathname_);

  if (pathname != file_list->get_direction())
  {
    file_list->set_directory(pathname);
    update_button_state();

    filename_inputbox->set_text("");
    if (pathname == "/")
    {
      pathname_inputbox->set_text("/");
    }
    else
    {
      pathname_inputbox->set_text(pathname + "/");
    }
  }
}

void
FileDialog::on_cancel()
{
  log_info("Cancel");
  hide();
}

void
FileDialog::on_open()
{
  if (!filename_inputbox->get_text().empty())
  {
    if (mode == LOAD)
    {
      Pathname file(Pathname::join(pathname_inputbox->get_text(), filename_inputbox->get_text()),
                    Pathname::SYSTEM_PATH);
      log_info("Open: %1%", file);
      editor->load(file);
      hide();
    }
    else if (mode == SAVE) 
    {
      Pathname file(Pathname::join(pathname_inputbox->get_text(), filename_inputbox->get_text()),
                    Pathname::SYSTEM_PATH);
      log_info("Save: %1%", file);
      editor->save(file);
      hide();
    }
  }
}

void
FileDialog::on_up()
{
  file_list->prev_page();
  update_button_state();
}

void
FileDialog::on_down()
{
  file_list->next_page();
  update_button_state();
}

void
FileDialog::update_layout()
{
  GUI::GroupComponent::update_layout();

  file_list->set_rect(Rect(4, 30 + 30 + 30,
                           rect.get_width()-4 - 30, rect.get_height() - 4 - 35));
  
  Rect file_rect = file_list->get_rect();

  up_button->set_rect(Rect(file_rect.right + 2, file_rect.top,
                           rect.get_width()-4, file_rect.top + file_rect.get_height()/2 - 1));
                     
  down_button->set_rect(Rect(file_rect.right + 2, file_rect.top + file_rect.get_height()/2 + 1,
                             rect.get_width()-4, file_rect.bottom));
  
  open_button->set_rect(Rect(Vector2i(rect.get_width() - 104, rect.get_height() - 4 - 30),
                             Size(100, 30)));
  
  cancel_button->set_rect(Rect(Vector2i(rect.get_width() - 104 - 104, rect.get_height() - 4 - 30),
                               Size(100, 30)));

  filename_inputbox->set_rect(Rect(4 + 60, 4+30, rect.get_width()-4, 26+30));
  pathname_inputbox->set_rect(Rect(4 + 60, 4+60, rect.get_width()-4, 26+60));

  datadir_button->set_rect(Rect(Vector2i(4, rect.get_height() - 4 - 30), Size(100, 30)));
  userdir_button->set_rect(Rect(Vector2i(4 + 110, rect.get_height() - 4 - 30), Size(100, 30)));
}

void
FileDialog::on_userdir()
{ 
  set_directory(System::get_userdir() + "levels/");
}

void
FileDialog::on_datadir()
{ 
  set_directory(Pathname("levels/", Pathname::DATA_PATH).get_sys_path());
}

void
FileDialog::on_filename_enter()
{
  on_open();
}

void
FileDialog::on_pathname_change()
{
  Pathname path(pathname_inputbox->get_text(), Pathname::SYSTEM_PATH);
  if (path.exist())
  {
    set_directory(path.get_sys_path());
  }
}

void
FileDialog::update_button_state()
{
  if (file_list->has_more_prev_pages())
    up_button->enable();
  else
    up_button->disable();

  if (file_list->has_more_next_pages())
    down_button->enable();
  else
    down_button->disable();
}
  
} // namespace Editor

/* EOF */
