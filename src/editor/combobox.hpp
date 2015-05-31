// Pingus - A free Lemmings clone
// Copyright (C) 2007 Jason Green <jave27@gmail.com>,
//                    Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_EDITOR_COMBOBOX_HPP
#define HEADER_PINGUS_EDITOR_COMBOBOX_HPP

#include <boost/signals2.hpp>

#include "engine/display/sprite.hpp"
#include "engine/gui/rect_component.hpp"

namespace Editor {

/** A ComboItems holds a specific item in a Combobox (string, id, etc.) */
class ComboItem
{
public:
  int id;
  std::string label;

  ComboItem(int id_, const std::string& label_)
    : id(id_), label(label_)
  {}
};

/** The Combobox class gives you a drop-down list of items to choose from. */
class Combobox : public GUI::RectComponent
{
protected:
  Sprite sprite;
  Rect list_rect;

  /** List of items in this Combobox */
  std::vector<ComboItem> item_list;

  /** item_list[current_item], -1 if none */
  int current_item;
  int hover_item;

  /** Whether or not the list is showing right now */
  bool drop_down;

  int list_offset;

public:
  Combobox(const Rect& rect);
  ~Combobox ();

  void add(int id, const std::string& str);

  /** Clears all items from combobox */
  virtual void clear();

  /** Return a pointer to the selected item.  Returns -1 if nothing is selected */
  int get_selected_item() { return current_item; }

  /** Sets the selected item to the given ComboItem.  Returns false if that item
      doesn't exist */
  bool set_selected_item(int id);

  /** Draw this Combobox and all of it's items if selected */
  void draw (DrawingContext& gc);

  /** Tells the gui_manager if the mouse is on top of the Combobox */
  bool is_at (int x, int y);

  /** Gets emmited when a button is pressed and released over the
      same component */
  void on_primary_button_press(int x, int y);

  void on_pointer_move(int x, int y);

  int get_box_offset();

  void update_layout() {}

  boost::signals2::signal<void (const ComboItem&)> on_select;

private:
  Combobox();
  Combobox (const Combobox&);
  Combobox& operator= (const Combobox&);

};

} // namespace Editor

#endif

/* EOF */
