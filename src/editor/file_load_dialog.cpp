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

#include <iostream>
#include <boost/bind.hpp>
#include "display/drawing_context.hpp"
#include "gui/gui_manager.hpp"
#include "editor_screen.hpp"
#include "gui_style.hpp"
#include "fonts.hpp"
#include "pathname.hpp"
#include "button.hpp"
#include "file_load_dialog.hpp"

namespace Editor {

FileLoadDialog::FileLoadDialog(EditorScreen* editor_, const Rect& rect)
  : GroupComponent(rect),
    editor(editor_),
    file_list(Rect(4, 30 + 30 + 30,
                   rect.get_width()-4 - 30, rect.get_height() - 4 - 35))
{
  add(&file_list, false);
  file_list.on_click.connect(boost::bind(&FileLoadDialog::load_file, this, _1));

  Rect file_rect = file_list.get_rect();
  up_button = new Button(Rect(file_rect.right + 2, file_rect.top,
                                     rect.get_width()-4, file_rect.top + file_rect.get_height()/2 - 1),
                                "/\\\n|");
  down_button = new Button(Rect(file_rect.right + 2, file_rect.top + file_rect.get_height()/2 + 1,
                                rect.get_width()-4, file_rect.bottom),
                           "|\n\\/");

  // FIXME: This could be turned into system specific hotkeys (C:, D:,
  // etc. on windows, Home, '/', Datadir on Linux)
  home_button = new Button(Rect(Vector2i(4, rect.get_height() - 4 - 30),
                                        Size(100, 30)), "Home");
  
  open_button = new Button(Rect(Vector2i(rect.get_width() - 104, rect.get_height() - 4 - 30),
                                        Size(100, 30)), "Open");
  
  cancel_button = new Button(Rect(Vector2i(rect.get_width() - 104 - 104, rect.get_height() - 4 - 30),
                                  Size(100, 30)), "Cancel");
  
  up_button->on_click.connect(boost::bind(&FileLoadDialog::on_up, this));
  down_button->on_click.connect(boost::bind(&FileLoadDialog::on_down, this));
  home_button->on_click.connect(boost::bind(&FileLoadDialog::on_home, this));
  open_button->on_click.connect(boost::bind(&FileLoadDialog::on_open, this));
  cancel_button->on_click.connect(boost::bind(&FileLoadDialog::on_cancel, this));
  
  add(up_button, true);
  add(down_button, true);

  add(home_button, true);

  add(open_button, true);
  add(cancel_button, true);
}

FileLoadDialog::~FileLoadDialog()
{
}

void
FileLoadDialog::draw_background(DrawingContext& gc)
{
  GUIStyle::draw_raised_box(gc, Rect(0,0,rect.get_width(), rect.get_height()));
  gc.draw_fillrect(4,4,rect.get_width()-4, 30, Color(77,130,180));
  gc.print_center(Fonts::pingus_small, rect.get_width()/2, 2, "Open a level");

  GUIStyle::draw_lowered_box(gc, Rect(4 + 60,4+30,rect.get_width()-4, 26+30),
                             Color(255,255,255));

  gc.print_left(Fonts::verdana11, 10, 8+30, "File: ");
  gc.print_left(Fonts::verdana11, 10 + 60, 8+30, filename);

  GUIStyle::draw_lowered_box(gc, Rect(4 + 60,4+60,rect.get_width()-4, 26+60),
                             Color(255,255,255));
  gc.print_left(Fonts::verdana11, 10, 8+60, "Path: ");
  gc.print_left(Fonts::verdana11, 10 + 60, 8+60, pathname);
}
  
void
FileLoadDialog::load_file(const System::DirectoryEntry& entry)
{
  if (entry.type == System::DE_DIRECTORY)
    {
      //std::cout << "Directory: " << entry.name << std::endl;
      set_directory(pathname + "/" + entry.name);
    }
  else
    {
      //std::cout << pathname + "/" + entry.name << std::endl;
      filename = entry.name;
    }
}

void
FileLoadDialog::set_directory(const std::string& pathname_)
{
  filename = "";
  pathname = System::realpath(pathname_);
  file_list.set_directory(pathname);
  update_button_state();
}

void
FileLoadDialog::on_cancel()
{
  std::cout << "Cancel" << std::endl;
  hide();
}

void
FileLoadDialog::on_open()
{
  if (!filename.empty())
    {
      Pathname file(pathname + "/" + filename, Pathname::SYSTEM_PATH);
      std::cout << "Open: " << file << std::endl;
      editor->load(file);
      hide();
    }
}

void
FileLoadDialog::on_up()
{
  file_list.prev_page();
  update_button_state();
}

void
FileLoadDialog::on_down()
{
  file_list.next_page();
  update_button_state();
}

void
FileLoadDialog::update_layout()
{
  GUI::GroupComponent::update_layout();

  file_list.set_rect(Rect(4, 30 + 30 + 30,
                          rect.get_width()-4 - 30, rect.get_height() - 4 - 35));
  
  Rect file_rect = file_list.get_rect();

  up_button->set_rect(Rect(file_rect.right + 2, file_rect.top,
                          rect.get_width()-4, file_rect.top + file_rect.get_height()/2 - 1));
                     
  down_button->set_rect(Rect(file_rect.right + 2, file_rect.top + file_rect.get_height()/2 + 1,
                             rect.get_width()-4, file_rect.bottom));
  
  open_button->set_rect(Rect(Vector2i(rect.get_width() - 104, rect.get_height() - 4 - 30),
                             Size(100, 30)));
  
  cancel_button->set_rect(Rect(Vector2i(rect.get_width() - 104 - 104, rect.get_height() - 4 - 30),
                               Size(100, 30)));
}

void
FileLoadDialog::on_home()
{
  
}

void
FileLoadDialog::update_button_state()
{
  if (file_list.has_more_prev_pages())
    up_button->enable();
  else
    up_button->disable();

  if (file_list.has_more_next_pages())
    down_button->enable();
  else
    down_button->disable();
}
  
} // namespace Editor

/* EOF */
