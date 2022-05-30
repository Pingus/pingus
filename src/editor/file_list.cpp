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

#include "editor/file_list.hpp"

#include <logmich/log.hpp>

#include "editor/gui_style.hpp"
#include "pingus/fonts.hpp"

namespace pingus::editor {

FileList::FileList(Rect const& rect_) :
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
  hspace = rect.width()/2;
  vspace = 20;

  num_pages = static_cast<int>(directory.size()) / items_per_page();
  if ((static_cast<int>(directory.size()) % items_per_page()) != 0 || num_pages == 0)
    num_pages += 1;

  if (page >= num_pages)
    page = num_pages-1;
}

struct DirectorySorter
{
  bool operator()(System::DirectoryEntry const& lhs,
                  System::DirectoryEntry const& rhs)
  {
    if (lhs.type == rhs.type)
    {
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
      return lhs.name < rhs.name; // NOLINT
    }
    else
    {
      return lhs.type == System::DE_DIRECTORY;
    }
  }
};

void
FileList::set_directory(std::string const& pathname, std::string const& pattern)
{
  m_direction = pathname;

  try
  {
    directory = System::opendir(pathname, pattern);
  }
  catch(std::exception const& err)
  {
    log_error("{}", err.what());
    directory.clear();
  }

  directory.push_back(System::DirectoryEntry("..", System::DE_DIRECTORY));
  std::sort(directory.begin(), directory.end(), DirectorySorter());

  num_pages = static_cast<int>(directory.size()) / items_per_page();
  if ((static_cast<int>(directory.size()) % items_per_page()) != 0 || num_pages == 0)
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

  int x = rect.left();
  int y = rect.top();
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

    gc.print_left(pingus::fonts::verdana11, Vector2i(x + 4, y + 3),
                  ((i->type == System::DE_DIRECTORY) ? "[DIR]  " : "[FILE] ") + i->name);

    y += 20;
    if (y > rect.bottom() - vspace)
    {
      y = rect.top();
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
    on_click(directory[static_cast<size_t>(current_item)]);
  }
  click_item = -1;
}

void
FileList::on_pointer_move (int x, int y)
{
  x = x - rect.left();
  y = y - rect.top();

  current_item = std::clamp(y / vspace, 0, rect.height() / vspace - 1) +
    std::clamp(x / hspace, 0, rect.width() / hspace - 1) *
    (rect.height()/vspace);

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
FileList::items_per_page() const
{
  return (rect.height()/vspace * 2);
}

bool
FileList::has_more_next_pages() const
{
  return (page < num_pages-1);
}

bool
FileList::has_more_prev_pages() const
{
  return (page > 0);
}

} // namespace pingus::editor

/* EOF */
