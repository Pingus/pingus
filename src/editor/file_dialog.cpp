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

#include <iostream>
#include <boost/bind.hpp>

#include "display/drawing_context.hpp"
#include "gui/gui_manager.hpp"
#include "editor_screen.hpp"
#include "util/system.hpp"
#include "gui_style.hpp"
#include "fonts.hpp"
#include "gettext.h"
#include "label.hpp"
#include "inputbox.hpp"
#include "pathname.hpp"
#include "button.hpp"
#include "file_dialog.hpp"

namespace Editor {

FileDialog::FileDialog(EditorScreen* editor_, const Rect& rect, Mode mode_)
  : GroupComponent(rect),
    editor(editor_),
    mode(mode_)
{
  file_list = new FileList(Rect(4, 30 + 30 + 30,
                                rect.get_width()-4 - 30, rect.get_height() - 4 - 35));
  add(file_list);

  file_list->on_click.connect(boost::bind(&FileDialog::load_file, this, _1));

  Rect file_rect = file_list->get_rect();
  up_button = new Button(Rect(file_rect.right + 2, file_rect.top,
                              rect.get_width()-4, file_rect.top + file_rect.get_height()/2 - 1),
                         "/\\\n|");
  down_button = new Button(Rect(file_rect.right + 2, file_rect.top + file_rect.get_height()/2 + 1,
                                rect.get_width()-4, file_rect.bottom),
                           "|\n\\/");

  datadir_button = new Button(Rect(Vector2i(4, rect.get_height() - 4 - 30),
                                   Size(100, 30)), _("Datadir"));
  userdir_button = new Button(Rect(Vector2i(4 + 110, rect.get_height() - 4 - 30),
                                   Size(100, 30)), _("Userdir"));
  
  open_button = new Button(Rect(Vector2i(rect.get_width() - 104, rect.get_height() - 4 - 30),
                                Size(100, 30)), mode == LOAD ? _("Open") : _("Save"));
  
  cancel_button = new Button(Rect(Vector2i(rect.get_width() - 104 - 104, rect.get_height() - 4 - 30),
                                  Size(100, 30)), _("Cancel"));
  
  up_button->on_click.connect(boost::bind(&FileDialog::on_up, this));
  down_button->on_click.connect(boost::bind(&FileDialog::on_down, this));

  datadir_button->on_click.connect(boost::bind(&FileDialog::on_datadir, this));
  userdir_button->on_click.connect(boost::bind(&FileDialog::on_userdir, this));

  open_button->on_click.connect(boost::bind(&FileDialog::on_open, this));
  cancel_button->on_click.connect(boost::bind(&FileDialog::on_cancel, this));

  add(filename_label = new Label(Rect(6, 4+30, 4 + 60, 26+30), _("Filename:")));
  add(pathname_label = new Label(Rect(6, 4+60, 4 + 60, 26+60), _("Pathname:")));
  
  add(filename_inputbox = new Inputbox(Rect(4 + 60, 4+30, rect.get_width()-4, 26+30)));
  add(pathname_inputbox = new Inputbox(Rect(4 + 60, 4+60, rect.get_width()-4, 26+60)));

  add(up_button);
  add(down_button);

  add(datadir_button);
  add(userdir_button);

  add(open_button);
  add(cancel_button);
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
      //std::cout << "Directory: " << entry.name << std::endl;
      set_directory(pathname_inputbox->get_text() + "/" + entry.name);
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
  file_list->set_directory(pathname);
  update_button_state();

  filename_inputbox->set_text("");
  pathname_inputbox->set_text(pathname);
}

void
FileDialog::on_cancel()
{
  std::cout << "Cancel" << std::endl;
  hide();
}

void
FileDialog::on_open()
{
  if (!filename_inputbox->get_text().empty())
    {
      if (mode == LOAD)
        {
          Pathname file(pathname_inputbox->get_text() + "/" + filename_inputbox->get_text(), Pathname::SYSTEM_PATH);
          std::cout << "Open: " << file << std::endl;
          editor->load(file);
          hide();
        }
      else if (mode == SAVE) 
        {
          Pathname file(pathname_inputbox->get_text() + "/" + filename_inputbox->get_text(), Pathname::SYSTEM_PATH);
          std::cout << "Save: " << file << std::endl;
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
