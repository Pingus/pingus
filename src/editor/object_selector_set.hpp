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

#ifndef HEADER_PINGUS_EDITOR_OBJECT_SELECTOR_SET_HPP
#define HEADER_PINGUS_EDITOR_OBJECT_SELECTOR_SET_HPP

#include <vector>

#include "editor/object_selector_list.hpp"
#include "math/size.hpp"

namespace Editor {

class ObjectSelectorSet
{
private:
  ObjectSelectorList* list;
  Size thumb_size;

  typedef std::vector<std::unique_ptr<ObjectSelectorList::Object> > Objects;
  Objects objects;

  /** Used to record the scroll offset of the set */
  float offset;

public:
  ObjectSelectorSet(ObjectSelectorList* list, int thumb_w, int thumb_h);
  ~ObjectSelectorSet();

  void add(std::unique_ptr<ObjectSelectorList::Object> obj);

  int get_width() const;
  int get_height() const;

  int get_thumb_width() const  { return thumb_size.width; }
  int get_thumb_height() const { return thumb_size.height; }

  float get_offset() const { return offset; }
  void  set_offset(float o)  { offset = o; }

  const Objects& get_objects() const { return objects; }

private:
  ObjectSelectorSet (const ObjectSelectorSet&);
  ObjectSelectorSet& operator= (const ObjectSelectorSet&);
};

} // namespace Editor

#endif

/* EOF */
