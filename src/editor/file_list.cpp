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

#include "editor/file_list.hpp"

#include "editor/gui_style.hpp"
#include "util/log.hpp"
#include "pingus/fonts.hpp"

namespace Editor {

FileList::FileList(const Rect& rect_) :
  RectComponent(rect_),
  hspace(),
  vspace(),
  file_icon(),
  directory_icon(),
  m_direction(),
  directory(),
  current_item(-1),
  click_item(-1),
  page(0),
  num_pages(0),
  on_click()
{
  update_layout();
}

void
FileList::update_layout() 
{
  hspace = rect.get_width()/2;
  vspace = 20;

  num_pages = static_cast<int>(directory.size()) / items_per_page();
  if ((directory.size() % items_per_page()) != 0 || num_pages == 0)
    num_pages += 1;
  
  if (page >= num_pages)
    page = num_pages-1;
}

struct DirectorySorter
{
  bool operator()(const System::DirectoryEntry& lhs,
                  const System::DirectoryEntry& rhs)
  {
    if (lhs.type == rhs.type)
    {
      return lhs.name < rhs.name;
    }
    else
    {
      if (lhs.type == System::DE_DIRECTORY)
        return true;
      else
        return false;
    }
  }
};

void
FileList::set_directory(const std::string& pathname, const std::string& pattern)
{
  m_direction = pathname;

  try
  {
    directory = System::opendir(pathname, pattern);
  }
  catch(const std::exception& err)
  {
    log_error("%1%", err.what());
    directory.clear();
  }

  directory.push_back(System::DirectoryEntry("..", System::DE_DIRECTORY));
  std::sort(directory.begin(), directory.end(), DirectorySorter());

  num_pages = static_cast<int>(directory.size()) / items_per_page();
  if ((directory.size() % items_per_page()) != 0 || num_pages == 0)
    num_pages += 1;

  page = 0;
}

void
FileList::draw(DrawingContext& gc)
{
  GUIStyle::draw_lowered_box(gc, rect, Color(255, 255, 255));

  int end = (page+1) * items_per_page();
  if (end > int(directory.size()))
    end = static_cast<int>(directory.size());

  int x = rect.left;
  int y = rect.top;
  for(System::Directory::iterator i = directory.begin() + page * items_per_page();
      i != directory.begin() + end; ++i)
  {
    if (i->type == System::DE_DIRECTORY)
      gc.draw(directory_icon, Vector2i(x, y));
    else if (i->type == System::DE_FILE)
      gc.draw(file_icon, Vector2i(x, y));

    if ((click_item == -1 && (i - directory.begin()) == current_item) ||
        (i - directory.begin()) == click_item)
    {
      if (click_item == current_item)
        gc.draw_fillrect(Rect(x, y, x + hspace, y + vspace), Color(0, 0, 255));
      else
        gc.draw_rect(Rect(x, y, x + hspace, y + vspace), Color(0, 0, 255));
    }
      
    gc.print_left(Fonts::verdana11, Vector2i(x + 4, y + 3),
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
  if (click_item == current_item && current_item != -1)
  {
    on_click(directory[current_item]);
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

  current_item += page * items_per_page();
  
  if (current_item < 0 || current_item >= int(directory.size()))
    current_item = -1;
}

void
FileList::next_page()
{
  page += 1;
  if (page >= num_pages)
    page = num_pages - 1;
}

void
FileList::prev_page()
{
  page -= 1;
  if (page < 0)
    page = 0;
}

void
FileList::update (float delta)
{
}

int
FileList::items_per_page()
{
  return (rect.get_height()/vspace * 2);
}

bool
FileList::has_more_next_pages()
{
  return (page < num_pages-1);
}

bool
FileList::has_more_prev_pages()
{
  return (page > 0);
}

} // namespace Editor

/* EOF */
