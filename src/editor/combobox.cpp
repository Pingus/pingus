// Pingus - A free Lemmings clone
// Copyright (C) 2005 Ingo Ruhnke <grumbel@gmail.com>
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

#include "editor/combobox.hpp"

#include "engine/display/drawing_context.hpp"
#include "pingus/fonts.hpp"

namespace Editor {

Combobox::Combobox(const Rect& rect_) :
  RectComponent(rect_),
  sprite("core/editor/combobox"),
  list_rect(),
  item_list(),
  current_item(-1),
  hover_item(),
  drop_down(false),
  list_offset(),
  on_select()
{
}

Combobox::~Combobox ()
{
  clear();
}

void
Combobox::add(int id, const std::string& str)
{
  item_list.push_back(ComboItem(id, str));
}

// Remove all items from this combobox
void
Combobox::clear()
{
  item_list.clear();
  current_item = -1;
  drop_down    = false;
}

// Returns whether or not the combobox is at this location
bool
Combobox::is_at(int x, int y)
{
  if (drop_down)
    return Rect(Vector2i(rect.left, rect.top + get_box_offset()),
                Size(rect.get_width(), rect.get_height() * static_cast<int>(item_list.size()))).contains(Vector2i(x,y));
  else
    return RectComponent::is_at(x,y);
}

void
Combobox::on_primary_button_press(int x, int y)
{
  if (drop_down)
  {
    // Determine which item was selected, if any, and set the current item to it.
    drop_down = false;
    ungrab();

    if (hover_item != -1)
    {
      current_item = hover_item;
      on_select(item_list[static_cast<size_t>(current_item)]);
    }
  }
  else
  {
    drop_down = true;
    grab();

    list_rect = Rect(Vector2i(rect.left,
                              rect.top + get_box_offset()),
                     Size(rect.get_width(),
                          rect.get_height() * static_cast<int>(item_list.size())));

    on_pointer_move(x,y);
  }
}

void
Combobox::draw(DrawingContext &gc)
{
  { // draw the unopened box
    gc.draw_fillrect(rect, Color(255,255,255));
    gc.draw(sprite, Vector2i(rect.right - 12, rect.top));
    gc.draw_rect(rect, Color(0,0,0));

    if (current_item != -1)
    {
      gc.print_left(Fonts::verdana11,
                    Vector2i(rect.left + 5,
                             rect.top + rect.get_height()/2 - Fonts::verdana11.get_height()/2),
                    item_list[static_cast<size_t>(current_item)].label);
    }
  }

  if (drop_down && !item_list.empty())
  {
    gc.draw_fillrect(list_rect, Color(255,255,255), 90);

    for (int i = 0; i < int(item_list.size()); ++i)
    {
      if (i == hover_item)
        gc.draw_fillrect(Rect(Vector2i(rect.left, list_rect.top + rect.get_height()*i),
                              Size(rect.get_width(), rect.get_height())),
                         Color(150,200,255), 95);

      gc.print_left(Fonts::verdana11,
                    Vector2i(list_rect.left + 5,
                             list_rect.top + i * rect.get_height() + rect.get_height()/2 - Fonts::verdana11.get_height()/2),
                    item_list[static_cast<size_t>(i)].label, 100);
    }

    gc.draw_rect(list_rect, Color(0,0,0), 100);
  }
}

int
Combobox::get_box_offset()
{
  // open over the current item
  //return -(rect.get_height() * current_item);

  // open to the top
  return -(rect.get_height() * static_cast<int>(item_list.size()));
}

bool
Combobox::set_selected_item(int id)
{
  for(int i = 0; i < static_cast<int>(item_list.size()); ++i)
  {
    if (item_list[static_cast<size_t>(i)].id == id)
    {
      current_item = i;
      return true;
    }
  }
  return false;
}

void
Combobox::on_pointer_move(int x, int y)
{
  if (drop_down)
  {
    if (list_rect.contains(Vector2i(x,y)))
    {
      hover_item = (y - list_rect.top) / rect.get_height();
      hover_item = Math::clamp(0, hover_item, int(item_list.size()-1));
    }
    else
    {
      hover_item = -1;
    }
  }
}

} // namespace Editor

/* EOF */
