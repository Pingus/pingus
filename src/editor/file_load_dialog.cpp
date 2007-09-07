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
#include "file_load_dialog.hpp"

namespace Editor {

FileLoadDialog::FileLoadDialog(EditorScreen* editor_, const Rect& rect)
  : GroupComponent(rect),
    editor(editor_),
    file_list(Rect(10, 10,
                   rect.get_width()-10, rect.get_height() - 10))
{
  add(&file_list, false);
  
  file_list.on_click.connect(boost::bind(&FileLoadDialog::load_file, this, _1));
}

FileLoadDialog::~FileLoadDialog()
{
}

void
FileLoadDialog::draw_background(DrawingContext& gc)
{
  gc.draw_fillrect(0, 0, rect.get_width(), rect.get_height(), Color(255, 255, 255));
}
  
void
FileLoadDialog::load_file(const std::string& file) const
{
  std::cout << "FileLoadDialog::load_file: " << file << std::endl;
  
}

void
FileLoadDialog::set_directory(const std::string& pathname_)
{
  pathname = pathname_;
  file_list.set_directory(pathname);
}

} // namespace Editor

/* EOF */
