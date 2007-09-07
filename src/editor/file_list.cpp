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

#include <iostream>
#include "math.hpp"
#include "fonts.hpp"
#include "file_list.hpp"

namespace Editor {

FileList::FileList(const Rect& rect)
  : RectComponent(rect),
    current_item(-1),
    click_item(-1)
{
  update_layout();
}

void
FileList::update_layout() 
{
  hspace = rect.get_width()/2;
  vspace = 20;
}

void
FileList::set_directory(const std::string& pathname, const std::string& pattern)
{
  directory = System::opendir(pathname, pattern);
}

void
FileList::draw(DrawingContext& gc)
{
  gc.draw_fillrect(rect.left, rect.top, rect.right, rect.bottom,
                   Color(0, 0, 0));

  int x = rect.left;
  int y = rect.top;
  for(System::Directory::iterator i = directory.begin(); i != directory.end(); ++i)
    {
      if (i->type == System::DE_DIRECTORY)
        gc.draw(directory_icon, x, y);
      else if (i->type == System::DE_FILE)
        gc.draw(file_icon, x, y);

      if ((click_item == -1 && (i - directory.begin()) == current_item) ||
          (i - directory.begin()) == click_item)
        {
          if (click_item == current_item)
            gc.draw_fillrect(x, y, x + hspace, y + vspace, Color(0, 0, 255));
          else
            gc.draw_rect(x, y, x + hspace, y + vspace, Color(0, 0, 255));
        }
      
      gc.print_left(Fonts::courier_small, x + 4, y + 3,
                    ((i->type == System::DE_DIRECTORY) ? "[DIR]  " : "[FILE] ") + i->name);

      y += 20;
      if (y > rect.bottom - vspace)
        {
          y = rect.top;
          x += hspace;
        }
    }
}

void
FileList::on_primary_button_press (int x, int y)
{
  on_pointer_move(x,y);
  click_item = current_item;
}

void
FileList::on_primary_button_release (int x, int y)
{
  on_pointer_move(x,y);
  if (click_item == current_item)
    {
      std::cout << directory[current_item].name << std::endl;
      on_click(directory[current_item].name);
    }
  click_item = -1;
}

void
FileList::on_pointer_move (int x, int y)
{
  x = x - rect.left;
  y = y - rect.top;

  current_item = Math::clamp(0, y / vspace, rect.get_height() / vspace - 1)
    + Math::clamp(0, x / hspace, rect.get_width() / hspace - 1) * (rect.get_height()/vspace);
  
  if (current_item < 0 || current_item >= int(directory.size()))
    current_item = -1;
}

void
FileList::update (float delta)
{
}

} // namespace Editor

/* EOF */
